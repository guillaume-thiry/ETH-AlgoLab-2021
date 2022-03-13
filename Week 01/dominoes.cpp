// 1. Loops and max

#include <iostream>

using namespace std;

int main(){
  int t; cin >> t;
  for(int i=0; i<t; i++){
    int n; cin >> n;
    int l = 0;
    for(int j=0; j<n; j++){
      if(j>l){
        int h; cin >> h;
      }else{
        int h; cin >> h;
        l = max(l, j+h-1);
      }
    }
    cout << min(l+1,n) << endl;
  }
}