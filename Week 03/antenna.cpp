// 1. Construct the smallest enclosing circle of the data points using EPEC kernel
// 2. Extract the support points of the circle and cast them into EPECS kernel
// 3. Construct a new circle in this new kernel with the support
// 4. Calcule the radius of the circle
// Using the heavier kernel EPECS only for a few points (2 or 3) and not all of them
// will make the calculations faster, which is necessary for the last test set

#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K2;

typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

typedef CGAL::Min_circle_2_traits_2<K2> Traits2;
typedef CGAL::Min_circle_2<Traits2> Min_circle2;

using namespace std;

double ceil_to_double(const K2::FT& x){
  double a = floor(CGAL::to_double(x));
  while (a >= x+1) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main(){
  //INPUT
  cout << fixed << setprecision(0);
  int n; cin >> n;
  while(n!=0){
    vector<K::Point_2> pos;
    for(int i=0; i<n; i++){
      long x, y; cin >> x >> y;
      K::Point_2 p(x,y);
      pos.push_back(p);
    }
    
    //First enclosing circle
    Min_circle mc(pos.begin(), pos.end(), true);
    
    int sup = mc.number_of_support_points();
    K2::FT d;
    
    //With only 2 support points, this gives us the squared diameter directly
    if(sup==2){
      K2::Point_2 p(CGAL::to_double(mc.support_point(0).x()), CGAL::to_double(mc.support_point(0).y())); //Always use this syntax to cast into another kernel
      K2::Point_2 q(CGAL::to_double(mc.support_point(1).x()), CGAL::to_double(mc.support_point(1).y()));
      d = CGAL::squared_distance(p,q)/4;
    //With 3 support points, we have to reconstruct a new circle
    }else if(sup==3){
      vector<K2::Point_2> new_points;
      for(int k=0; k<3; k++){
        K2::Point_2 p(CGAL::to_double(mc.support_point(k).x()), CGAL::to_double(mc.support_point(k).y())); //Same syntax
        new_points.push_back(p);
      }
      Min_circle2 mc2(new_points.begin(), new_points.end(), true);
      Traits2::Circle c = mc2.circle();
      d = c.squared_radius();
    }
    
    //To get the radius, necessary to get the square root (hence the EPECS kernel)
    cout << ceil_to_double(CGAL::sqrt(d)) << endl;
    
    cin >> n;
  }
}