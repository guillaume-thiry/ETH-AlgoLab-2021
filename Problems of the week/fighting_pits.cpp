// 1. Encode the information on North and South entrance:
// For a memory m, you only need to remember m-1 values because one will be replaced anyway
// So for maximum value of m = 3, we should encode a list of 2 elements
// Each element can be any fighter type (4) or unknown (no figher yet) so 5 values
// So the North and South entrance have at most 25 configuration that we encode as integers
// 2. Use that encoding for a Dynamic Programming algorithm (and update the memory lists)

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

//Encode memory list of fighters a
int encode(vector<int> a){
  return a[0]+5*a[1];
}

//Update list of fighters a with new fighter x given memory m
vector<int> update(vector<int> a, int m, int x){
  if(m==2){
    a[0] = x;
  }else{
    a[0] = a[1];
    a[1] = x;
  }
  return a;
}

//Give the distinct number of fighter types in list a (+ new value x) given memory m
int distinct(vector<int> a, int m, int x){
  int res = 0;
  if(m==2){
    if(a[0]!=4){
      res++;
    }
    if(a[0]!=x){
      res++;
    }
  }else{
    if(a[0]!=4){
      res++;
    }
    if(a[1]!=4 and a[1]!=a[0]){
      res ++;
    }
    if(x!=a[1] and x!=a[0]){
      res ++;
    }
  }
  return res;
}


int best(int i, int a, int b, int diff, int n, int m, vector<vector<int>>& decode, vector<int>& fighters, vector<vector<vector<vector<int>>>>& memo){
  //No more fighters
  if(i==n){
    return 0;
  }
  //Difference too big (automatic negative excitement)
  if(abs(diff)>11){
    return -2;
  }
  //Memo case
  if(memo[i][a][b][diff+11]!=-1){
    return memo[i][a][b][diff+11];
  }
  
  //Decode the memory list
  vector<int> file1 = decode[a];
  vector<int> file2 = decode[b];
  int x = fighters[i]; //new fighter
  
  //Case 1: new fighter takes first entrance
  int cas1 = best(i+1, encode(update(file1, m, x)), b, diff+1, n, m, decode, fighters, memo); //Update memory list for entrance 1 and increase the difference (in relative value)
  int next1 = 1000*distinct(file1, m, x) - pow(2, abs(diff+1)); //use absolute value of the difference as penalty
  int res1;
  if(cas1!=-2 and next1>=0){  //Check that the current and future excitement follows the non-negative rule
    res1 = cas1 + next1;
  }else{
    res1 = -2;
  }

  //Case 2: new fighter takes the second entrance
  int cas2 = best(i+1, a, encode(update(file2, m, x)), diff-1, n, m, decode, fighters, memo); //update list 2 and decrease the difference (relative value)
  int next2 = 1000*distinct(file2, m, x) - pow(2, abs(diff-1)); 
  int res2;
  if(cas2!=-2 and next2>=0){
    res2 = cas2 + next2;
  }else{
    res2 = -2;
  }
  
  //Because diff can be negative (between -11 and +11), shift it for the indexing
  memo[i][a][b][diff+11] = max(res1, res2);
  return max(res1, res2);
}

void testcase(){
  //INPUT
  int n, k, m; cin >> n >> k >> m;
  vector<int> fighters;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    fighters.push_back(x);
  }
  //Create a vector decode to get back the memory list given the coded integer
  //Enumerate all combinations, code them, and store the reverse relationship in the vector
  vector<vector<int>> decode(25);
  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      vector<int> a = {i,j};
      decode[encode(a)] = a;
    }
  }
  //At start the memory list is 2 fighters of type 4 (meaning there is no fighter)
  vector<int> start = {4,4};
  int x = encode(start);
  
  //Memo indexed by: current fighter in the list (n), memory list for North (25), memory list for South (25), difference N/S (23)
  vector<vector<vector<vector<int>>>> memo(n, vector<vector<vector<int>>>(25, vector<vector<int>>(25, vector<int>(23,-1))));
  //DP
  int res = best(0, x, x, 0, n, m, decode, fighters, memo);
  
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