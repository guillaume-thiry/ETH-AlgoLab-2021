// 1. Sort the boxes from heaviest to lighest, and the friends from strongest to weakest
// 2. If the strongest friend cannot hold the heaviest box, then a solution is impossible
// 3. Else, apply a greedy strategy until all the boxes are managed:
//      - At first have one friend (the strongest) doing one trip (for the heaviest box)
//      - If the next friend can hold the next biggest box, add him to the friends list
//      - Else, just increase the number of trips for everybody (so that somebody strong enough hold this box)
// In the end, the number of trips given is the minimum possible. Note that it may not even require the use of all friends

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool comp(int a, int b){
  return a>b;
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  vector<int> friends;
  for(int i=0; i<n; i++){
    int s; cin >> s;
    friends.push_back(s);
  }
  vector<int> boxes;
  for(int j=0; j<m; j++){
    int b; cin >> b;
    boxes.push_back(b);
  }
  
  //Sort friends and boxes
  sort(friends.begin(), friends.end(), comp);
  sort(boxes.begin(), boxes.end(), comp);
  
  //First check if a solution is even possible
  if(boxes[0]>friends[0]){
    cout << "impossible" << endl;
  }else{
    //Then apply the greedy strategy
    int nf = 1;
    int nb = 1;
    int total = 1;  //At each step of this loop, the number of boxes managed
    while(total<m){//While still boxes to take
      int next = nf*nb; //Index of the next heaviest box still to take
      if(nf==n){  //If all friends used already, just add more trips
        nb++;
      }else{
        if(boxes[next]>friends[nf]){  //Else, look if the next friend can help or just increase the number of trips by 1
          nb++;
        }else{
          nf++;
        }
      }
      total = nf*nb;  //Number of boxes taken is number of friends x number of trips (because everyone can take his boxes + the ligher ones)
    }
    
    //OUTPUT
    cout << 3*nb-1 << endl;
  }
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}