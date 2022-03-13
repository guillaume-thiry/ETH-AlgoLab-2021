// 1. Model the problem with a graph:
//  - A first row of nodes corresponds to meals proposed by the canteen for each day
//  - A second row of nodes corresponds to the student for each day
//  - Meals are linked to the start (daily production) and together (freezer) with corresponding capacities and costs
//  - For each day, the node 'meals' is linked to the node 'students'
//  - The nodes students are linked to the end with corresponding demands
// Some 'costs' are negative because they are revenues. These ones are shifted to always have positive costs (and we shift back at the end) 

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,boost::property<boost::edge_capacity_t, long,boost::property<boost::edge_residual_capacity_t, long,boost::property<boost::edge_reverse_t, traits::edge_descriptor,boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor   edge_desc;

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
  graph G(n);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Daily production of meals
  for(int i=0; i<n; i++){
    int a, c; cin >> a >> c;
    adder.add_edge(source, i, a, c); //real cost
  }
  //Daily demands from students
  int students = 0;
  for(int i=0; i<n; i++){
    int s, p; cin >> s >> p;
    adder.add_edge(i, target, s, 20-p); //actual revenue
    students += s;
  }
  //Freezing some meals for next day
  for(int i=0; i<n-1; i++){
    int v, e; cin >> v >> e;
    adder.add_edge(i, i+1, v, e);
  }
  
  //Flow + Cost
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  
  int cost = boost::find_flow_cost(G);
  int flow = boost::push_relabel_max_flow(G, source, target);
  
  //Actual revenue at the end
  int res = -cost + 20*flow; //shift back
  
  //OUTPUT
  if(flow==students){
    cout << "possible " << flow << ' ' << res << endl;
  }else{
    cout << "impossible " << flow << ' ' << res << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}