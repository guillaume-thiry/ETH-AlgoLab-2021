// 1. Model this as a graph, but reverse all the directions to only use Dijkstra once (on the terminal node)
// 2. Compute strongly connected components for the teleportation network
// 3. For each component, add a new hub node to simulate the teleportation network
// Going to that node costs |component|-1 and leaving costs 0
// 4. Compute Dijkstra starting at the terminal node

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<
boost::vecS,boost::vecS,boost::directedS,boost::no_property,boost::property<boost::edge_weight_t, int>> graph;

using namespace std;

void testcase(){
  //INPUT
  int n, m, k, t; cin >> n >> m >> k >> t;
  vector<int> teleport(n,0);
  for(int i=0; i<t; i++){
    int x; cin >> x;
    teleport[x] = 1;
  }
  //Graph
  graph G(n);
  for(int i=0; i<m; i++){
    int u, v, c; cin >> u >> v >> c;
    boost::add_edge(v,u,c, G); //reverse directions here
  }
  
  //Strongly connected components
  vector<int> scc_map(n);
  int nscc = boost::strong_components(G,boost::make_iterator_property_map(scc_map.begin(),boost::get(boost::vertex_index, G)));
  //Compute the cardinal of each component
  vector<int> components_card(nscc,0);
  for(int i=0; i<n; i++){
    if(teleport[i]==1){
      components_card[scc_map[i]]++;
    }
  }
  //Add hub node for each component with according cost (given by the cardinal of the component)
  for(int i=0; i<n; i++){
    if(teleport[i]==1){ //first check that this node is in the network
      int comp = scc_map[i];
      int cost = components_card[comp]-1;
      boost::add_edge(i, n+comp, cost, G);  //this automatically creates a node if don't already exist
      boost::add_edge(n+comp, i, 0, G);
    }
  }
  
  //Dijkstra
  vector<int> dist_map(n+nscc);
  boost::dijkstra_shortest_paths(G, n-1,boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index, G))));
  
  //Check if one warehouse is close enough
  bool found = false;
  int min_path = 1000001;
  for(int j=0; j<k; j++){
    if (dist_map[j]<min_path){
      found = true;
      min_path = dist_map[j];
    }
  }
  //OUTPUT
  if(found){
    cout << min_path << endl;
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