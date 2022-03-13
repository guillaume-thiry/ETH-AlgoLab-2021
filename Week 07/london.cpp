// 1. Count the frequencies of each pair of letters (l1,l2) in the newspapers
// 2. Create a graph with start, end, a node for each pair, and a node for each letter (2 + 26*26 + 26)
// 3. Link all pairs to the start with their frequencies as weights
// 4. Link all pairs to the 2 corresponding letters (in some cases, just one letter)
// 5. Link all letters to target with the number of times they appear in the final message
// 6. Calculate the flow and see if it matches the number of characters in the message

#include <iostream>
#include <vector>
#include <string>
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
  int h, w; cin >> h >> w;
  string text; cin >> text;
  vector<int> char_text(26,0);
  //Count frequencies of characters in the final message
  for(int i=0; i<int(text.size()); i++){
    char_text[text[i]-65]++;
  }
  //Front of the letter
  vector<string> front;
  for(int j=0; j<h; j++){
    string line; cin >> line;
    front.push_back(line);
  }
  //Back of the letter
  vector<string> back;
  for(int j=0; j<h; j++){
    string line; cin >> line;
    back.push_back(line);
  }
  
  //Count the frequencies for each pair of letters
  vector<vector<int>> letters(26, vector<int>(26,0));
  for(int j=0; j<h; j++){
    for(int k=0; k<w; k++){
      int a = int(front[j][k])-65;
      int b = int(back[j][w-1-k])-65;
      letters[a][b]++;
    }
  }
  
  //Graph
  graph G(26*27);
  edge_adder adder(G);
  
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc target = boost::add_vertex(G);
  
  //Link pairs (l1,l2) with start and with corresponding letters
  for(int j=0; j<26; j++){
    for(int k=0; k<26; k++){
      adder.add_edge(source, j*26+k, letters[j][k]);
      if(j==k){
        adder.add_edge(j*26+k,26*26+j,INT_MAX); //Case where l1 = l2
      }else{
        adder.add_edge(j*26+k,26*26+j,INT_MAX);
        adder.add_edge(j*26+k,26*26+k,INT_MAX);
      }
    }
  }
  
  //Link letters to the end with correct number of appearance
  int tot = 0;
  for(int l=0; l<26; l++){
    adder.add_edge(26*26+l, target, char_text[l]);
    tot += char_text[l];
  }
  //Flow
  long flow = boost::push_relabel_max_flow(G, source, target);
  
  //OUTPUT
  if(flow == tot){
    cout << "Yes" << endl;
  }else{
    cout << "No" << endl;
  }
  
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}