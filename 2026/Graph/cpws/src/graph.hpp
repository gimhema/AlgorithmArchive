#include <bits/stdc++.h>
using namespace std;

using Visited = vector<uint8_t>;

vector<int> bfs_order(const vector<vector<int>>& graph, int start) {
    const int n = (int)graph.size();
    vector<int> order;
    order.reserve(n);

    Visited visited(n, 0);
    queue<int> q;

    visited[start] = 1;
    q.push(start);

    while (!q.empty()) {
        int cur = q.front(); q.pop();
        order.push_back(cur);

        for (int nxt : graph[cur]) {
            if (visited[nxt]) continue;
            visited[nxt] = 1;
            q.push(nxt);
        }
    }
    return order;
}

void dfs_rec_impl(const vector<vector<int>>& graph, int u, Visited& visited, vector<int>& order) {
    visited[u] = 1;
    order.push_back(u);
    for (int v : graph[u]) {
        if (!visited[v]) dfs_rec_impl(graph, v, visited, order);
    }
}

vector<int> dfs_order_recursive(const vector<vector<int>>& graph, int start) {
    const int n = (int)graph.size();
    vector<int> order;
    order.reserve(n);

    Visited visited(n, 0);
    dfs_rec_impl(graph, start, visited, order);
    return order;
}

vector<int> dfs_order_iterative(const vector<vector<int>>& graph, int start) {
    const int n = (int)graph.size();
    vector<int> order;
    order.reserve(n);

    Visited visited(n, 0);
    vector<int> st;
    st.reserve(n);

    st.push_back(start);

    while (!st.empty()) {
        int u = st.back();
        st.pop_back();

        if (visited[u]) continue;
        visited[u] = 1;
        order.push_back(u);

        for (auto it = graph[u].rbegin(); it != graph[u].rend(); ++it) {
            int v = *it;
            if (!visited[v]) st.push_back(v);
        }
    }
    return order;
}
