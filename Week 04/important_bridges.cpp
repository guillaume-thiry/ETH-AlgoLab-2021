// 1. Convince yourself that the result corresponds to the number of biconnected components containing only one edge
// 2. Compute all the biconnected components of the graph
// 3. Only keep the ones of size 1 and add the corresponding edges to a vector
// 4. Sort the vector at the end

#include <iostream>
#include <vector>

#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

//See the biconnected_components example file on the BGL documentation
namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<boost::edge_component_t, int>> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

using namespace std;

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  
  //Graph
  graph G(n);
  for(int i=0; i<m; i++){
    int x, y; cin >> x >> y;
    boost::add_edge(x,y,G);
  }
  
  //Biconnected_components
  boost::property_map<graph, boost::edge_component_t >::type comp = get(boost::edge_component, G);
  int num_comp = biconnected_components(G, comp);
  
  vector<int> card(num_comp,0); //Store the cardinality of each component
  boost::graph_traits<graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
    card[comp[*ei]]++;
  }
  
  // Store the edges for the components of size 1
  vector<pair<int,int>> res;
  int c = 0;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei){
    if(card[comp[*ei]]==1){
      int a = min(source(*ei, G), target(*ei, G)); //Sort the endpoints of the edge here
      int b = max(source(*ei, G), target(*ei, G)); //And here
      res.push_back(make_pair(a,b));
      c++;
    }
  }
  
  //Final sorting and OUTPUT
  cout << c << endl;
  sort(res.begin(), res.end());
  for(int i=0; i<int(res.size()); i++){
    cout << res[i].first << ' ' << res[i].second << endl;
  }

}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}