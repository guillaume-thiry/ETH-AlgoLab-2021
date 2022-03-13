// 1. For each pair of person, compute the number of common characteristics (sort them for more efficiency)
// 2. Create a graph with only edges between people having a similarity stricly larger than the solution f
// 3. Compute the Maximum Matching with Edmond algorithm
// 4. If all the edges are still in the matching, a solution stricly better than f exist, else f is the best you can have

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

//Simple function to compute the number of common elements in two sorted lists
int common(vector<string>& a, vector<string>& b, int c){
  int count = 0;
  int i=0; int j=0;
  while(true){
    if(a[i]==b[j]){
      count ++;
      i++; j++;
    }else if(a[i]>b[j]){
      j++;
    }else{
      i++;
    }
    if((i==c) or (j==c)){
      break;
    }
  }
  return count;
}

void testcase(){
  //INPUT
  int n, c, f; cin >> n >> c >> f;
  vector<vector<string>> charac(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<c; j++){
      string x; cin >> x;
      charac[i].push_back(x);
    }
  }
  //Sort the characteristics for a more efficient comparison
  for(int i=0; i<n; i++){
    sort(charac[i].begin(), charac[i].end());
  }
  
  //Create the graph with edges of strictly larger weights than f
  graph G(n);
  for(int i=0; i<n; i++){
    for(int j=i+1; j<n; j++){
      if(common(charac[i], charac[j],c)>f){
        boost::add_edge(i,j,G);
      }
    }
  }
  
  //Edmond algorithm
  vector<vertex_desc> mate_map(n);

  boost::edmonds_maximum_cardinality_matching(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
  
  //OUTPUT
  if(matching_size==n/2){
    cout << "not optimal" << endl;
  }else{
    cout << "optimal" << endl;
  }
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}