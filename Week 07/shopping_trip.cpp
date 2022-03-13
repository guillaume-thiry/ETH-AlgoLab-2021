// 1. Create a graph depicting all the intersections and streets
// 2. Connect all the shops to a target (new node)
// 3. Calculate flow between the starting point (node 0) and the target

#include <iostream>
#include <vector>

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
  int n, m, s; cin >> n >> m >> s;
  
  //Graph
  graph G(n);
  edge_adder adder(G);
  
  const vertex_desc target = boost::add_vertex(G);
  
  for(int i=0; i<s; i++){
    int x; cin >> x;
    adder.add_edge(x, target, 1);
  }
  
  for(int j=0; j<m; j++){
    int x, y; cin >> x >> y;
    adder.add_edge(x,y,1);
    adder.add_edge(y,x,1);
  }
  
  //Flow
  long flow = boost::push_relabel_max_flow(G, 0, target);
  
  //OUTPUT
  if(flow==s){
    cout << "yes" << endl;
  }else{
    cout << "no" << endl;
  }
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}