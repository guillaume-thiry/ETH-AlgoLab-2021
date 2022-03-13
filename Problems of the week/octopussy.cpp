// 1. Sort the bombs by deactivation time (smallest to highest)
// 2. Deactivate each bomb one at a time, checking the time to do it (and deactivation others while doing it)
// When recursively deactivating the dependencies bombs, no need to check their time because if they were to explode,
// the bomb we are trying to deactivate in the first place would explode before anyway (because it's the one with smallest time)

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//Compute the time to deactivate bomb i, recursively deactivating the others
int deactivate(int i, int n, vector<bool>& deactivated){
  //Already out, nothing to do
  if(deactivated[i]){
    return 0;
  }
  //If terminal bombs, just deactivate them
  if(2*i+1>=n){
    deactivated[i] = true;
    return 1;
  }
  //Else, first deactivate everything behing
  int t1 = deactivate(2*i+1, n, deactivated);
  int t2 = deactivate(2*i+2, n, deactivated);
  
  //And deactivate it
  deactivated[i]=true;
  return t1+t2+1;
}

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<int> times;
  vector<pair<int,int>> bombs;
  for(int i=0; i<n; i++){
    int t; cin >> t;
    times.push_back(t);
    bombs.push_back(make_pair(t,i));
  }
  
  sort(bombs.begin(), bombs.end());
  
  vector<bool> deactivated(n, false); //Stores which bombs are already deactivated
  bool explode = false;
  
  int tot = 0;
  for(int i=0; i<n; i++){
    auto bomb = bombs[i];
    if(!deactivated[bomb.second]){
      int t = deactivate(bomb.second, n, deactivated);  //Calculate how much time to deactivate (with dependencies)
      //dependencies not already deactivated have all a time larger than this one (greedy) so no need to check them
      if(tot+t>bomb.first){ //If too long, explode
        explode = true;
        break;
      }else{
        tot += t;
      }
    }
  }
  //OUTPUT
  if(explode){
    cout << "no" << endl;
  }else{
    cout << "yes" << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}