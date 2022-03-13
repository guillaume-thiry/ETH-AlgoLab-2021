// 1. First do a sliding window on each waterway (without Pyke) and update max_islands
// 2. Then we include Pyke and try to join two waterways (one possibly with no islands):
//    - Explore the waterways in the order, and progressively add more islands
//    - Calculate the remaining number of men and look in a vector if a solution from a previous waterway exist (and update max_islands)
//    - Update the vector with the islands from this new waterway and continue

#include <iostream>
#include <vector>

using namespace std;

void testcase(){
  //INPUT
  int n, k, w; cin >> n >> k >> w;
  vector<int> costs;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    costs.push_back(x);
  }
  vector<vector<int>> waterways;
  for(int j=0; j<w; j++){
    int l; cin >> l;
    vector<int> wat;
    for(int k=0; k<l; k++){
      int x; cin >> x;
      wat.push_back(x);
    }
    waterways.push_back(wat);
  }
  
  //Sliding window on each waterway
  int max_islands = 0;
  for(int j=0; j<w; j++){
    int a = 1; int b = 1;
    int nw = waterways[j].size();
    int val = costs[waterways[j][1]];
    while(a<nw and b<nw){
      if(val==k){
        max_islands = max(max_islands, b-a+1);
      }
      if(val<k){
        b++;
        if(b<nw){
          val+= costs[waterways[j][b]];
        }
      }else{
        val -= costs[waterways[j][a]];
        a++;
      }
    }
  }
  
  //Now we always include Pyke so we have a new value for k
  int new_k = k-costs[0];
  if(new_k == 0){
    max_islands = max(max_islands, 1);
  }
  if(new_k>0){
    vector<int> max_other(new_k+1,-1); //Here we store the solution for the previous waterways (max number of islands in a previous waterwat for a given number of men remaining)
    max_other[0] = 0;
    for(int j=0; j<w; j++){//For each waterway
      int val = 0;
      int nw = waterways[j].size();
      vector<pair<int,int>> new_islands;
      for(int i=1; i<nw; i++){  //Progressively add more islands (in the order)
        val += costs[waterways[j][i]];
        if(val<=new_k){ //Some men are remaining
          if(max_other[new_k-val]!=-1){ //Try to find islands in a previous waterway exactly corresponding to this number
            max_islands = max(max_islands, i+1+max_other[new_k-val]); //And update the number of islands
          }
          new_islands.push_back(make_pair(val,i));//Add this combination of islands to a temporary vector
        }
      }
      for(int x=0; x<int(new_islands.size()); x++){
        max_other[new_islands[x].first] = max(max_other[new_islands[x].first], new_islands[x].second); //And update the max_other vector at the end of the waterway
        //Do not do it before because a waterway could interfere with itself
      }
    }
  }
  //OUTPUT
  cout << max_islands << endl;
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}