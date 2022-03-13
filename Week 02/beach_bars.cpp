// 1. Sort parasols locations
// 2. Use a sliding window of size 200 through this sorted list
// 3. Update max_para and max_dist to always keep the best possible locations

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<int> pos;
  for(int i=0; i<n; i++){
    int x; cin >> x;
    pos.push_back(x);
  }
  
  //First sort the parasols locations
  sort(pos.begin(), pos.end());
  
  //To store the results
  int max_para = 0;
  double max_dist = 0;
  int count = 1;
  double dist = 0;
  vector<int> final_pos;
  
  //Do a sliding window on the parasols
  int i=0; int j=0; //Indexes of the sliding window
  while(true){
    //Window too big: drop the left-most parasol
    if(dist>200){
      count --;
      i++;
    //Else add a parasol to the right
    }else{
      count ++;
      j++;
    }
    //If no more parasol to add, end of the loop
    if(j==n){
      break;
    }else{
      dist = pos[j]-pos[i]; //update the window size
      if(dist<=200){ //if valid window
        if(count>max_para){
          max_para = count;
          max_dist = ceil(dist/2);  //the best is to center the window, so the max distance for the parasols is dist/2
          if(int(dist)%2 == 0){ //if center is integer, only one possible bar location
            final_pos = {pos[i]+int(floor(dist/2.0))};
          }else{  //else two positions = dist/2  +/-  0.5
            final_pos = {pos[i]+int(floor(dist/2)), pos[i]+int(ceil(dist/2))};
          }
        }else if(count == max_para){
          if(max_dist>ceil(dist/2)){ //same number of parasols but smaller distance
            max_dist = ceil(dist/2);
            if(int(dist)%2 == 0){
              final_pos = {pos[i]+int(floor(dist/2))};
            }else{
              final_pos = {pos[i]+int(floor(dist/2)), pos[i]+int(ceil(dist/2))};
            }
          }else if(max_dist == ceil(dist/2)){ //same number of parasols and same distance: just add new possible locations
            if(int(dist)%2 == 0){
              final_pos.push_back(pos[i]+int(floor(dist/2)));
            }else{
              final_pos.push_back(pos[i]+int(floor(dist/2)));
              final_pos.push_back(pos[i]+int(ceil(dist/2)));
            }
          }
        }
      }
    }
  }
  
  //OUTPUT
  cout << max_para << " " << max_dist << endl;
  int k = final_pos.size();
  for(int a = 0; a<k; a++){
    cout << final_pos[a] << " "; 
  }
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}