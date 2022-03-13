// 1. Dynamic Programming on the beverage list: at each beverage and for each remaining volume, stores:
//  - The remaining min cost for this volume
//  - The max number of beverages to use for that
//  - The information of whether or not the current beverage has been used in this remaining part (to not count it multiple times)

#include <iostream>
#include <vector>
#include <tuple>
#include <climits>

using namespace std;

typedef tuple<int,int,bool> elt;

//DP function
elt best(int i, int k, int n, vector<int>& costs, vector<int>& volumes, vector<vector<elt>>& memo){
  // No volume left to buy, so 0 cost and 0 beverage used and we don't use i
  if(k<=0){
    elt res(0,0,false);
    return res;
  }
  //If all beverages explored but still volume left, then problem
  if(i==n){
    elt res(INT_MAX,0,false);
    return res;
  }
  //Memo case
  if(get<0>(memo[i][k])!=-1){
    return memo[i][k];
  }
  
  //Case 1: use beverage i
  elt case1 = best(i, k-volumes[i], n, costs, volumes, memo);
  int cost1 = get<0>(case1) + costs[i];
  int drink1 = get<1>(case1) + !get<2>(case1); //Make sure to not count it twice if beverage i is still used later on
  elt res1(cost1, drink1, true);
  
  //Case 2: move to next beverage without buying anything
  elt case2 = best(i+1, k, n, costs, volumes, memo);
  int cost2 = get<0>(case2);
  int drink2 = get<1>(case2);
  elt res2(cost2, drink2, false);
  
  //Choose the best case
  elt res;
  if(cost1<cost2){
    res = res1;
  }else if(cost1>cost2){
    res = res2;
  }else{
    if(drink1>drink2){
      res = res1;
    }else{
      res = res2;
    }
  }
  
  memo[i][k] = res;
  return res;
}

void testcase(){
  //INPUT
  int n, k; cin >> n >> k;
  vector<int> costs;
  vector<int> volumes;
  for(int i=0; i<n; i++){
    int c, v; cin >> c >> v;
    costs.push_back(c);
    volumes.push_back(v);
  }
  //Memo
  vector<vector<elt>> memo(n, vector<elt>(k+1, {-1,-1,false}));
  //DP
  elt res = best(0, k, n, costs, volumes, memo);
  
  //OUTPUT
  cout << get<0>(res) << ' ' << get<1>(res) << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}