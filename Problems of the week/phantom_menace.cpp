// 1. Model this problem as a graph:
//  - Every planet is composed of an 'in' and an 'out' node, linked together by an edge of capacity 1
//  - Planets are linked together by edges between 'out' and 'in' according to the data (capacity can be 1 or infinite, it doesn't matter)
// 2. Calculate the min cut of the graph using the flow

#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,boost::property<boost::edge_capacity_t, long,boost::property<boost::edge_residual_capacity_t, long,boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


class edge_adder {
graph &G;
public:
explicit edge_adder(graph &G) : G(G) {}
void add_edge(int from, int to, long capacity) {
auto c_map = boost::get(boost::edge_capacity, G);
auto r_map = boost::get(boost::edge_reverse, G);
const auto e = boost::add_edge(from, to, G).first;
const auto rev_e = boost::add_edge(to, from, G).first;
c_map[e] = capacity;
c_map[rev_e] = 0;
r_map[e] = rev_e;
r_map[rev_e] = e;
}
};

using namespace std;

void testcase(){
  //INPUT
  int n, m, s, d;
  cin >> n >> m >> s >> d;
  
  //Graph
  graph G(2*n);
  edge_adder adder(G);
  for(int i=0; i<m; i++){
    int x, y; cin >> x >> y;
    adder.add_edge(x+n, y, INT_MAX); //Edge between 2 planets
  }
  
  for(int i=0; i<n; i++){
    adder.add_edge(i, i+n, 1);  //Edge between both nodes of a planet
  }
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Link source and target planets to corresponding nodes
  for(int i=0; i<s; i++){
    int x; cin >> x;
    adder.add_edge(source, x, INT_MAX);
  }
  for(int i=0; i<d; i++){
    int x; cin >> x;
    adder.add_edge(x+n, target, INT_MAX);
  }
  //Flow
  long flow = boost::push_relabel_max_flow(G, source, target);
  //OUTPUT
  cout << flow << endl;
  
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}
