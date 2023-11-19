#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Node {
    string name;
};


struct Item {
    int priority;
    Node* value;
};


class MinHeap {

private:
    void swap(int i, int j) {
        Item temp = heapArr[i];
        heapArr[i] = heapArr[j];
        heapArr[j] = temp;
    }
    int parent(int i) {
        return i / 2;
    }
    int left(int i) {
        return 2 * i;
    }
    int right(int i) {
        return (2 * i) + 1;
    }
public:
    Item* heapArr;
    int size;
    int head;
    MinHeap(int heap_size) {

        heapArr = new Item[heap_size];
        for (int i = 0; i < heap_size; i++) {
            heapArr[i].priority = 0;
            heapArr[i].value = 0;
        }
        head = 1;
        size = heap_size;
    }

    Item pop() {
        //Save top item
        Item out = heapArr[1];

        //Swap root with head and remove head
        head--;
        swap(1, head);
        heapArr[head].priority = 0;

        int cur = 1;
        int value = heapArr[1].priority;
        while (left(cur) < head && right(cur) < head)
        {
            int l = heapArr[left(cur)].priority;
            int r = heapArr[right(cur)].priority;
            if (l < value || r < value) {
                if (l < value && r < value) {
                    if (l < r) {
                        swap(cur, left(cur));
                        cur = left(cur);
                    }
                    else {
                        swap(cur, right(cur));
                        cur = right(cur);
                    }
                }
                else if (l < value) {
                    swap(cur, left(cur));
                    cur = left(cur);
                }
                else if (r < value) {
                    swap(cur, right(cur));
                    cur = right(cur);
                }
            }
            else {
                break;
            }
        }

        return out;

    }
    void insert(Item item) {

        heapArr[head] = item;
        int cur = head;
        head++;

        if (cur == 1) {
            return;
        }
        
            while (cur != 1) {

                if (heapArr[cur].priority < heapArr[parent(cur)].priority) {
                    swap(cur, parent(cur));
                }
                else {
                    break;
                }
                cur = parent(cur);
            }

    }
    void print_heap() {
        for (int i = 1; i < size; i++) {
            if (heapArr[i].value == nullptr) {
                cout << "(" << heapArr[i].priority << "," << ")" << ", ";
            }
            else {
                cout << "(" << heapArr[i].priority << "," << heapArr[i].value->name << ")" << ", ";
            }
        }
        cout << endl;
    }

    bool is_empty() {
        return head == 1;
    }
};



class Graph {
    //Delete node from graph
    //Implement errors
public:
    int size ;

    unordered_map<Node*, list<pair<Node*, int>>*> edgeMap;
    unordered_map<string, Node*> nodes;

    Graph() {
        size = 0;
    }

    void add_node(string node_name) {
        Node* node = new Node{ node_name };
        nodes[node_name] = node;
    }

    void add_edge(string nodeAName, string nodeBName, int weight) {

        //Check for existence
        Node *nodeA = nodes[nodeAName];
        Node* nodeB = nodes[nodeBName];


        //If edge map already contains node A
        if (edgeMap.count(nodeA)) {
            edgeMap[nodeA]->push_back(make_pair(nodeB, weight));
        }
        else {
            list<pair<Node*, int>>* edgeList = new list<pair<Node*, int>>();


            edgeList->push_back(make_pair(nodeB, weight));

            edgeMap[nodeA] = edgeList;


        }
        size++;
    }

    //dijkstra
    list<Node*> closest_k_nodes(string start_node, int k) {
        
        //Find node in graph by name

        Node* node = nodes[start_node];

        //Create priority queue for nodes
        MinHeap priorityQueue(size + 1);
        priorityQueue.insert(Item{ 0, node });

        //Create a set for visted nodes
        unordered_set<Node*> visitedNodes;
        visitedNodes.insert(node);

        //Create distance hash map
        unordered_map<Node*, int> distances;
        for (auto& it : nodes) {
            distances[it.second] = INT32_MAX;
        }
        distances[node] = 0;

        while (!priorityQueue.is_empty()) {

            //Get next closest node
            Item top = priorityQueue.pop();

            //Unpackage Item from heap
            int cur_priority = top.priority;
            Node* cur_node = top.value;

            //Mark current node as visited
            visitedNodes.insert(cur_node);
            
            cout << "\nProcessing Node: " << cur_node->name << endl;

            
            //if edgeMap doesnt contain the node (i.e node has no outgoing edges)
            if (!edgeMap.count(cur_node)) {
                //skip while loop
                continue;
            }

            //Loop over every edge
            for (auto edge: *edgeMap[cur_node]) {

                //Unpackage Item from heap
                Node* new_node =  edge.first;
                int weight = edge.second;

                //If new node hasnt been visited
                if (!visitedNodes.count(new_node)) {


                    //Add to the priority queue with higher priority
                    priorityQueue.insert(Item{ cur_priority + weight, new_node });
                }

                //Augment shortest distances
                if (distances[new_node] > cur_priority + weight) {
                    distances[new_node] = cur_priority + weight;
                }
                
            }


        }
        vector<pair<Node*, int>> distances_vec(distances.begin(), distances.end());
        
        //Sort distances vec
        sort(distances_vec.begin(), distances_vec.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
            });

        //Create list of closest nodes
        list<Node*> closestNodes;

        for (int i = 1; i < k + 1; i++) {
            closestNodes.push_back(distances_vec[i].first);
        }
        return closestNodes;
    }



private:

};


int main()
{
    Graph myGraph;


    myGraph.add_node("A");
    myGraph.add_node("B");
    myGraph.add_node("C");
    myGraph.add_node("D");
    myGraph.add_node("E");
    myGraph.add_node("F");

    myGraph.add_edge("A", "B", 10);
    myGraph.add_edge("A", "D", 13);
    myGraph.add_edge("B", "C", 15);
    myGraph.add_edge("C", "F", 1);
    myGraph.add_edge("D", "E", 20);

    list<Node*> out = myGraph.closest_k_nodes("A", 3);

    cout << out.size() << endl;
    cout << "done " << endl;
    for (auto n : out) {
        cout << n->name << endl;
    }



}