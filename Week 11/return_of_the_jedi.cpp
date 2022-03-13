// 1. First find Leia solution by performing the MST (she does Prim algorithm but Kruskal works as well here, also no real need to care for the priorities)
// 2. Then perform again the MST but one edge from Leia solution is forbidden
// 3. Do that for each edge and find the minimal cost

#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
#include <algorithm>

using namespace std;

typedef tuple<int, int, int> edge;

//Union Find structure to help for the MST
struct UnionFind{
  vector<int> P;
  vector<int> R;
  
  UnionFind(int n){
    for(int i=0; i<n; i++){
      P.push_back(i);
      R.push_back(0);
    }
  }
  
  int Find(int i){
    while(i!=P[i]){
      i = P[i];
    }
    return i;
  }
  
  void Union(int i, int j){
    int a = Find(i);
    int b = Find(j);
    if(a!=b){
      if(R[a]>R[b]){
        P[b] = a;
      }else{
        P[a] = b;
        R[b] = max(R[b],R[a]+1);
      }
    }
  }
};

//MST function on edges, storing the MST in res, and with the possibility to forbid edge between i and j
int MST(vector<edge>& edges, int i, int j, int n, vector<edge>& res){
  int cost = 0;
  UnionFind uf(n);
  int c = 0;
  int k = 0;
  
  //Edges are sorted from smallest to highest cost
  //Just explore the edges and add them if they do not belong yet to the same component
  //Do that until everybody is connected
  //This is just Kruskal algorithm in the end
  while(true){
    edge e = edges[k];
    int a = get<1>(e);
    int b = get<2>(e);
    if(a!=i or b!=j){ //Here we can forbid edge between i and j
      if(uf.Find(a)!=uf.Find(b)){
        uf.Union(a,b);
        cost += get<0>(e);
        res.push_back(e);
        c++;
      }  
    }
    k++;
    if(c==n-1){
      break;
    }
  }
  return cost;
}

void testcase(){
  //INPUT
  int n, tat; cin >> n >> tat;
  vector<edge> edges;
  for(int i=0; i<n; i++){
    for(int j=i+1; j<n; j++){
      int x; cin >> x;
      edge e(x, i, j);
      edges.push_back(e);
    }
  }
  
  //Sort edges, important for Kruskal
  sort(edges.begin(), edges.end());
  
  //Find Leia solution (edges of the MST stored in a vector)
  vector<edge> leia;
  MST(edges, -1, -1, n, leia);
  
  //For each one of the edges, forbid it and recompute the MST, keep track of the smallest cost
  int new_cost = INT_MAX;
  for(int i=0; i<n-1; i++){
    vector<edge> a;
    edge e = leia[i]; //One edge from Leia solution
    int c = MST(edges,get<1>(e), get<2>(e), n, a);  //That we forbid in the new MST
    new_cost = min(new_cost, c);
  }
  //OUTPUT
  cout << new_cost << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}