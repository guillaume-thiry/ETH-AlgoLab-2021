// 1. Model the problem as a weighted graph
// 2. Apply Dijkstra for each agent to know the distance to each shelter
// 3. Do a Binary Search to find the minimum time necessary to protect all agents:
//  - For a given time we want to test, only keep agent-shelter connections requiring less time than that
//  - Apply Edmond Maximum Matching to check if every agent can find a shelter


#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS,boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph2;

typedef boost::graph_traits<graph2>::vertex_descriptor vertex_desc;

using namespace std;

//Every agent and shelter is a node and we want to connect them together
//For a given time t, only keep agent-shelter connection taking less than t (taking into account delay d)
//Do a maximum matching to see how many agents have found a shelter in these conditions. 
int matching(vector<vector<int>>& distances, int d, int a, int s, int c, int t){
  graph2 H(a+2*s);
  
  for(int i=0; i<a; i++){
    for(int j=0; j<s; j++){
      if(distances[i][j]!=INT_MAX){//Check that a connection in a finite time is even possible
        if(distances[i][j]+d<=t){ //And that it is reachable under t (with delay d)
          boost::add_edge(i, a+j, H);
        }
        //When two places per shelter, double the shelter nodes and connect the second one to an agent if reachable under time t with TWO delays d
        //Convince yourself that this works when two agents want to access the same shelter, but one after the other
        if(c==2){
          if(distances[i][j]+2*d<=t){
            boost::add_edge(i, a+s+j, H);
          }
        }
      }
    }
  }
  //Max matching
  std::vector<vertex_desc> mate_map(a+2*s); 
  boost::edmonds_maximum_cardinality_matching(H,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, H)));
  int matching_size = boost::matching_size(H,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, H)));
  
  return matching_size;
}

void testcase(){
  //INPUT
  int n, m, a, s, c, d;
  cin >> n >> m >> a >> s >> c >> d;
  
  graph G(n);
  
  for(int i=0; i<m; i++){
    char w; cin >> w;
    int x, y, z; cin >> x >> y >> z;
    if(w=='S'){
      boost::add_edge(x,y,z,G);
    }else{
      boost::add_edge(x,y,z,G);
      boost::add_edge(y,x,z,G);
    }
  }
  vector<int> agents;
  for(int j=0; j<a; j++){
    int x; cin >> x;
    agents.push_back(x);
  }
  vector<int> shelters;
  for(int k=0; k<s; k++){
    int x; cin >> x;
    shelters.push_back(x);
  }
  
  //Dijkstra for each agent
  vector<vector<int>> distances(a, vector<int>(s, -1)); //Store each the distance between each agent and each shelter
  for(int j=0; j<a; j++){
    vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, agents[j],boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index, G))));
    for(int k=0; k<s; k++){
      distances[j][k] = dist_map[shelters[k]];

    }  
  }
  
  //Binary Search to find the minimum time necessary to protect all agents
  int i = 0; int j = INT_MAX;
  while(i!=j){
    int mid = (i+j)/2;
    int match = matching(distances, d, a, s, c, mid);//Determine the number of protected agents in time 'mid'
    
    if(match<a){
      i = mid+1;
    }else{
      j = mid;
    }
  }
  
  //OUTPUT
  cout << j << endl;
}

int main(){
  cout << fixed << setprecision(0);
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}