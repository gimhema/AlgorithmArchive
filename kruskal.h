#pragma once
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<iostream>
using namespace std;

bool cmp(vector<int>& v1, vector<int>& v2) {
	// [1]번째 기준으로 정렬
	if (v1[2] == v2[2])
		return v1[0] < v2[0];
	else return v1[2] < v2[2];
}

void sort_2dVec(vector<vector<int>>& v)
{
	sort(v.begin(), v.end(), cmp);
}

class Kruskal
{
public:
	Kruskal(vector<vector<int>> _graph)
	{
		initializeGraph(_graph);
		int parents_length = _graph.size() + 1;
		vector<int> temp(6);
		parents = temp;
	}
	vector<vector<int>> graph;
	vector<int> parents;
	int total_cost;
private:
	void initializeGraph(vector<vector<int>> _graph)
	{
		graph = _graph;
	}
public:
	int find_parent(vector<int> &parent, int i)
	{
		if (parent[i] == i)
		{
			return i;
		}
		return find_parent(parent, parent[i]);
	}
	void calc_union(vector<int> &parent, int a, int b)
	{
		int a_parent = find_parent(parent, a);
		int b_parent = find_parent(parent, b);

		if (a_parent < b_parent)
		{
			parent[b_parent] = a_parent;
		}
		else
		{
			parent[a_parent] = b_parent;
		}
	}

	void calc_kruskal()
	{
		for (int i = 0; i < parents.size(); i++)
		{
			parents[i] = i;
		}

		sort_2dVec(graph);

		for (int i = 0; i < graph.size(); i++)
		{
			if (find_parent(parents, graph[i][0]) != find_parent(parents, graph[i][1])) {
				total_cost += graph[i][2];
				calc_union(parents, graph[i][0], graph[i][1]);
			}
		}

		cout << "Krusukal Result : " << total_cost;
	}

};
