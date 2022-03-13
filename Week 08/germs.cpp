// 1. Use each bacterium as a point for a triangulation
// 2. Keep min distance of contact as information for each vertex
// 3. First initialize this distance for each vertex using the border of the dish
// 4. Explore the edges of the triangulation and update the distances
// 5. Store the distances in a vector, sort it, and take first, middle and last value
// 6. Calculate corresponding time and outputs

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K2;
typedef CGAL::Triangulation_vertex_base_with_info_2<K::FT, K>       Vb; //Here we store distances as K::FT with each vertex
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;


double ceil_to_double(const K2::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while (a >= x+1) a -= 1;
  while (a < x) a += 1;
  return a;
}

int main(){
  //INPUT
  int n; cin >> n;
  while(n!=0){
    int l, b, r, t; cin >> l >> b >> r >> t;
    vector<K::Point_2> points;
    for(int i=0; i<n; i++){
      long x, y; cin >> x >> y;
      K::Point_2 p(x,y);
      points.push_back(p);
    }
    
    //Triangulation
    Triangulation tri;
    tri.insert(points.begin(), points.end());
    
    //Init of distances with borders of the dist
    for(Vertex_iterator v = tri.finite_vertices_begin(); v!=tri.finite_vertices_end(); ++v){
      K::FT x = v->point().x();
      K::FT y = v->point().y();
      K::FT a1 = min((r-x)*(r-x),(x-l)*(x-l));
      K::FT a2 = min((t-y)*(t-y), (y-b)*(y-b));
      v->info() = min(a1,a2);
    }
    
    //Update the distances using the edges of the triangulation
    for(Edge_iterator e = tri.finite_edges_begin(); e!=tri.finite_edges_end(); ++e){
      Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
      Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
      K::FT d = CGAL::squared_distance(v1->point(), v2->point())/4;
      v1->info() = min(d, v1->info());
      v2->info() = min(d, v2->info());
    }
    //Extract distances and sort them
    vector<K::FT> dist;
    for(Vertex_iterator v = tri.finite_vertices_begin(); v!=tri.finite_vertices_end(); ++v){
      dist.push_back(v->info());
    }
    sort(dist.begin(), dist.end());
    
    //Final calculations and OUTPUT
    K2::FT r1(CGAL::to_double(dist[0]));
    K2::FT r2(CGAL::to_double(dist[n/2]));
    K2::FT r3(CGAL::to_double(dist[n-1]));
    
    cout << ceil_to_double(CGAL::sqrt(CGAL::sqrt(r1)-0.5)) << ' ';
    cout << ceil_to_double(CGAL::sqrt(CGAL::sqrt(r2)-0.5)) << ' ';
    cout << ceil_to_double(CGAL::sqrt(CGAL::sqrt(r3)-0.5)) << ' ';
    cout << endl;
    cin >> n;
  }
}