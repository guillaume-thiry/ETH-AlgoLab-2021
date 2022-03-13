// 1. Create a graph with 2 nodes for every square in the grid (one in and one out)
// 2. Link the in and out nodes together between neighbors 
// 3  Link in and out nodes of same square together with cost being the negative number of coins received
// 4. Set a start and an end with capacity 2 (because we want two paths)
// 5. Calculate the final cost
// This problem of back and forth path is just finding 2 paths from start to end that maximimize the coins (minimize the cost)

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

typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;

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
  int n; cin >> n;
  vector<vector<int>> coins(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      int x; cin >> x;
      coins[i].push_back(x);
    }
  }
  
  //Graph
  graph G(2*n*n);
  edge_adder adder(G);
  
  //Edges
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      if(j!=n-1){
        adder.add_edge(n*n+i*n+j, n*i+j+1, 1, 0); //Link to node at its left
      }
      if(i!=n-1){
        adder.add_edge(n*n+i*n+j, n*i+j+n, 1, 0); //Link to node below it
      }
      adder.add_edge(n*i+j, n*n+n*i+j, 1, 100-coins[i][j]); //Link in and out of same square together, shift the cost to keep it positive
    }
  }
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  adder.add_edge(source, n*n, 2, 0);
  adder.add_edge(n*n-1, target, 2, 0);
  
  //Final cost + OUTPUT
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  int cost = boost::find_flow_cost(G);
  cout << -cost+200*(2*n-3)+coins[0][0]+coins[n-1][n-1] << endl; //Shift back the cost
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}