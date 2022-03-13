// 1. Compute the vector of partial sums and count the number of odds and even partial sums
// 2. The number of even pairs can be deducted from that numbers using just a formula

#include <iostream>
#include <vector>

using namespace std;

void testcase(){
  int n; cin >> n;
  vector<int> v;
  vector<int> s;
  int sum = 0;
  int even = 0; int odd = 0;
  for(int j=0; j<n; j++){
    int a; cin >> a;
    v.push_back(a);
    sum += a;
    if(sum%2==0){
      even+=1;
    }else{
      odd+=1;
    }
  }
  int count = (odd * (odd-1) / 2) + (even * (even-1) / 2) + even; //Formula

  cout << count << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}