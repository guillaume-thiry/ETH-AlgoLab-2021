// 1. For each species, construct a graph and apply Kruskal
// 2. On all the edges given by the MST, update the weights
// 3. Create a final graph with the min weights from all the species
// 4. The result is then simply given by Dijkstra

// Note: To retrieve the id of an edge after the Kruskal can be challenging
// Many solutions exist, we decided to implement a 'reverse' datastructure to get the id of an edge given its endpoints
// Another solution is to store the min weights not with the id but with the endpoints (min_weight[x][y])
// And to get the weight of an edge in the MST loop using a weight map (boost::property_map<weighted_graph, boost::edge_weight_t>::type)
// That way, no need to index the edges at all

#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

using namespace std;

void testcase(){
  //INPUT
  int n, e, s, a, b;
  cin >> n >> e >> s >> a >> b;
  vector<vector<int>> reverse(n, vector<int> (n, -1)); //Matrix storing the reverse information as 'edges'
  vector<pair<int,int>> edges;
  vector<vector<int>> times(e);
  for(int i=0; i<e; i++){
    int x, y; cin >> x >> y;
    edges.push_back(make_pair(x,y));
    reverse[x][y] = i; reverse[x][y] = i; //Build the reverse matrix
    for(int j=0; j<s; j++){
      int w; cin >> w;
      times[i].push_back(w);
    }
  }
  for(int j=0; j<s; j++){ //Not necessary actually
    int h; cin >> h;
  }
  
  
  vector<int> min_weights(e, -1); //Store the min weights for each edge
   
  for(int j=0; j<s; j++){
    //For each species, create a graph with the right weights
    weighted_graph G(n);
    for(int i=0; i<e; i++){
      boost::add_edge(edges[i].first, edges[i].second, times[i][j], G);
    }
    
    //Apply Kruskal MST to find the edges taken by this species
    vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    //Go through all the edges, and udpate the min weights
    for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it){
      int a = boost::source(*it, G);
      int b = boost::target(*it, G);
      
      int id = reverse[a][b]; //Here we use the 'reverse' datastructure to get the id of the edge
      //Update the weights
      if(min_weights[id]==-1){
        min_weights[id] = times[id][j];
      }else{
        min_weights[id] = min(min_weights[id],times[id][j]);
      }
    }
  }
  
  //Create a new graph with the minimal weights for each edges (if any)
  weighted_graph G(n);
  for(int i=0; i<e; i++){
    if(min_weights[i]!=-1){
      boost::add_edge(edges[i].first, edges[i].second, min_weights[i], G);
    }
  }
  
  //Just Dijkstra
  vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, a,boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index, G))));
  
  //OUTPUT
  cout << dist_map[b] << endl;
  
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}