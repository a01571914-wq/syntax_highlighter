#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
using namespace std;

// ============================================================
//  Graph - Adjacency list representation with BFS, DFS,
//  connectivity, cycle detection, topological sort, and
//  Dijkstra's shortest path
// ============================================================

class Graph {
private:
    int vertices;
    bool directed;
    vector<vector<int>> adjList;          // unweighted adjacency list
    vector<vector<pair<int,int>>> wAdjList; // weighted adjacency list: (neighbor, weight)

public:
    Graph(int v, bool isDirected = false)
        : vertices(v), directed(isDirected), adjList(v), wAdjList(v) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        if (!directed) adjList[v].push_back(u);
    }

    void addWeightedEdge(int u, int v, int weight) {
        wAdjList[u].push_back({v, weight});
        if (!directed) wAdjList[v].push_back({u, weight});
    }

    int vertexCount() const { return vertices; }

    // Breadth-first search from a starting vertex; returns visit order
    vector<int> BFS(int start) const {
        vector<int> order;
        vector<bool> visited(vertices, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            order.push_back(node);
            for (int neighbor : adjList[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return order;
    }

    // Recursive depth-first search helper
    void DFSUtil(int node, vector<bool>& visited, vector<int>& order) const {
        visited[node] = true;
        order.push_back(node);
        for (int neighbor : adjList[node])
            if (!visited[neighbor])
                DFSUtil(neighbor, visited, order);
    }

    // Recursive DFS from a starting vertex; returns visit order
    vector<int> DFS(int start) const {
        vector<int> order;
        vector<bool> visited(vertices, false);
        DFSUtil(start, visited, order);
        return order;
    }

    // Iterative DFS using an explicit stack
    vector<int> DFSIterative(int start) const {
        vector<int> order;
        vector<bool> visited(vertices, false);
        stack<int> s;
        s.push(start);
        while (!s.empty()) {
            int node = s.top();
            s.pop();
            if (visited[node]) continue;
            visited[node] = true;
            order.push_back(node);
            // push neighbors in reverse so traversal order matches recursive DFS
            for (auto it = adjList[node].rbegin(); it != adjList[node].rend(); ++it)
                if (!visited[*it]) s.push(*it);
        }
        return order;
    }

    // Check whether the (undirected) graph is connected
    bool isConnected() const {
        if (vertices == 0) return true;
        vector<int> visitOrder = BFS(0);
        return (int)visitOrder.size() == vertices;
    }

    // Count connected components in an undirected graph
    int countConnectedComponents() const {
        vector<bool> visited(vertices, false);
        int components = 0;
        for (int i = 0; i < vertices; i++) {
            if (!visited[i]) {
                components++;
                vector<int> comp = BFS(i);
                for (int node : comp) visited[node] = true;
            }
        }
        return components;
    }

    // Detect a cycle in an undirected graph
    bool hasCycleUndirectedUtil(int node, int parent, vector<bool>& visited) const {
        visited[node] = true;
        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                if (hasCycleUndirectedUtil(neighbor, node, visited)) return true;
            } else if (neighbor != parent) {
                return true;
            }
        }
        return false;
    }

    bool hasCycleUndirected() const {
        vector<bool> visited(vertices, false);
        for (int i = 0; i < vertices; i++)
            if (!visited[i] && hasCycleUndirectedUtil(i, -1, visited))
                return true;
        return false;
    }

    // Detect a cycle in a directed graph using colors (white/gray/black)
    bool hasCycleDirectedUtil(int node, vector<int>& state) const {
        state[node] = 1; // gray: in progress
        for (int neighbor : adjList[node]) {
            if (state[neighbor] == 1) return true; // back edge -> cycle
            if (state[neighbor] == 0 && hasCycleDirectedUtil(neighbor, state)) return true;
        }
        state[node] = 2; // black: done
        return false;
    }

    bool hasCycleDirected() const {
        vector<int> state(vertices, 0);
        for (int i = 0; i < vertices; i++)
            if (state[i] == 0 && hasCycleDirectedUtil(i, state))
                return true;
        return false;
    }

    // Topological sort for a DAG using DFS-based ordering
    vector<int> topologicalSort() const {
        vector<bool> visited(vertices, false);
        stack<int> result;

        // local recursive helper via lambda using std::function-style recursion
        auto helper = [&](int start) {
            stack<pair<int,int>> callStack; // (node, neighbor index)
            callStack.push({start, 0});
            visited[start] = true;
            while (!callStack.empty()) {
                auto& [node, idx] = callStack.top();
                if (idx < (int)adjList[node].size()) {
                    int neighbor = adjList[node][idx];
                    idx++;
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        callStack.push({neighbor, 0});
                    }
                } else {
                    result.push(node);
                    callStack.pop();
                }
            }
        };

        for (int i = 0; i < vertices; i++)
            if (!visited[i]) helper(i);

        vector<int> order;
        while (!result.empty()) {
            order.push_back(result.top());
            result.pop();
        }
        return order;
    }

    // Dijkstra's algorithm for shortest paths from a source vertex (weighted graph)
    vector<int> dijkstra(int source) const {
        vector<int> dist(vertices, INT_MAX);
        dist[source] = 0;

        // min-heap of (distance, vertex)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue; // stale entry

            for (auto& [v, w] : wAdjList[u]) {
                if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    void printAdjList() const {
        for (int i = 0; i < vertices; i++) {
            cout << "  " << i << ": ";
            for (int neighbor : adjList[i]) cout << neighbor << " ";
            cout << endl;
        }
    }
};

void printVector(const vector<int>& v, const string& label) {
    cout << label << ": ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

int main() {
    cout << "=== Undirected Graph Traversals ===" << endl;
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    cout << "Adjacency list:" << endl;
    g.printAdjList();

    printVector(g.BFS(0), "BFS from 0");
    printVector(g.DFS(0), "DFS from 0 (recursive)");
    printVector(g.DFSIterative(0), "DFS from 0 (iterative)");

    cout << endl << "=== Connectivity ===" << endl;
    cout << "Is connected: " << (g.isConnected() ? "yes" : "no") << endl;
    cout << "Connected components: " << g.countConnectedComponents() << endl;

    cout << endl << "Adding a disconnected vertex (6) and edge (none)..." << endl;
    Graph g2(7);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 3);
    g2.addEdge(2, 4);
    g2.addEdge(3, 5);
    // vertex 6 has no edges
    cout << "Is connected: " << (g2.isConnected() ? "yes" : "no") << endl;
    cout << "Connected components: " << g2.countConnectedComponents() << endl;

    cout << endl << "=== Cycle Detection (Undirected) ===" << endl;
    Graph treeGraph(5);
    treeGraph.addEdge(0, 1);
    treeGraph.addEdge(0, 2);
    treeGraph.addEdge(1, 3);
    treeGraph.addEdge(1, 4);
    cout << "Tree graph has cycle: " << (treeGraph.hasCycleUndirected() ? "yes" : "no") << endl;

    Graph cyclicGraph(4);
    cyclicGraph.addEdge(0, 1);
    cyclicGraph.addEdge(1, 2);
    cyclicGraph.addEdge(2, 3);
    cyclicGraph.addEdge(3, 0);
    cout << "Square graph (with cycle) has cycle: " << (cyclicGraph.hasCycleUndirected() ? "yes" : "no") << endl;

    cout << endl << "=== Cycle Detection and Topological Sort (Directed) ===" << endl;
    Graph dag(6, true);
    dag.addEdge(5, 2);
    dag.addEdge(5, 0);
    dag.addEdge(4, 0);
    dag.addEdge(4, 1);
    dag.addEdge(2, 3);
    dag.addEdge(3, 1);
    cout << "DAG has cycle: " << (dag.hasCycleDirected() ? "yes" : "no") << endl;
    printVector(dag.topologicalSort(), "Topological order");

    Graph directedCycle(3, true);
    directedCycle.addEdge(0, 1);
    directedCycle.addEdge(1, 2);
    directedCycle.addEdge(2, 0);
    cout << "Directed cycle graph has cycle: " << (directedCycle.hasCycleDirected() ? "yes" : "no") << endl;

    cout << endl << "=== Dijkstra's Shortest Path ===" << endl;
    Graph weighted(6);
    weighted.addWeightedEdge(0, 1, 4);
    weighted.addWeightedEdge(0, 2, 1);
    weighted.addWeightedEdge(2, 1, 2);
    weighted.addWeightedEdge(1, 3, 5);
    weighted.addWeightedEdge(2, 3, 8);
    weighted.addWeightedEdge(3, 4, 3);
    weighted.addWeightedEdge(4, 5, 1);
    weighted.addWeightedEdge(3, 5, 6);

    vector<int> distances = weighted.dijkstra(0);
    cout << "Shortest distances from vertex 0:" << endl;
    for (int i = 0; i < weighted.vertexCount(); i++) {
        cout << "  to " << i << ": ";
        if (distances[i] == INT_MAX) cout << "unreachable" << endl;
        else cout << distances[i] << endl;
    }

    return 0;
}