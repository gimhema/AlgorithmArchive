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

auto create_reference_graph(vector<vector<int>> computers)
{
  // computers를 기반으로 그래프를 만들어 리턴합니다.
  // computers vector는 아래와 같이 구성되어있습니다.
  // [[1, 1, 0], [1, 1, 0], [0, 0, 1]]
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
    
    // 컴퓨터 인덱스 배열을 computers의 개수만큼 초기화합니다.
    for(int i = 0; i < computers.size(); i++)
    {
        computer_idx.push_back(i);
        network_complete_map[i] = false;
    }
    
    // 컴퓨터 인덱스 배열에 대하여
    for(int i = 0; i < computer_idx.size(); i++)
    {
        if(network_complete_map[i] == false)
        {
            // 인덱스가 아직 네트워크에 등록되지않았다면
            // i번째 인덱스에 대해서 탐색을 진행하며 네트워크를 구성한다.
            // BFS vs DFS ????       
        }
    }
        
    // 네트워크의 배열을 리턴한다.    
    answer = network.size();
    return answer;
}
