// 1. Create a graph corresponding to the problems, linking the nodes with the costs as capacity values
// 2. Choosing different start and end points, compute different min splits with the flows and take the smallest one

#include <iostream>
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
  int n, m; cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);
  
  for(int j=0; j<m; j++){
    int x, y, c; cin >> x >> y >> c;
    adder.add_edge(x,y,c);
  }
  
  // Configuration to check: 
  // - I have node 0 and the other have node i (i = 1 to n-1)
  // - I have node i (i = 1 to n-1) and the other have node 0
  // For each configuration, compute the min cost split (with flow)
  int min_cut = INT_MAX; //Keep track of split of minimal cost among all
  for(int i=1; i<n; i++){
    int flow1 = boost::push_relabel_max_flow(G, 0, i);
    int flow2 = boost::push_relabel_max_flow(G, i, 0);
    min_cut = min(min_cut, flow1);
    min_cut = min(min_cut, flow2);
  }
  
  //OUTPUT
  cout << min_cut << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}