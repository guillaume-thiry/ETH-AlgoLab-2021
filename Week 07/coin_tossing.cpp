// 1. Count all the existing scores of the players with known match
// 2. Represent all remaining matches with a graph:
//      - Each match is a node linked to start with capacity 1
//      - Each match is also linked to both players with capacity 1
// 3. Link players to the end with capacity the number of points still to score for them
// 4. Check if flow matches with all of that

#include <iostream>
#include <vector>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

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

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  
  graph G(n+m);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Look at the outcome of each match
  //If known, update the scores of the players
  //Else, add an edge in the graph
  vector<int> scores(n,0);
  int count = 0;
  for(int i=0; i<m; i++){
    int a, b, c; cin >> a >> b >> c;
    if(c==1){
      scores[a]++;
    }else if(c==2){
      scores[b]++;
    }else{
      //Unknown outcome
      count ++;
      adder.add_edge(source, i, 1);
      adder.add_edge(i, m+a, 1);
      adder.add_edge(i, m+b, 1);
    }
  }
  //Check if all of that is possible (even before the flow)
  bool possible = true;
  int tot_score = 0;
  for(int j=0; j<n; j++){
    int x; cin >> x;
    tot_score += x;
    //First if a player has already more points than he should, there is a problem
    if(x>=scores[j]){
      adder.add_edge(m+j, target, x-scores[j]); //Else, add the remaining number of points in graph
    }else{
      possible = false;
    }
  }
  //Also if the total number of points in the report is different from the number of matches (can happen even with a correct flow), problem
  if(tot_score!=m){
    possible = false;
  }
  
  //Flow
  long flow = boost::push_relabel_max_flow(G, source, target);

  //OUTPUT
  if(!possible){
    cout << "no" << endl;
  }else{
    if(flow==count){
      cout << "yes" << endl;
    }else{
      cout << "no" << endl;
    }
  }
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}