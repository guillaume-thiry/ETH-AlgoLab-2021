// 1. Dynamic Programming
#include <iostream>
#include <vector>

using namespace std;

//i and j are the first and last coins remaining, p is the current player, k is the player of interest
int best(int i, int j, vector<int>& values, vector<vector<int>>& memo, int p, int k, int n){
  if(i==j){ //last coin
    if(p==k){
      return values[i];
    }else{
      return 0;
    }
  }
  //Memo case
  if(memo[i][j] != -1){
    return memo[i][j];
  }
  
  int new_p = (p+1)%n; //modulo n
  int choice1 = best(i+1, j, values, memo, new_p, k, n);
  int choice2 = best(i, j-1, values, memo, new_p, k, n);
  
  int res;
  if(p==k){
    res = max(values[i]+choice1, values[j]+choice2);
  }else{
    res = min(choice1, choice2);
  }
  
  memo[i][j] = res;
  return res;
}

void testcase(){
  //INPUT
  int n, m, k; cin >> n >> m >> k;
  vector<int> values;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    values.push_back(x);
  }
  
  //Memo
  vector<vector<int>> memo (n, vector<int> (n, -1));
  //DP
  int res = best(0, n-1, values, memo, 0, k, m);
  
  //OUTPUT
  cout << res << endl;
  
}


int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}