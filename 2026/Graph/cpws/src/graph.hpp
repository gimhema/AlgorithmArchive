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

struct Edge {
    int to;
    long long w;
    Edge(int to, long long w) : to(to), w(w) {}
};

pair<vector<long long>, vector<int>> dijkstra(const vector<vector<Edge>>& g, int start) {

    const long long INF = (1LL << 60);
    const int n = (int)g.size();

    vector<long long> dist(n, INF);
    vector<int> parnet(n, -1);

     priority_queue<pair<long long,int>,
                   vector<pair<long long,int>>,
                   greater<pair<long long,int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while(!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if(d != dist[u]) continue;

        for(const auto& e : g[u]) {

            long long nd = d + e.w;
            if(nd < dist[e.to]) {
                dist[e.to] = nd;
                parnet[e.to] = u;
                pq.push({nd, e.to});
            }
        }
    }
    return {dist, parnet};
}

tuple<vector<long long>, vector<int>, bool> bellman_ford(const vector<vector<Edge>>& g, int start) {
    const long long INF = (1LL << 60);
    const int n = (int)g.size();

    vector<long long> dist(n, INF);
    vector<int> parent(n, -1);

    dist[start] = 0;

    for (int i = 0; i < n - 1; ++i) {
        bool changed = false;

        for (int u = 0; u < n; ++u) {
            if (dist[u] == INF) continue;

            for (const auto& e : g[u]) {
                long long nd = dist[u] + e.w;
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    parent[e.to] = u;
                    changed = true;
                }
            }
        }

        if (!changed) break;
    }

    bool hasNegCycle = false;
    for (int u = 0; u < n; ++u) {
        if (dist[u] == INF) continue;

        for (const auto& e : g[u]) {
            long long nd = dist[u] + e.w;
            if (nd < dist[e.to]) {
                hasNegCycle = true;
                break;
            }
        }
        if (hasNegCycle) break;
    }

    return {dist, parent, hasNegCycle};
}
