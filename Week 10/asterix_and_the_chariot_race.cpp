// 1. Dynamic Programming problem:
//    - At each city, decide to repair or not the streets and propagate to 'children' cities
//    - If you repaired the streets of the city, then no need to repair the streets for the next ones (but you can)
//    - If you didn't repair, either the city before has them repaired and we're good, or one of the children cities at least must carry on reparation
// Thus, 3 states are possible, and we do the DP exploration jumping from one state to another
// - State 0: you must absolutely repair the streets of the given city (easy)
// - State 1: the city before was repaired so everything is already good for the given city (although you can still try to repair it)
// - State 2: the city before was not repaired (but got managed anyway) so either you repair the streets here, or you have to do it for at least one children city
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int best(int i, int k, vector<int>& costs, vector<vector<int>>& child, vector<vector<int>>& memo){
  int n = child[i].size();
  //If we reach the end
  if(n==0){
    if(k==1){ //Either the city is already good (parent is repaired, case 1) and we do nothing
      return 0;
    }else{
      return costs[i]; //Or we have to manage it (case 0 and 2) so no other choice to repair
    }
  }
  //Memo case
  if(memo[i][k]!=-1){
    return memo[i][k];
  }
  
  int c;
  //Case 0: you must repair
  if(k==0){
    c = costs[i];
    for(int j=0; j<n; j++){
      c += best(child[i][j], 1, costs, child, memo);
    }
  //Case 1: you can repair (and then the children cities are case 1) or just pass to the children (case 2 then)
  }else if(k==1){
    int c1 = 0;
    for(int j=0; j<n; j++){
      c1 += best(child[i][j], 2, costs, child, memo); //don't repair and children are case 2
    }
    int c2 = costs[i]; //repair city i
    for(int j=0; j<n; j++){
      c2 += best(child[i][j], 1, costs, child, memo); //children are case 1
    }
    c = min(c1,c2);
  //Case 2: you can repair (and then children are case 1) or not (and then one children must be case 0, and the others are case 2)
  }else{
    int c1 = costs[i]; //repair
    for(int j=0; j<n; j++){
      c1 += best(child[i][j], 1, costs, child, memo); //children are case 1
    }
    //not repair, children are case 2, except one that is case 0
    int c2 = INT_MAX;
    int cc = 0;
    for(int j=0; j<n; j++){
      cc += best(child[i][j], 2, costs, child, memo); //all children cities at case 2
    }
    //loop to find the best child to put at case 0
    //we substract value of case 2 and add value of case 0 (that way, we don't need to recalculate the rest of the sum)
    for(int j=0; j<n; j++){
      c2 = min(c2, cc-best(child[i][j], 2, costs, child, memo)+best(child[i][j], 0, costs, child, memo));
    }
    c = min(c1,c2);
  }
  
  memo[i][k] = c;
  return c;
}

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<vector<int>> child(n);
  for(int i=0; i<n-1; i++){
    int x, y; cin >> x >> y;
    child[x].push_back(y);
  }
  vector<int> costs;
  for(int i=0; i<n; i++){
    int c; cin >> c;
    costs.push_back(c);
  }
  //DP
  vector<vector<int>> memo(n, vector<int>(3, -1));
  int res = best(0, 2, costs, child, memo);
  
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