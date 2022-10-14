#include<iostream>
#include<algorithm>
#include<queue>
#include<string>
#include<set>
#include<map>
#include<queue>
#include<vector>
#include<unordered_map>
#include<limits>
#include<climits>
using namespace std;

const int UNKNOWN = INT_MAX;

template <typename T>
struct Edge
{
    unsigned src;
    unsigned dst;
    T weight;
};

template <typename T>
class Graph
{
public:
    Graph(unsigned N) : V(N) {}
    
    auto vertices() const { return V; }

    auto& edges() const { return edge_list; }

    auto edges(unsigned v) const
    {
        vector<Edge<T>> edge_from_v;
        for (auto& e : edge_list)
        {
            if (e.src == v)
                edge_from_v.emplace_back(e);
        }
        return edge_from_v;
    }

    void add_edge(Edge<T>&& e)
    {
        if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
        {
            edge_list.emplace_back(e);
        }
        else
        {
            cout << "Exception " << endl;
        }
    }

    template <typename U>
    friend ostream& operator<< (ostream& os, const Graph<U>& G);

private:
    unsigned V;
    vector<Edge<T>> edge_list;
};

template <typename U>
ostream& operator<< (ostream& os, const Graph<U>& G)
{
    for (unsigned i = 1; i < G.vertices(); i++)
    {
        os << i << ":\t";

        auto edges = G.edges(i);
        for (auto& e : edges)
        {
            os << "{" << e.dst << ": " << e.weight << "}, ";
        }
        os << endl;
    }

    return os;
}

template <typename T>
auto create_reference_graph()
{
    Graph<T> G(9);

    map<unsigned, vector<pair<unsigned, T>>> edge_map;

    edge_map[1] = { {2, 2}, {5, 3} };
    edge_map[2] = { {1, 2}, {5, 5}, {4, 1} };
    edge_map[3] = { {4, 2}, {7, 3} };
    edge_map[4] = { {2, 1}, {3, 2}, {5, 2}, {6, 4}, {8, 5} };
    edge_map[5] = { {1, 3}, {2, 5}, {4, 2}, {8, 3} };
    edge_map[6] = { {4, 4}, {7, 4}, {8, 1} };
    edge_map[7] = { {3, 3}, {6, 4} };
    edge_map[8] = { {4, 5}, {5, 3}, {6, 1} };

    for (auto& i : edge_map)
    {
        for (auto& j : i.second)
        {
            G.add_edge(Edge<T>{i.first, j.first, j.second});
        }
    }
    return G;
}

template <typename T>
struct Label
{
    unsigned ID;
    T distance;

    inline bool operator> (const Label<T>& l) const
    {
        return this->distance > l.distance;
    }
};

template <typename T>
auto prim_MST(const Graph<T>& G, unsigned src)
{
    priority_queue<Label<T>, vector<Label<T>>, greater<Label<T>>> heap;
    
    vector<T> distance (G.vertices(), numeric_limits<T>::max());

    set<unsigned> visited;
    vector<unsigned> MST;

    heap.emplace(Label<T>{src, 0});

    while (!heap.empty())
    {
        auto current_vertex = heap.top();
        heap.pop();

        if (visited.find(current_vertex.ID) == visited.end())
        {
            MST.push_back(current_vertex.ID);

            for (auto& e : G.edges(current_vertex.ID))
            {
                auto neighbor = e.dst;
                auto new_distance = e.weight;

                if (new_distance < distance[neighbor])
                {
                    heap.emplace(Label<T>{neighbor, new_distance});
                    distance[neighbor] = new_distance;
                }
            }

            visited.insert(current_vertex.ID);
        }
    }

    return MST;
}

template<typename T>
auto dijkstra_shortest_path(const Graph<T>& G, unsigned src, unsigned dst)
{
    priority_queue<Label<T>, vector<Label<T>>, greater<Label<T>>> heap;

    vector<T> distance(G.vertices(), numeric_limits<T>::max());

    set<unsigned> visited;
    vector<unsigned> parent(G.vertices());

    heap.emplace(Label<T>{src, 0});
    parent[src] = src;

    while (!heap.empty())
    {
        auto current_vertex = heap.top();
        heap.pop();

        if (current_vertex.ID == dst)
        {
            cout << current_vertex.ID << "번 도착" << endl;
            break;
        }
        if (visited.find(current_vertex.ID) == visited.end())
        {
            cout << current_vertex.ID << " 번 안착 " << endl;

            for (auto& e : G.edges(current_vertex.ID))
            {
                auto neighbor = e.dst;
                auto new_distance = current_vertex.distance + e.weight;

                if (new_distance < distance[neighbor])
                {
                    heap.emplace(Label<T>{neighbor, new_distance});
                    parent[neighbor] = current_vertex.ID;
                    distance[neighbor] = new_distance;
                }
            }
            visited.insert(current_vertex.ID);
        }
    }
    vector<unsigned> shortest_path;
    auto current_vertex = dst;

    while (current_vertex != src)
    {
        shortest_path.push_back(current_vertex);
        current_vertex = parent[current_vertex];
    }

    shortest_path.push_back(src);
    reverse(shortest_path.begin(), shortest_path.end());

    return shortest_path;
}

struct SignedEdge
{
    int src;
    int dst;
    int weight;
};

vector<int> BellmanFord(vector<SignedEdge> edges, int V, int start)
{
    vector<int> distance(V, UNKNOWN);
    distance[start] = 0;

    for (int i = 0; i < V - 1; i++)
    {
        for (auto& e : edges)
        {
            if (distance[e.src] == UNKNOWN)
            {
                cout << "Distance Update Skip . ." << endl;
                continue;
            }
            if (distance[e.dst] > distance[e.src] + e.weight)
            {
                cout << "Destination Distance : " << distance[e.dst];
                cout << "Destination Src : " << distance[e.src] + e.weight;
                distance[e.dst] = distance[e.src] + e.weight;
                cout << "Distance Update : " << distance[e.dst] << endl;
            }
        }
    }

    return distance;

}



int main() {

    //using T = unsigned;

    //auto G = create_reference_graph<T>();
    //cout << "Graph . . " << endl;
    //cout << G << endl;

    ////auto MST = prim_MST<T>(G, 1);
    //auto MST = dijkstra_shortest_path<T>(G, 1, 6);

    ////cout << "Prim Result . ." << endl;
    //cout << "Dijkstra Result . ." << endl;

    //for (int i = 0; i < MST.size(); i++)
    //{
    //    cout << MST[i] << " ";
    //}
    //for (auto v : MST)
    //{
    //    //cout << v << endl;
    //    cout << v << " ";
    //}
    //cout << endl;

    int V = 5;
    vector<SignedEdge> edges;
    vector<vector<int>> edge_map{
        {0, 1, 3},
        {1, 2, 5},
        {1, 3, 10},
        {3, 2, -7},
        {2, 4, 2}
    };

    for (auto& e : edge_map)
    {
        edges.emplace_back(SignedEdge{ e[0], e[1], e[2] });
    }

    int start = 0;
    vector<int> distance = BellmanFord(edges, V, start);

    cout << start << "번 최소 거리" << endl;

    for (int i = 0; i < distance.size(); i++)
    {
        if (distance[i] == UNKNOWN)
        {
            cout << i << "번 정점 : 방문하지 않음 " << endl;
        }
        else
        {
            cout << i << "번 정점 거리 : " << distance[i] << endl;
        }
    }


    return 0;
}
