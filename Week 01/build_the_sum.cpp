// 1. Loops

#include <iostream>

using namespace std;

int main(){
  int t; cin >>t;
  for(int i=0; i<t; i++){
    int n; cin >> n;
    int som=0;
    for(int j=0; j<n; j++){
      int a; cin >> a;
      som += a;
    }
    cout << som << endl;
  }
}