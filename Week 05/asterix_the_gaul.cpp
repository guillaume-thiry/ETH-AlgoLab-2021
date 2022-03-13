// 1. Find for each integer k the highest possible distance to be done in k moves, while keeping time under T
// This is done enumerating the set (split and list)
// 2. For each number of gulps, try to find a distance large enough (under T) by using values from step 1

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool comp_a(pair<pair<long,long>,int> a, pair<pair<long,long>,int> b){
  return a.first.first < b.first.first;
}
bool comp_d(pair<pair<long,long>,int> a, pair<pair<long,long>,int> b){
  return a.first.first > b.first.first;
}

//Explore all the combinations of moves and return the highest distance (for time under T) for each number of moves allowed
vector<long> explore(vector<pair<long,long>>& moves, int n, long T){
  //Split the moves into 2 sets (split and list)
  int n1 = n/2;
  int n2 = n-n1;
  
  vector<pair<pair<long,long>,int>> combi1;
  vector<pair<pair<long,long>,int>> combi2;
  
  //Enumerate moves combination for first set
  for(int i=0; i< 1<<n1; i++){
    long sum_d = 0;
    long sum_p = 0;
    int c = 0;
    for(int j=0; j<n1; j++){
      if(i & 1<<j){
        sum_d += moves[j].first;
        sum_p += moves[j].second;
        c++;
      }
    }
    combi1.push_back(make_pair(make_pair(sum_p,sum_d), c));
  }
  //Enumerate moves combination for second set
  for(int i=0; i< 1<<n2; i++){
    long sum_d = 0;
    long sum_p = 0;
    int c = 0;
    for(int j=0; j<n2; j++){
      if(i & 1<<j){
        sum_d += moves[n1+j].first;
        sum_p += moves[n1+j].second;
        c++;
      }
    }
    combi2.push_back(make_pair(make_pair(sum_p,sum_d), c));
  }
  
  //To combine the two lists of moves combination, sort one in increasing time and the other in decreasing time
  //By keeping the max distance (for each number of moves) as we explore the first one, it can be combine with element of the second time instantly
  //In the end, each set will be explored once (linear in the set size)
  
  vector<long> max_d(n1+1,-1); //Current max distance for list 1
  vector<long> res(n+1, -1);
  
  //Sort both lists in different orders
  sort(combi1.begin(), combi1.end(), comp_a);
  sort(combi2.begin(), combi2.end(), comp_d);
  
  int i1 = 0; int i2 = 0;
  int l1 = combi1.size(); int l2 = combi2.size();
  
  while(true){
    if(i2<l2){
      while(i1<l1 and combi1[i1].first.first + combi2[i2].first.first < T){ //Increase in list 1 until the sum of time with element in list 2 is above T
          max_d[combi1[i1].second] = max(max_d[combi1[i1].second], combi1[i1].first.second); //Update max distance store for list 1
          i1++;
      }
      for(int k=0; k<n1+1; k++){
        if(max_d[k]!=-1){
          res[k+combi2[i2].second] = max(res[k+combi2[i2].second], max_d[k]+combi2[i2].first.second); //Combine both lists using element in 2 and max distance store for 1
        }
      }
      i2++;
    }else{
      break;
    }
  }
  return res;
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long D, T; cin >> D >> T;
  vector<pair<long,long>> moves;
  for(int i=0; i<n; i++){
    long d, t; cin >> d >> t;
    moves.push_back(make_pair(d,t));
  }
  vector<long> gulps = {0};
  for(int j=0; j<m; j++){
    long g; cin >> g;
    gulps.push_back(g);
  }
  
  //Highest distance under T with k moves
  vector<long> paths = explore(moves, n, T);
  
  //For each gulps try every k (number of moves) to check if distance can be above D
  int res;
  int k = 0;
  bool ok = false;
  while(k<m+1 and !ok){
    for(int i=0; i<n+1; i++){
      long best = paths[i];
      if(best!=-1){
        if(best+i*gulps[k]>=D){
          res = k;
          ok = true;
        }
      }
    }
    k++;
  }
  
  //OUTPUT
  if(ok){
    cout << res << endl;
  }else{
    cout << "Panoramix captured" << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}