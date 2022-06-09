#include <string>
#include <vector>
#include <iostream>

using namespace std;

template<typname T>

struct Edge
{
  unsigned src;
  unsigned dst;
  T weight;
}

class Graph
{
public:
 // N개의 정점으로 이루어짐
 Graph(unsigned N) : V(N) {}
 
 // 그래프의 정점 개수를 리턴한다.
 auto vertices() const { return V; }
 
 auto& edges() const { return edge_list; }
 
 // 파라미터 정점 v로부터 나가는 모든 에지를 리턴한다.
 auto edges(unsinged v) const
 {
  vector<Edge<T>> edges_from_v;
  for(auto& e : edge_list)
  {
    if(e.src == v) edge_from_v.emplace_back(e);
  }
  return edge_from_v;
 }
 
 void add_edge(Edge<T>&& e)
 {
  if(e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
    { edge_list.emplace_back(e); }
  else
   { cout << "Error ! ! !" << endl; }
 }
  
private:
 unsigned V; // 정점의 개수
 vector<Edge<T>> edge_list;
};
















