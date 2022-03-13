// 1. DP to find minimal number of winning moves for each meeple
// 2. Find the winner depending on these numbers and the playing order

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int best(int i, vector<vector<int>>& transi, vector<vector<int>>& memo, int n);
int worst(int i, vector<vector<int>>& transi, vector<vector<int>>& memo, int n);

// Given meeple at position i, number of moves till the end if you play to win and the other play the worst moves
int best(int i, vector<vector<int>>& transi, vector<vector<int>>& memo, int n){
  //Base case (finish)
  if(i==n){
    return 0;
  }
  //Memo case
  if(memo[i][0]!=-1){
    return memo[i][0];
  }
  
  //Find transition giving the minimum number of moves, accounting for the next player playing the worst moves
  int mini = INT_MAX;
  vector<int> next = transi[i];
  for(int k=0; k<int(next.size()); k++){
    int j = next[k];
    mini = min(mini, worst(j, transi, memo, n));
  }
  memo[i][0] = mini+1;
  return mini+1;
}

// Given meeple at position i, number of moves till the end if you play the worst moves and the other the best moves
int worst(int i, vector<vector<int>>& transi, vector<vector<int>>& memo, int n){
  //Base case (finish)
  if(i==n){
    return 0;
  }
  //Memo case
  if(memo[i][1]!=-1){
    return memo[i][1];
  }
  
  //Find the longest way till the end, accounting for the next player playing perfectly to win
  int maxi = 0;
  vector<int> next = transi[i];
  for(int k=0; k<int(next.size()); k++){
    int j = next[k];
    maxi = max(maxi, best(j, transi, memo, n));
  }
  memo[i][1] = maxi+1;
  return maxi+1;
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m ;
  int r, b; cin >> r >> b;
  vector<vector<int>> transi(n+1);
  for(int i=0; i<m; i++){
    int u, v; cin >> u >> v;
    transi[u].push_back(v);
  }
  
  //Memo init
  vector<vector<int>> memo(n+1, vector<int>(2,-1));
  
  //Smallest number of moves for Sherlock (red meeple) and Moriarty (black meeple) to win
  //Each player plays its meeple first
  int sher = best(r, transi, memo, n);
  int mor = best(b, transi, memo, n);
  
  //OUTPUT
  //Depending on number of winning moves for each meeple and the playing order, find the winner
  if(sher<mor){
    cout << 0 << endl;
  }else if(mor<sher){
    cout << 1 << endl;
  }else{
    if(mor%2 == 0){
      cout << 1 << endl;
    }else{
      cout << 0 << endl;
    }
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}