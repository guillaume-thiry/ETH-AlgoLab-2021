// 1. For each integer k, determines the max power you can have with k potions of type A while meeting the requirement on hapiness (if even possible)
// This is done by Dynamic Programming
// 2. Sort the potions of type B from higher to lower levels of wit
// 3. For each integer k, checks if a whole solution is possible with k potions of type A:
//      - As you know k, you know how much total wit Harry needs (initial value + k maluses)
//      - This is easily converted in a minimal number of potions of type B (greedily taking the strongest first until requirement on wit is met)
//      - Now you know how much power is needed (initial value + maluses)
//      - Checks if the maximal value of power with k potions of type A is enough with this new need
//      - Do that for increasing k until finding a solution (or you run out of potions)
//      - All of that is done under the assumption that requirement on hapiness is met already


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool comp(int a, int b){
  return a>b;
}

//DP to calculate max P (under condition on H) for a given number of potions
long best(int i, int k, int h, int n, vector<pair<int,int>>& pot, vector<vector<vector<long>>>& memo){
  //i = index of the current potions A discussed, k = remaining number of potions to use, h = remaining quantity of H to find
  if(k==0){
    if(h==0){
      return 0;
    }else{
      return -2; //-2 means that this combination does not meet criteria on H
    }
  }
  if(i==n){
    return -2;
  }
  if(memo[i][k][h]!=-1){
    return memo[i][k][h];
  }
  
  long res1 = best(i+1, k, h, n, pot, memo); //Not take potions i
  
  long next2 = best(i+1, k-1, max(0,h-pot[i].second), n, pot, memo); //Take potions i
  long res2;
  if(next2==-2){
    res2 = -2;
  }else{
    res2 = next2 + pot[i].first;
  }
  
  memo[i][k][h] = max(res1,res2);
  return max(res1, res2);
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long a, b; cin >> a >> b;
  long P, H, W; cin >> P >> H >> W;
  vector<pair<int,int>> potions_A;
  for(int i=0; i<n; i++){
    int p, h; cin >> p >> h;
    potions_A.push_back(make_pair(p,h));
  }
  vector<int> potions_B;
  for(int j=0; j<m; j++){
    int w; cin >> w;
    potions_B.push_back(w);
  }
  
  //Dynamic Programming
  vector<long> max_P(n+1); //Store here the max P (under requirement on H) for each number of potions k
  vector<vector<vector<long>>> memo(n, vector<vector<long>> (n+1, vector<long>(H+1,-1)));
  
  for(int i=0; i<n+1; i++){
    max_P[i] = best(0, i, H, n, potions_A, memo);
  }
  
  //Sort potions B and compute cumulative wit values (take the strongest potions B first to minimize their number)
  sort(potions_B.begin(), potions_B.end(), comp);
  long val = 0;
  vector<long> cum_B = {0};
  for(int j=0; j<m; j++){
    val += potions_B[j];
    cum_B.push_back(val);
  }
  
  //Final loop to find the minimal number of potions so that everything is met
  int res = -1;
  for(int i=0; i<n+1; i++){
    if(max_P[i]!=-2 and max_P[i]>=P){
      long j=0;
      while(j<m and cum_B[j]<W+i*a){ //Add potions B until requirement on W is met
        j++;
      }
      if(cum_B[j]>=W+i*a and max_P[i]>=P+b*j){  //Checks requirement on P
        res = j+i;
        break;
      }
    }
  }
  
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