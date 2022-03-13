// 1. Just apply Kruskal and Dijkstra algorithm to the graph
// A weight map is useful

#include <iostream>
#include <vector>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, int>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;


using namespace std;

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  
  //Create the graph
  graph G(n);
  for(int i=0; i<m; i++){
    int x, y, c; cin >> x >> y >> c;
    boost::add_edge(x,y,c,G);
  }
  //Weight map
  auto w = boost::get(boost::edge_weight,G);
  
  //Kruskal
  vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  //Sum the weights using the weight map
  int total_w = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it){
    total_w += w[*it];
  }
  
  //Dijkstra
  vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0,boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index, G))));
  
  //Get the max distance
  int max_d = 0;
  for(int i=0; i<n; i++){
    max_d = max(max_d, dist_map[i]);
  }
  
  //OUTPUT
  cout << total_w << " " << max_d << endl;
  
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}