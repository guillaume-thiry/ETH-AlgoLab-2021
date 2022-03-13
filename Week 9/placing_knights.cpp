// 1. Model the problem with a graph:
//  - Because knights on white square can only move to black squares, the graph is bipartite
//  - Link the squares together if a knigh can move from one to the other
// 2. Compute the Maximum Independent Set of the graph thanks to the flow (König theorem, see lecture)

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
  int n; cin >> n;
  vector<vector<int>> holes(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      int x; cin >> x;
      holes[i].push_back(x);
    }
  }
  
  graph G(n*n);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Link together squares pair-wise reachable by a knight
  vector<int> di = {-1,-1,1,1,-2,-2,2,2}; //All 8 targets of a knight
  vector<int> dj = {-2,2,-2,2,-1,1,-1,1};
  
  int tot = 0; //Number of non-holes squares = Number of nodes in the graph (without start and end)
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      if(holes[i][j]){
        tot++;
        if((i+j)%2==0){
          adder.add_edge(source, i*n+j, 1); //Link black (or white) squares to the start
          for(int k=0; k<8; k++){
            int new_i = i+di[k];
            int new_j = j+dj[k];
            if(new_i>=0 and new_i<n){
              if(new_j>=0 and new_j<n){
                if(holes[new_i][new_j]){
                  adder.add_edge(i*n+j, new_i*n+new_j,1);
                }
              }
            }
          }
        }else{
          adder.add_edge(i*n+j, target, 1); //Link the other color to the end (make a bipartite graph)
        }
      }
    }
  }
  //Flow
  long flow = boost::push_relabel_max_flow(G, source, target);
  
  //OUTPUT
  cout << tot-flow << endl; //See König theorem
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}