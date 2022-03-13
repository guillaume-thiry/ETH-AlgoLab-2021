// 1. Do a triangulation will the stations
// 2. Extract the neighbors of each vertex and do Union Find to form sets of linked stations
// 3. Color each component with 0s and 1s (to check if no interference)
// 4. If no interference after the coloring, we need to verify it one last time (problems can still happen because of the way the triangulation is done):
//  - Extract all vertices with color 0, create a new triangulation, and check that no edge of this triangulation is small enough to create interference
//  - Repeat for color 1
// 5. Final analysis (with or without interference) given positions of Holmes and Watson

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>         Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

using namespace std;

//Union Find structure with complexity optimization (height of trees)
struct UnionFind{
  vector<int> P;
  vector<int> R;
  
  UnionFind(int n){
    for(int i=0; i<n; i++){
      P.push_back(i);
      R.push_back(1);
    }
  }
  
  int Find(int i){
    while(i!=P[i]){
      i = P[i];
    }
    return i;
  }
  
  void Union(int i, int j){
    int a = Find(i);
    int b = Find(j);
    if(a!=b){
      if(R[a]>R[b]){
        P[b] = a; 
      }else{
        P[a] = b;
        R[b] = max(R[b], R[a]+1);
      }
    }
  }
};

//Color a component with alterning 0s and 1s with a DFS
//Return false if the component cannot be colored properly (conflicting colors)
bool coloring(int i, int c, vector<vector<int>>& nei, vector<int>& color){
  //Already colored in the good color
  if(color[i]==c){
    return true;
  }
  //Already colored but in the wrong color
  if(color[i]!=-1){
    return false;
  }
  
  //Color the vertex and propagate to neighbors
  color[i] = c;
  for(int k=0; k<int(nei[i].size()); k++){
    bool a = coloring(nei[i][k], (c+1)%2, nei, color);  //alternate the color here
    if(!a){
      return false;
    }
  }
  return true;
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long r; cin >> r;
  vector<K::Point_2> stations;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    stations.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(stations.begin(), stations.end());
  
  //Vertex index
  int id = 0;
  for(Vertex_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++){
    v->info() = id;
    id++;
  }
  
  UnionFind uf(n);
  
  //Find all neighbors of a vertex (all vertices at distance smaller than r) and store them in a vector
  //Union Find at the same time to create components according to radius r
  vector<vector<int>> nei(n);
  for(Edge_iterator e = t.finite_edges_begin(); e!= t.finite_edges_end(); e++){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    
    K::FT d = CGAL::squared_distance(v1->point(), v2->point());
    if(d<=r*r){
      uf.Union(v1->info(),v2->info());
      nei[v1->info()].push_back(v2->info());
      nei[v2->info()].push_back(v1->info());
    }
  }
  
  //Color all the vertices, component by component
  //Check if any conflicting issue in the coloring for one of the component
  //Value between different components is not relevant (not close enough to interfere anyway)
  vector<int> color(n, -1);
  bool no_interf = true;
  for(int i=0; i<n; i++){
    if(color[i]==-1){
      bool b = coloring(i, 0, nei, color);
      no_interf = no_interf && b;
    }
  }
  
  //If no issue in coloring, final check to see if any interference
  if(no_interf){
    vector<K::Point_2> c0;
    vector<K::Point_2> c1;
    
    //Extract vertices of color 0 and 1
    for(Vertex_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++){
      if(color[v->info()]==0){
        c0.push_back(v->point());
      }else{
        c1.push_back(v->point());
      }
    }
    //And create new triangulations
    Triangulation t0;
    t0.insert(c0.begin(), c0.end());
    Triangulation t1;
    t1.insert(c1.begin(), c1.end());
    
    //Check if no interference between stations of color 0
    for(Edge_iterator e = t0.finite_edges_begin(); e!= t0.finite_edges_end(); e++){
      Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
      Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
      K::FT d = CGAL::squared_distance(v1->point(), v2->point());
      if(d<=r*r){
        no_interf = false;
      }
    }
    //Same for color 1
    for(Edge_iterator e = t1.finite_edges_begin(); e!= t1.finite_edges_end(); e++){
      Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
      Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
      K::FT d = CGAL::squared_distance(v1->point(), v2->point());
      if(d<=r*r){
        no_interf = false;
      }
    }
  }
  
  //OUTPUT
  for(int j=0; j<m; j++){
    long x0, y0, x1, y1; cin >> x0 >> y0 >> x1 >> y1;
    K::Point_2 p(x0,y0);
    K::Point_2 q(x1,y1);
    
    if(!no_interf){ //In interference, directly no
      cout << 'n';
    }else{
      K::FT d = CGAL::squared_distance(p,q);
      if(d<=r*r){ //If Holmes and Watson close enough, yes
        cout << 'y';
      }else{
        //Else use the network
        Triangulation::Vertex_handle v1 = t.nearest_vertex(p);
        Triangulation::Vertex_handle v2 = t.nearest_vertex(q);
        if(CGAL::squared_distance(p, v1->point())>r*r){ //Check that Holmes is close enough to his station
          cout << 'n';
        }else if(CGAL::squared_distance(q, v2->point())>r*r){ //Same for Watson
          cout << 'n';
        }else{
          if(uf.Find(v1->info())==uf.Find(v2->info())){ //Finally check if in the same UF component or not
            cout << 'y';
          }else{
            cout << 'n'; 
          }
        }
      }
    }
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