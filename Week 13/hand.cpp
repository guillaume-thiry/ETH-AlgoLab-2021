// 1. Triangulate the problem to get all the edges and the length
// 2. Sort the edges by length
// 3. Then do Union Find of the vertices (either all the edges under given length or by increasing length, depending on the question)
// 4. Calculate at the same time the number of families this gives (with functions 'update' and 'family')
// We use the small value of k (<= 4) to calculate easily the number of families


#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

using namespace std;

//Union Find structure that keeps in memory the size of each component
struct UnionFind{
  vector<int> P;
  vector<int> S;
  
  UnionFind(int n){
    for(int i=0; i<n; i++){
      P.push_back(i);
      S.push_back(1);
    }
  }
  
  int Find(int i){
    while(P[i]!=i){
      i = P[i];
    }
    return i;
  }
  
  bool Union(int i, int j){ //Note here that the Union returns a boolean, to directly tell us if the components were different or not (very important for merging families)
    int a = Find(i);
    int b = Find(j);
    if(a!=b){
      P[a] = b;
      S[b] += S[a];
      return true;
    }
    return false;
  }
  
  int size(int i){
    return S[Find(i)];
  }
};

//Given integer k, and components of size 1, 2, 3 or above k, assemble all of that into families 
int family(vector<int> size, int k){
  //If k==1, every single component (of any size) is a family
  if(k==1){
    return (size[0]+size[1]+size[2]+size[3]);
  }
  //k=2, all components of size more than 2 are families, and then we can merge together components of size 1 to get extra families
  if(k==2){
    int res = size[0]+size[2]+size[3];
    res += size[1]/2; //Two 1s make a family
    return res;
  }
  //k=3, now we have components of size 1 and 2 left to form extra families
  //So we assemble a 1 and 2 until we ran out of one of them
  //And deal with the rest
  if(k==3){
    int res = size[0]+size[3];
    if(size[1]<=size[2]){//More 2s than 1s
      res += size[1];
      res += (size[2]-size[1])/2;//Two 2s make a family (can't do better)
    }else{
      res += size[2];
      res += (size[1]-size[2])/3; //Three 1s make a family too
    }
    return res;
  }
  //k=4, we have components of size 1, 2 and 3 left to form extra families
  //First assemble 1s and 3s until you run out of one of them
  //Then deal with the rest
  if(k==4){
    int res = size[0];
    if(size[1]<=size[3]){ //More 3s than 1s
      res += size[1];
      res += (size[3]-size[1]+size[2])/2; //A 2 and a 3 (or two 2s or two 3s) gives a family (not better)
    }else{
      res += size[3];
      res += (size[1]-size[3]+2*size[2])/4; //Two 1s and a 2 (or two 2s or four 1s)
    }
    return res;
  }
  return 0;
}

//Update the vector of families size by merging a component of size a with a component of size b
vector<int> update(vector<int> s, int a, int b){
  int c = a+b;
  if(c>3){ //Everything above 3 directly goes in category 0
    c = 0;
  }
  if(a>3){
    a = 0; //Same
  }
  if(b>3){
    b = 0; //Same
  }
  //Update the counts
  s[a]--;
  s[b]--;
  s[c]++;
  return s;
}

void testcase(){
  //INPUT
  int n, k, f; cin >> n >> k >> f;
  long s; cin >> s;
  vector<K::Point_2> points;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    points.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(points.begin(), points.end());
  
  //Vertex index
  int id = 0;
  for(Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v){
    v->info()=id;
    id++;
  }
  
  //Get edges and their length
  vector<pair<K::FT, pair<int,int>>> edges;
  
  for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    K::FT d = CGAL::squared_distance(v1->point(), v2->point());
    pair<K::FT, pair<int,int>> p  = make_pair(d, make_pair(v1->info(), v2->info()));
    edges.push_back(p);
  }
  
  //Sort edges
  sort(edges.begin(), edges.end());
  
  vector<int> size = {0,n,0,0}; //This stores the number of families of size 1,2,3 and 0 (meaning the family size already exceeds k)
  K::FT smax = 0;
  int i = 0;
  UnionFind uf(n);
  int ne = edges.size();
  
  //While we still have enough family, take the next edge and merge the components in the UF (meaning all the vertices now belong to one family)
  //Stop when we don't have enough families anymore: the last edge gives the critical length above which the assignment is not possible
  while(true){
    auto e = edges[i];
    int x = e.second.first;
    int y = e.second.second;
    int sx = uf.size(x);
    int sy = uf.size(y);
    if(uf.Union(x,y)){ //If we actually merged two components, some families were modified
      size = update(size, sx, sy); //Update the family sizes count
      int new_f = family(size, k); //And calculate the new number of families
      smax = e.first;
      if(new_f<f){
        break;
      }
    }
    i++;
    if(i==ne){
      break;
    }
  }
  
  //Second Union Find, here we merge all the edges with length small enough at once
  UnionFind uf2(n);
  vector<int> size2 = {0,n,0,0};
  
  for(int i=0; i<ne; i++){
    auto e = edges[i];
    int x = e.second.first;
    int y = e.second.second;
    int sx = uf2.size(x);
    int sy = uf2.size(y);
    if(e.first<s){
      if(uf2.Union(x,y)){
        size2 = update(size2, sx, sy); //with update along the way
      }
    }
  }
  //But calculate only the number of families at the end (not useful before)
  int f_max = family(size2, k);
  
  //OUTPUT
  cout << fixed << setprecision(0) << smax << ' ' << f_max << endl;
}

int main(){
  int t; cin >> t;
  while(t--){
    testcase();
  }
}