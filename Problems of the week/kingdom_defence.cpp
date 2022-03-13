// 1. Model the problem as a graph:
//    - Every city is a node
//    - Roads between cities are edges
//    - To ensure the minimal flow in edges, shift the edge capacity to begin at 0 and increase supply and demand of the corresponding vertices

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,boost::property<boost::edge_capacity_t, long,boost::property<boost::edge_residual_capacity_t, long,boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;

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
c_map[rev_e] = 0; // reverse edge has no capacity!
r_map[e] = rev_e;
r_map[rev_e] = e;
}
};

using namespace std;

void testcase(){
  //INPUT
  int l, q; cin >> l >> q;
  vector<int> supplies;
  vector<int> demands;
  for(int i=0; i<l; i++){
    int g, d; cin >> g >> d;
    //Stores the supply and demands of vertices (can be updated)
    supplies.push_back(g);  
    demands.push_back(d);
  }
  
  //Graph
  graph G(l);
  edge_adder adder(G);
  
  for(int j=0; j<q; j++){
    int x, y; cin >> x >> y;
    int c, d; cin >> c >> d;
    adder.add_edge(x,y,d-c); //If flow must be between c and d, then give the edge a capacity of d-c
    supplies[y] += c; //Increase by c the supply of the ending vertex
    demands[x] += c;  //Increase by c the demand of the starting vertex
    //Basically we require c more soldiers to node x, and virtually send them to y through the road by adding it c more soldiers as supply
    //So the road has at least c soldiers + a extra value between 0 and d-c
  }
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Create the edges with the updated supplies and demands
  int tot_demand = 0;
  for(int i=0; i<l; i++){
    adder.add_edge(source, i, supplies[i]);
    adder.add_edge(i, target, demands[i]);
    tot_demand += demands[i];
  }
  //Flow
  long flow = boost::push_relabel_max_flow(G, source, target);
  
  //OUTPUT
  if(flow == tot_demand){
    cout << "yes" << endl;
  }else{
    cout << "no" << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}