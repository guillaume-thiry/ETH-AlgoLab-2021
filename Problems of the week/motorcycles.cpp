// 1. For each motorcycle, store its initial y position and its angle (Delta_y/Delta_x) using a CGAL Kernel
// 2. Sort the motorcyles from highest to lowest departure point
// 3. From top to bottom, progressively add motorcycle with a positive angle (go up) if the angle is smaller (in absolute value) than all previous ones (no crossing)
// Else, the biker will cross another one above having an smaller angle and lose
// In case of equal angle, this biker always wins
// 4. From bottom to top, progressively add motorcycle with a negative angle (go down) if the angle is smaller (in absolute value) than all previous ones
// Be careful in case a equal (absolute) angle for this one: 
//    - if the angles have the same sign, they are parallel and no problem
//    - if the angles are opposite, then this biker loses (will come from the left)
// A drawing can help

#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

bool comp_up(pair<double, pair<K::FT, int>> a, pair<double, pair<K::FT, int>> b){
  return a.first > b.first;
}

void testcase(){
  //INPUT
  int n; cin >> n;
  vector<pair<double, pair<K::FT, int>>> points;
  for(int i=0; i<n; i++){
    double y0, x1, y1; cin >> y0 >> x1 >> y1;
    K::FT angle = (y1-y0);
    angle /= x1;
    points.push_back(make_pair(y0, make_pair(angle,i)));
  }
  
  vector<int> res(n, 0); //Accepted bikers
  
  //Sort from largest to lowest departure point
  sort(points.begin(), points.end(), comp_up);
  K::FT min_angle = abs(points[0].second.first); //Keep here the minimal absolute angle of all previously seen bikers
  for(int i=0; i<n; i++){
    K::FT angle = points[i].second.first;
    int id = points[i].second.second;
    if(angle==0){ //angle 0 always wins
      res[id] = 1;
    }else if(angle>0){  //else if positive, compare to current min angle (absolute)
      if(min_angle>=angle){
        res[id]=1;
      }
    }
    min_angle = min(min_angle, abs(angle)); //and update min angle
  }
  
  //Sort from lowest to highest now
  sort(points.begin(), points.end());
  min_angle = abs(points[0].second.first);
  int sens = -1;  //use this to store the sign of the min_angle (in case of equal angle)
  //if at least one min angles is positive, then sens should be 1
  for(int i=0; i<n; i++){
    K::FT angle = points[i].second.first;
    int id = points[i].second.second;
    if(angle<0){
      if(min_angle>abs(angle)){ //compare with min_angle
        res[id] = 1;
      }else if(min_angle==abs(angle)){  //in case of equality, use the sign to know what the situation is
        if(sens==-1){
          res[id] = 1;
        }
      }
    }
    //A more complex update of the min angle
    if(min_angle>abs(angle)){
      min_angle = abs(angle);
      if(angle<0){
        sens = -1;
      }else{
        sens = 1;
      }
    }else if(min_angle==abs(angle)){
      if(angle>0){  //at least one positive --> sens = 1 (enough to make the bikers lose)
        sens = 1; 
      }
    }
  }
  
  //OUTPUT
  for(int i=0; i<n; i++){
    if(res[i]==1){
      cout << i << " ";
    }
  }
  cout << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}