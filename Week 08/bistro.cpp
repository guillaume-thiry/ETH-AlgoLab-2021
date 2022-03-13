// 1. Just a simple triangulation

#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

using namespace std;

int main(){
  //INPUT
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n!=0){
    vector<K::Point_2> points;
    for(int i=0; i<n; i++){
      double x, y; cin >> x >> y;
      K::Point_2 p(x,y);
      points.push_back(p);
    }
    
    //Triangulation
    Triangulation t;
    t.insert(points.begin(), points.end());
    //Find nearest vertex and OUTPUT
    int m; cin >> m;
    for(int j=0; j<m; j++){
      double x, y; cin >> x >> y;
      K::Point_2 p(x,y);
      K::FT d = CGAL::squared_distance(p, t.nearest_vertex(p)->point());
      cout << fixed << setprecision(0) << d << endl;
    }
    cin >> n;
  }
}