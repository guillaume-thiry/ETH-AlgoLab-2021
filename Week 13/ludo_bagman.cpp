// 1. Model this with a graph:
// - For the difficult matches, just link them from start to end with the according costs (no need for nodes)
// - For the 'easy' matches, have nodes representing the teams and link them with the matches (and according costs)
// - Each team also has a demand of l and a supply of l
// - So each team is represented by 2 nodes: one 'in' node having a demand of l, linked to an 'out' node having a supply of l
// - That way, we know that each team has a least l 'easy' matches but we don't affect the number of matches flowing in the graph by having an early demand
// Also have a start_bis and end_bis to ensures that exactly p matches are played

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
  int e, w, m, d; cin >> e >> w >> m >> d;
  int p, l; cin >> p >> l;
  
  //Graph
  graph G(2*e+2*w);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc source2 = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  const vertex_desc target2 = boost::add_vertex(G);
  
  //Connect East teams to the start_bis
  //Add demand of l and supply of l (using the in/out nodes)
  for(int i=0; i<e; i++){
    adder.add_edge(source2, i, p, 0);
    adder.add_edge(i, e+i, p, 0);
    adder.add_edge(source, e+i, l, 0);
    adder.add_edge(i, target, l, 0);
  }
  //Same for West teams but connected to the target instead
  for(int j=0; j<w; j++){
    adder.add_edge(2*e+j, 2*e+w+j, p, 0);
    adder.add_edge(source, 2*e+w+j, l, 0);
    adder.add_edge(2*e+j, target, l, 0);
    adder.add_edge(2*e+w+j, target2, p, 0);
  }
  //Connect East and West teams according to the matches
  for(int k=0; k<m; k++){
    int u, v, r; cin >> u >> v >> r;
    adder.add_edge(e+u, 2*e+v, 1, r);
  }
  //Add the difficult matches
  for(int k=0; k<d; k++){
    int u, v, r; cin >> u >> v >> r;
    adder.add_edge(source2, target2, 1, r);
  }
  
  adder.add_edge(source, source2, p, 0); //Start_bis is given at most p matches
  adder.add_edge(target2, target, p, 0);//Same for end_bis
  
  //Flow + Cost + OUTPUT
  long flow = boost::push_relabel_max_flow(G, source, target);
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  long cost = boost::find_flow_cost(G);
  if(flow<p+e*l+w*l){
    cout << "No schedule!" << endl;
  }else{
    cout << cost << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}