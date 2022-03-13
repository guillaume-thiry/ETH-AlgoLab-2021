// 1. Create a graph where if intersection is composed of two nodes (in node and out node)
// 2. Link an out node with an in node for neighboring intersections and capacity 1
// 3. Link in node and out node of same intersection with capacity C
// 4. Link all external nodes to a target node (exit)
// 5. Link all initial positions of knights to a start node with capacity 1
// 6. Calculate flow
// Note: No need to care for paths crossing each other in opposite directions as we can cancel them (see lecture)

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
  int m, n, k, c; cin >> m >> n >> k >> c;
  graph G(2*n*m);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G); //exit node
  
  //Link each out node with neighboring in nodes and/or exit node
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      adder.add_edge(2*(j+i*m), 2*(j+i*m)+1,c); //here also link in and out of same intersection with capacity C
      if(j<m-1){
        adder.add_edge(2*(j+i*m)+1, 2*(j+i*m+1),1);
      }else{
        adder.add_edge(2*(j+i*m)+1, target,1);
      }
      if(j>0){
        adder.add_edge(2*(j+i*m)+1, 2*(j+i*m-1),1);
      }else{
        adder.add_edge(2*(j+i*m)+1, target,1);
      }
      if(i>0){
        adder.add_edge(2*(j+i*m)+1, 2*(j+i*m-m),1);
      }else{
        adder.add_edge(2*(j+i*m)+1, target,1);
      }
      if(i<n-1){
        adder.add_edge(2*(j+i*m)+1, 2*(j+i*m+m),1);
      }else{
        adder.add_edge(2*(j+i*m)+1, target,1);
      }
    }
  }
  //Link start node with initial positions of knights
  for(int a=0; a<k; a++){
    int x, y; cin >> x >> y;
    adder.add_edge(source, 2*(x+y*m), 1);
  }
  
  //Flow and OUTPUT
  long flow = boost::push_relabel_max_flow(G, source, target);
  cout << flow << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}