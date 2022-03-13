// 1. Use functions from CGAL to check intersection and calculate intersection point
// 2. After the first intersection, truncate the ray so that it stops at this intersection (making it a segment)
// That way, if a new intersection with this new "ray" is detected, we know that it's closer
// And we avoid calculating many intersection points that are too far away

#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

using namespace std;

double floor_to_double(const K::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main(){
  //INPUT
  cout << fixed << setprecision(0);
  int n; cin >> n;
  while(n!=0){
    long a, b, x, y; cin >> a >> b >> x >> y;
    P p(a,b), q(x,y);
    K::Ray_2 ray(p,q);
    vector<S> vect;
    for(int i=0; i<n; i++){
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      P pi(r,s), qi(t,u);
      S seg(pi,qi);
      vect.push_back(seg);
    }
    random_shuffle(vect.begin(), vect.end()); //Shuffle the segments to avoid the worst case scenario (last test set)
    
    bool hit = false;
    K::FT dist;
    P closest; //Closest point so far
    S new_ray; //Actually it's a segment
    
    for(int j=0; j<n; j++){
      S seg = vect[j];
      
      bool new_hit; 
      if(!hit){ //Check if already one intersection (to know if we intersect the original ray or the truncated segment)
        new_hit = CGAL::do_intersect(ray,seg);
      }else{
        new_hit = CGAL::do_intersect(new_ray,seg);
      }
      
      //If a new hit is detected, we know the intersection is closer
      if(new_hit){
        hit = true;
        auto o = CGAL::intersection(ray,seg); //Calculate intersection here
        
        //Now we check if the new intersection is a point or a segment
        if (const P* op = boost::get<P>(&*o)){
          new_ray = S(p, *op);  //If a point, easy
          
        }else if (const S* os = boost::get<S>(&*o)){
          //Else, find the closest of the two points of the segment
          P p1 = os->source();
          P p2 = os->target();
          P new_closest;
          if (CGAL::squared_distance(p,p1)<CGAL::squared_distance(p,p2)){
            new_closest = p1;
          }else{
            new_closest = p2;
          }
          new_ray = S(p, new_closest); //update the truncated ray accordingly
        }
      }
    }
    
    //OUTPUT
    if(hit){
      P closest = new_ray.target();
      cout << setprecision(0) << fixed << floor_to_double(closest.x()) << " " << floor_to_double(closest.y()) << endl;
    }else{
      cout << "no" << endl;
    }
    cin >> n;
  }
}