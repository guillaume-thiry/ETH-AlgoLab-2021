// 1. Call CGAL intersection function for each segment
// 2. Stop at first intersection

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

using namespace std;

int main(){
  //INPUT
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n!=0){
    long x, y, a, b; cin >> x >> y >> a >> b;
    P p1(x,y);
    P p2(a,b);
    K::Ray_2 ray(p1,p2);
    vector<S> segments;
    for(int i=0; i<n; i++){
      long r, s, t, u; cin >> r >> s >> t >> u;
      P p(r,s);
      P q(t, u);
      S seg(p,q);
      segments.push_back(seg);
    }
    
    
    bool intersect = false;
    for(int i=0; i<n; i++){
      S seg = segments[i];
      if(!intersect){ //Stop calculating intersection after finding the first one
        if(CGAL::do_intersect(ray, seg)){
          intersect = true;
        }
      }
    }
    
    //OUTPUT
    if(intersect){
      cout << "yes" << endl;
    }else{
      cout << "no" << endl;
    }
    cin >> n;
  }
}