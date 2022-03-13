// 1. Sliding window

#include <vector>
#include <iostream>

using namespace std;

void testcase(){
  //INPUT
  int n, k; cin >> n >> k;
  vector<int> val;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    val.push_back(x);
  }
  
  int i=0; int j=0;
  int v = val[0];
  int best_i = 0; int best_j = 0;
  int best = abs(k-v);
  while(i<n and j<n){
    if(abs(k-v)<best){
      best = abs(k-v);
      best_i = i; best_j = j;
    }
    if(v<k){
      j++;
      if(j<n){
        v += val[j];
      }
    }else{
      v -= val[i];
      i++;
    }
  }
  //OUTPUT
  cout << best_i << ' ' << best_j << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}