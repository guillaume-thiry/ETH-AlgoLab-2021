// 1. For each triangle, determine all the edges included in it (with 'inside' function)
// 2. Sliding window on the triangles list, keeping a record of the edges included at any point

#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/enum.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Segment_2 S;

using namespace std;

//The segment is inside the triangle if both its points are
//We use the CGAL orientation here to check if, for each edge of the triangle,
//the point is on the right side of the edge (or directly on the edge)
//If true for all 3 edges, the point is inside the triangle
//To know what is the good side of the edge, we use the other points of the triangle as reference
bool inside(vector<P>& points, vector<P>& tri){
  for(int i=0; i<3; i++){
    auto ref = CGAL::orientation(tri[2*i], tri[2*i+1], tri[(2*i+2)%6]); //Orientation reference using another point of the triangle
    for(int j=0; j<2; j++){
      auto orient = CGAL::orientation(tri[2*i], tri[2*i+1], points[j]); //Orientation of our point
      if(orient!=CGAL::COLLINEAR){  //if COLLINEAR, the point is on the edge
        if(orient!=ref){
          return false; //Early return of false to speed up the process
        }
      }
    }
  }
  return true;
}

void testcase(){
  //INPUT
  int m, n ; cin >> m >> n;
  vector<P> points;
  for(int i=0; i<m; i++){
    long x, y; cin >> x >> y;
    P p(x,y);
    points.push_back(p);
  }
  vector<vector<P>> triangles(n);
  for(int i=0; i<n; i++){
    for(int k=0; k<6; k++){
      long x, y; cin >> x >> y;
      P p(x,y);
      triangles[i].push_back(p);
    }
  }
  vector<vector<P>> seg(m-1);
  for(int j=0; j<m-1; j++){
    seg[j].push_back(points[j]);
    seg[j].push_back(points[j+1]);
  }
  
  //For each triangle, store a list of all the segments contained in it
  //Calculated with the function 'inside'
  vector<vector<int>> contains(n);
  for(int i=0; i<n; i++){
    for(int j=0; j<m-1; j++){
      if(inside(seg[j], triangles[i])){
        contains[i].push_back(j);
      }
    }
  }
  
  //Then we do a sliding window on the triangle, keeping at each step
  //the information of which edges are included and which are not
  //At each point when all are included, check the size of the window
  int cost = INT_MAX;
  int i=0; int j=0;
  vector<int> counts(m-1,0); //Store how many triangles in the current window contain if edge
  int count = 0; //Store the number of edges included in at least one triangle
  while(true){
    if(count<m-1){
      if(j<n){
        for(int k=0; k<int(contains[j].size()); k++){ //Update counts for all edges in the new triangle
          counts[contains[j][k]]++;
          if(counts[contains[j][k]]==1){  //If 1, it went from 0 to 1 so we can increase 'count' (one more edge is included in the window)
            count++;
          }
        }
        j++;
      }else{
        break;
      }
    }else{
      cost = min(cost, j-i); //Update the min cost 
      for(int k=0; k<int(contains[i].size()); k++){ //Update counts for all the edges in the lost triangle
        counts[contains[i][k]]--;
        if(counts[contains[i][k]]==0){  //If 0, this edge is not included anymore in the window so decrease 'count'
          count--;
        }
      }
      i++;
    }
  }
  ///OUTPUT
  cout << cost << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}