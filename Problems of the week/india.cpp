// 1. Model the problem as a graph with corresponding costs
// 2. Manually add an edge between the starting node and the start city (to change its capacity later)
// The capacity of this edge is the number of luggages we are willing to take in total
// 3. Do a binary search with different values of the capacity to find the answer

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef traits::vertex_descriptor vertex_desc;

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;

void testcase(){
  //INPUT
  int c, g; cin >> c >> g;
  long b; cin >> b;
  int k, a; cin >> k >> a;
  
  //Graph
  graph G(c);
  edge_adder adder(G);
  
  for(int i=0; i<g; i++){
    int x, y, d, e; cin >> x >> y >> d >> e;
    adder.add_edge(x,y,e,d);
  }
  
  //Manually add an edge between the starting node and the city Kholby
  //That way, we can track it and change its capacity
  const vertex_desc source = boost::add_vertex(G);
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto w_map = boost::get(boost::edge_weight, G);
  const edge_desc e = boost::add_edge(source, k, G).first;
  const edge_desc rev_e = boost::add_edge(k, source, G).first;
  c_map[e] = 0;
  c_map[rev_e] = 0;
  r_map[e] = rev_e;
  r_map[rev_e] = e;
  w_map[e] = 0;
  w_map[rev_e] = 0;
  
  //Binary Search with different capacity
  int i = 0;
  int j = 5000000; //cost cannot exceeds that
  int flow;
  while(i!=j){
    int mid = (i+j+1)/2;//This BS is a bit different because here we want the minimal value respecting the condition (and not the maximal one)
    c_map[e] = mid; //Change capacity here
    //Flow + Cost
    flow = boost::push_relabel_max_flow(G, source, a);
    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    long cost = boost::find_flow_cost(G);
    //Chose the half to continue
    if(cost>b){
      j = mid-1; //BS also different here
    }else{
      i = mid; //and here
    }
  }
  
  if(i==5000000){ //If that, the budget is enough to take every elephant (cost is never higher than budget)
    cout << flow << endl; //And so the answer is just the flow
  }else{
    cout << i << endl; //Else this is the maximum number of luggages
  }
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}