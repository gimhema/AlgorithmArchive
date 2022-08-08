#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct node
{
  int idx;
  unordered_map<int, int> conn; // Key : Node Index, Value : Cost
};

unordered_map<int, bool> connected_island;
vector<node> node_vec;
int count = 0;

bool is_include(int island_idx)
{
    return connected_island[island_idx];
}

void add_node_vec_cost_info(int idx, int connected_idx, int cost)
{
    node_vec[idx].conn.insert({connected_idx, cost});
}

void initialize_node_info(int n, vector<vector<int>> &costs)
{
    for(int i = 0; i < costs.size(); i++)
    {
        int l_node = costs[i][0];
        int r_node = costs[i][1];
        int _cost = costs[i][2];
        add_node_vec_cost_info(l_node, r_node, _cost);
        add_node_vec_cost_info(r_node, l_node, _cost);
    }
}

void initialize_conncted_island(int n)
{
    for(int i = 0; i < n; i++)
    {
        node _n;
        _n.idx = i;
        node_vec.push_back(_n);
        connected_island.insert({i, false});
    }
}

int get_min_cost(node n)
{
    int result = 9999;
    int target_idx = -1;
    for(auto elem : n.conn){
        // 포함되어있는지를 확인한다.
        if(elem.second <= result)
        {
            target_idx = elem.first;
            result = elem.second;
        }
    }
    connected_island[n.idx] = true;
    connected_island[target_idx] = true;
    return result;
}



int solution(int n, vector<vector<int>> costs) {
    int answer = 0;
    initialize_conncted_island(n);
    initialize_node_info(n, costs);
    
    // 가장 최소비용을 선택한다.
    // 만약 선택한 노드가 이미 포함되어있다면 다른 노드를 선택한다.
    for(int i = 0; i < costs.size(); i++)
    {
        if(is_include(costs[i][0]) == false)
        {
            int value = get_min_cost(node_vec[costs[i][0]]);
            answer = answer + value;   
        }else if(is_include(costs[i][1]) == false)
        {
            int value = get_min_cost(node_vec[costs[i][1]]);
            answer = answer + value;
        }
    }
    
    return answer;
}




