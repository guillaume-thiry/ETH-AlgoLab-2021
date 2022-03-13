// 1. Calculate possible attack locations
// 2. Calculate the next overlapping spots
// 3. DP on possible attack locations using the vector next to speed up the search

#include <iostream>
#include <vector>

using namespace std;

//Begining at spot i and looking only to the right, find the best attack value
int best(int i, int m, int s, vector<int>& next, vector<pair<int,int>>& spots, vector<vector<int>>& memo){
  //Base cases
  if(m==0){ //no more attackers
    return 0;
  }
  if(i==s){ //no more spots (but still attackers idling: not good)
    return -2;
  }
  if(i==-1){ //when next available spot doesn't exist
    return -2;
  }
  //memo case
  if(memo[i][m]!=-1){
    return memo[i][m];
  }
  
  //Either you attack spot i
  int case1 = best(next[i], m-1, s, next, spots, memo); //Here the vector next is very important to speed the search (don't have to recalculate it)
  int res1;
  if(case1==-2){ //but need to check that the situation remaining still has a legal strategy
    res1 = -2;
  }else{
    res1 = case1 + spots[i].second - spots[i].first + 1;
  }
  
  //Or you just move to next spot without attacking
  int res2 = best(i+1, m, s, next, spots, memo); //can be -2 as well
  
  memo[i][m] = max(res1, res2);
  return memo[i][m];
  
}

void testcase(){
  //INPUT
  int n, m, k; cin >> n >> m >> k;
  vector<int> values;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    values.push_back(x);
  }
  
  //Sliding window to find the attack spots
  vector<pair<int,int>> spots;
  int i=0; int j=0;
  int v = values[0];
  while(i<n and j<n){
    if(v == k){ //Exactly the needed value
      spots.push_back(make_pair(i,j));
    }
    if(v<k){
      j++;
      if(j<n){
        v += values[j];
      }
    }else{
      v -= values[i];
      i++;
    }
  }
  
  // Calculate the next non overlapping spot and store it (crucial for the DP)
  int s = spots.size();
  vector<int> next(s,-1); //If cannot find any, return -1
  int a=1;
  for(int i=0; i<s; i++){
    while(a<s and spots[a].first<=spots[i].second){ //Move to the next spot until it stops overlapping with current one
      a++;
    }
    if(a<s){
      next[i] = a;
    }
  }

  //Memo init
  vector<vector<int>> memo(s, vector<int>(m+1, -1));
  
  //DP
  int res = best(0, m, s, next, spots, memo);
  //-1: not explored yet, -2: explored and failed
  
  //OUTPUT
  if(res==-2){
    cout << "fail" << endl;
  }else{
    cout << res << endl;
  }
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}