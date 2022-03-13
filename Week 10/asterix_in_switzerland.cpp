// 1. Formulate the problem as a graph:
//  - Each province is a node, and they are linked together by edges depecting their debts to each other 
//  - All provinces with positive balance are linked to the start with their balance as capacity
//  - All provinces with negative balance are linked to the end
// The initial group is the one composed of all provinces with positive balance, but it can be changed:
// 2. Find the min cut of this graph using the flow
// The cost represents what the initial group will lose in the cutting process:
// - Because they owe some money to another province not in the cut
// - Because they have a negative balance (province cut from the end and added in the group)
// - Or because a province with money is excluded from the cut (province cut from the start, excluded from the group)
// 3. Compare this cost to what the group has at first (when only composed of provinces with positive balances)

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
  int n, m; cin >> n >> m;
  long balance_pos = 0;
  
  graph G(n);
  edge_adder adder(G);
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Link each province to the start or the end, depending on their balance
  for(int i=0; i<n; i++){
    int x; cin >> x;
    if(x>0){
      balance_pos += x;
      adder.add_edge(source, i, x);
    }else if(x<0){
      adder.add_edge(i, target, -x);
    }
  }
  
  //Linked endebted provinces together
  for(int j=0; j<m; j++){
    int a, b, x; cin >> a >> b >> x;
    adder.add_edge(a,b,x);
  }
  
  //Flow and OUTPUT
  long flow = boost::push_relabel_max_flow(G, source, target);
  if(balance_pos-flow>0){
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