#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <queue>

using namespace std;

template<typename T>
struct Edge
{
    unsigned src;
    unsigned dst;
    T weight;
};

template<typename T>
class Graph
{
private:
    unsigned V;
    vector<Edge<T>> edge_list;
public:
    Graph(unsigned N) : V(N) {}
    auto vertices() const {return V;}
    auto& edges() const {return edge_list;}
    auto edges(unsigned v) const
    {
        vector<Edge<T>> edges_from_v;
        for(auto& e : edge_list)
        {
            if(e.src == v)
            {
                edges_from_v.emplace_back(e);
            }
        }
        return edges_from_v;
    }
    void add_edge(Edge<T>&& e)
    {
        if(e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
        {
            edge_list.emplace_back(e);
        }
    }
}

template <typename T>
auto create_reference_graph(vector<vector<int>> computers)
{
    // computers를 기반으로 그래프를 만들어 리턴합니다.
    // computers vector는 아래와 같이 구성되어있습니다.
    // [[1, 1, 0], [1, 1, 0], [0, 0, 1]]
    int num_vertice = computers.size();
    Graph<T> G(num_vertice);
    map<unsigned, vector<pair<unsigned, T>>> edge_map;
    
    // 가중치가 없는 그래프로 간주
    // num_vertice만큼 반복문을 돌린다.
    for(int i = 0; i < num_vertice; i++)
    {
        for(int j = 0; j < computers[j].size() < j++)
        {
            // computers vector 내부에서
            if(i != j)
            {
                // 자기자신은 항상 연결되어있기때문에 제외
                if(computers[i][j] == 1)
                {
                    // i, j가 연결되어있다면 
                    edge_map[i].push_back({j, 0})
                }
            }
        }
    }
    
    for(auto& i : edge_map)
    {
        for(auto& j : i.second)
        {
            G.add_edge(Edge<T>({i.first, j.first, j.second}));
        }
    }
    
    return G;
}

auto BFS(const Graph<T>& G, unsigned start)
{
    
}

int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    
    // 컴퓨터의 인덱스를 곳에 저장합니다.
    vector<int> computer_idxs;
    // 만들어진 네트워크 배열을 이곳에 저장합니다.
    vector<vector<int>> network;
    //현재 컴퓨터의 네트워크가 구성되었는지를 판단하는 map입니다.
    unordered_map<int, bool> network_complete_map;
    
    // computers vector를 기반으로 그래프를 구성합니다.
    auto computer_graph = create_reference_graph(comptuers);
    
    // 컴퓨터 인덱스 배열을 computers의 개수만큼 초기화합니다.
    for(int i = 0; i < computers.size(); i++)
    {
        computer_idx.push_back(i);
        network_complete_map[i] = false;
    }
    
    // 탐색을 진행한다.
    BFS(computer_graph, 0);
        
    // 네트워크의 배열을 리턴한다.    
    answer = network.size();
    return answer;
}



