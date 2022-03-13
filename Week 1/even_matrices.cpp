// 1. For each column of the matrix, compute the parity partial sum (0s and 1s to give the parity of the partial sum for each column)
// 2. For each pair of rows j1 and j2, compute the parity sum between row j1 and row j2 column-wise using P (done in constant time and not linear)
// 3. For each v, compute the number of even pairs in linear time as seen before
// This gives the number of even matrices

#include <iostream>
#include <vector>

using namespace std;

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<vector<int>> M;
  for(int j=0; j<n; j++){
    vector<int> row;
    for(int k=0; k<n; k++){
      int a; cin >> a;
      row.push_back(a);
    }
    M.push_back(row);
  }
  //Parity partial sum matrix
  vector<vector<int>> P(n, vector<int>(n));
  for(int k=0; k<n; k++){
    int sum = 0;
    for(int j=0; j<n; j++){
      sum = (sum+M[j][k])%2;
      P[j][k] = sum;
    }
  }
  //For each pair of rows j1 <= j2, compute a vector v
  //So that for each column k, v[k] is the parity sum from M[j1][k] to M[j2][k]
  //But this is computed in constant time thanks to parity matrix per column P
  int count = 0;
  for(int j1=0; j1<n; j1++){
    for(int j2=j1; j2<n; j2++){
      vector<int> v;
      for(int k=0; k<n; k++){
        if(j1==0){
          v.push_back(P[j2][k]);
        }else{
          v.push_back((P[j2][k]-P[j1-1][k])%2);
        }
      }
      //Compute the number of even pairs for v and add to the total count
      int sum = 0;
      int even = 0; int odd = 0;
      for(int k=0; k<n; k++){
        sum += v[k];
        if (sum%2==0){
          even +=1;
        }else{
          odd+=1;
        }
      }
      int even_pairs = (odd * (odd-1) / 2) + (even * (even-1) / 2) + even;
      count += even_pairs;
    }
  }
  //OUTPUT
  cout << count << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}