// 1. Sort the boats by increasing position of the ring
// 2. Add the very first boat, this one is always here
// 3. Then greedily find the next boat to add to maximize the number of boats by choosing the one ending the soonest
// A boat end depends on its ring position, its length, and the end of the previously added boat

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<pair<int,int>> boats;
  for(int i=0; i<n; i++){
    int l, p; cin >> l >> p;
    boats.push_back(make_pair(p,l));
  }
  
  //Sort the boats by ring position
  sort(boats.begin(), boats.end());
  
  int res = 1;
  int i = 1;
  int limit = boats[0].first; //First boat always added and define the left limit
  
  //While possible, add to the list the next boat ending the soonest
  while(i<n){
    int new_lim = max(boats[i].first, limit+boats[i].second); //First use this formula to compute where the very next boat will finish
    i++;
    while(i<n and boats[i].first<new_lim){ //Now look at all the next boats that could finish before this limit (so they begin before that limit)
      int other_lim = max(boats[i].first, limit+boats[i].second); //And check if they actually finish before
      new_lim = min(new_lim, other_lim);  //If so, the finishing end will be updated
      i++;
    }
    //Once the boat is found, add it to the list and start again from here to add the next boats
    res++;
    limit = new_lim;
  }
  
  //OUTPUT
  cout << res << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}