// 1. DP

#include <iostream>
#include <vector>

using namespace std;

//DP function to return my gain given a certain coin configuration
int best(int i, int j, bool player, vector<int>& coins, vector<vector<int>>& memo){
  //Base case
  if(i==j){
    if(player){ //Last coin for me
      return coins[i];
    }else{  //Not for me
      return 0;
    }
  }
  //Memo case
  if(memo[i][j]!=-1){
    return memo[i][j];
  }
  
  //Either player takes the leftmost or rightmost coin
  int cas1 = best(i+1, j, !player, coins, memo);
  int cas2 = best(i, j-1, !player, coins, memo);
  
  int res;
  if(player){ //If I play, I want to maximize my gain
    res = max(cas1+coins[i], cas2+coins[j]);
  }else{ //If my friend plays, he wants to minimize my gain (in the worst case) 
    res = min(cas1, cas2);
  }
  
  memo[i][j] = res;
  return res;
}

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<int> coins;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    coins.push_back(x);
  }
  
  //Memo init
  vector<vector<int>> memo(n, vector<int>(n,-1));
  
  //DP
  int res = best(0, n-1, true, coins, memo);
  
  //OUTPUT
  cout << res << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}