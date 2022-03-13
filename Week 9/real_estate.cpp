// 1. Solve that with a graph with capacity and cost
//  - Each buyer is linked to start with capacity 1 (only one buy)
//  - Each buyer is linked to each property with cost being the negative value he wants to pay for that (because we can only minimize cost)
//  - Each property is linked to a node for the state it belongs to
//  - Each state is linked to the end with capacity being the max number of sales for the state
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;
typedef traits::vertex_descriptor vertex_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

using namespace std;

void testcase(){
  //INPUT
  int n, m, s; cin >> n >> m >> s;
  vector<int> capacity;
  for(int k=0; k<s; k++){
    int x; cin >> x;
    capacity.push_back(x);
  }
  vector<int> states;
  for(int j=0; j<m; j++){
    int x; cin >> x;
    states.push_back(x-1);
  }
  
  //Graph
  graph G(n+m+s);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Link buyers to the start and to the properties (with negative prices as cost)
  for(int i=0; i<n; i++){
    adder.add_edge(source, i, 1, 0);
    for(int j=0; j<m; j++){
      int x; cin >> x;
      adder.add_edge(i, n+j, 1, 100-x); //Here actually we shift the negative price (-x) by 100 to still get a positive value
      // That way we can still apply 'successive_shortest_path_nonnegative_weights' and we can find the real cost by doing the reverse shift at the end
    }
  }
  //Link properties to states
  for(int j=0; j<m; j++){
    adder.add_edge(n+j,n+m+states[j],1,0);
  }
  //Link states to end with max sales as capacity
  for(int k=0; k<s; k++){
    adder.add_edge(n+m+k, target, capacity[k], 0);
  }
  //Flow + Cost
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  int cost = boost::find_flow_cost(G);
  int flow = boost::push_relabel_max_flow(G, source, target);
  
  //OUTPUT
  cout << flow << ' ' << 100*flow-cost << endl; //Shift back the cost by 100 (times the number of nodes with this shift, so the flow)
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}