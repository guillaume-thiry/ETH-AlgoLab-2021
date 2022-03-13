// 1. Using Dynamic Programming, determine the best score possible in k moves
// 2. If score still lower than x, it's impossible
// 3. Else, do a linear search (changing the number of allowed moves) to find the minimal number of moves required to score x or more

#include <iostream>
#include <vector>

using namespace std;

long score(vector<vector<pair<int, long>>>& canals, int i, int k, vector<vector<long>>& memo){
  //No more moves
  if(k==0){
    return 0;
  }
  //Memo case
  if(memo[i][k]!=-1){
    return memo[i][k];
  }
  
  vector<pair<int,long>> next = canals[i];
  int n = next.size();
  long res;
  
  if(n==0){ //If no outgoing canal, go back to the start with no move consumed
    res = score(canals, 0, k, memo);
  }else{
    //Else look at all the outgoing canals and choose the best one
    long max_val = 0;
    for(int j=0; j<n; j++){
      max_val = max(max_val, next[j].second + score(canals, next[j].first, k-1, memo));
    }
    res = max_val;
  }
  
  memo[i][k] = res;
  return res;
  
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long x; cin >> x;
  int k; cin >> k;
  vector<vector<pair<int, long>>> canals(n);
  for(int i=0; i<m; i++){
    int u, v; cin >> u >> v;
    long p; cin >> p;
    canals[u].push_back(make_pair(v,p)); 
  }
  
  //Memo vector
  vector<vector<long>> memo(n, vector<long>(k+1, -1));
  //DP: calculate the max score possible, starting at 0, in k moves
  long best = score(canals, 0, k, memo);
  //OUTPUT
  if(best<x){
    cout << "Impossible" << endl; //Score x not possible with k moves
  }else{
    int i=1;
    while(true){
      if(score(canals, 0, i, memo)<x){  //Linear search of the minimal number of moves
        i++;
      }else{
        break;
      }
    }
    cout << i << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}