#include <string>
#include <vector>

using namespace std;

// 쿼리를 탐색해서 가장 큰 세대값을 찾는다.
// 그 세대값을 기준으로 유전 트리를 만든다.
vector<string> create_child(string parent)
{
    vector<string> result;
    if(parent == "RR")
    {
        result = {"RR", "RR", "RR", "RR"};
    }
    else if(parent == "Rr")
    {
        result = {"RR", "Rr", "Rr", "rr"};
    }
    else if(parent == "rr")
    {
        result = {"rr", "rr", "rr", "rr"};
    }
    return result;
}


int find_max_n(vector<vector<int>> queries)
{
    int result = 0;
    for(int i = 0; i < queries.size(); i++)
    {
        if(queries[i][0] >= result)
        {
            result = queries[i][0];
        }
    }
    return result;
}

vector<vector<string>> make_tree(int n)
{
    vector<vector<string>> tree;
    tree.push_back({"Rr"});
    for(int i = 1; i < n; i++)
    {
        tree.push_back({});
    }
    
    for(int k = 1; k < n; k++)
    {
        vector<string> parent = tree[k - 1];
        for(int j = 0; j < parent.size(); j++)
        {
            vector<string> child = create_child(parent[j]);
            tree[k].push_back(child[0]);
            tree[k].push_back(child[1]);
            tree[k].push_back(child[2]);
            tree[k].push_back(child[3]);
        }
    }
    return tree;
}


vector<string> answer_query(vector<vector<int>> queries, vector<vector<string>> tree)
{
    vector<string> answer;
    
    for(int i = 0; i < queries.size(); i++)
    {
        int left = queries[i][0] - 1;
        int right = queries[i][1] - 1;
        answer.push_back(tree[left][right]);
    }
    
    return answer;
}

vector<string> solution(vector<vector<int>> queries) {
    vector<string> answer;
    int max_n = find_max_n(queries);
    vector<vector<string>> tree = make_tree(max_n);
    answer = answer_query(queries, tree);
    // answer = {"RR"};
    return answer;
}

