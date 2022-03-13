// 1. Triangulate the jammers
// 2. Use a Union Find structure to group together the jammers jointly joinable
// 3. Use the components of the UF to answer the queries of the first question
// 4. While doing that, keep some information of the minimal radius needed to connect to the jammers and store the edges with their length
// 5. Do a second Union Find, progressively adding more edges (by increasing length) to answer the queries, to get answer for second question

#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

using namespace std;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

//Union Find structure
struct UnionFind{
  vector<int> P;
  
  UnionFind(int n){
    for(int i=0; i<n; i++){
      P.push_back(i);
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
      P[a] = b;
    }
  }
};

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long r; cin >> r;
  vector<K::Point_2> points;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    points.push_back(p);
  }
  vector<pair<K::Point_2, K::Point_2>> queries;
  for(int j=0; j<m; j++){
    long x1, y1; cin >> x1 >> y1;
    long x2, y2; cin >> x2 >> y2;
    K::Point_2 p1(x1,y1);
    K::Point_2 p2(x2,y2);
    queries.push_back(make_pair(p1,p2));
  }
  
  //Triangulation
  Triangulation t;
  t.insert(points.begin(), points.end());
  //Vertex index
  int id = 0;
  for(Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v){
    v->info() = id;
    id++;
  }
  
  K::FT res_all = 0;  //Store the max radius required for answering all queries
  K::FT res_ok = 0; //Same but only for queries that were right in the first question
  
  UnionFind uf(n);
  vector<pair<K::FT, pair<int,int>>> edges;
  //Do the Union of the components, while also storing the edges of the triangulation with their length
  for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    K::FT d = CGAL::squared_distance(v1->point(), v2->point());
    edges.push_back(make_pair(d, make_pair(v1->info(), v2->info())));
    if(d<=r){
      uf.Union(v1->info(),v2->info());
    }
  }
  
  vector<pair<int,int>> all; //For each mission, store the start and end jammer for second question (unless the are the same)
  vector<pair<int,int>> ok; //Same but only for successful mission
  //Answer the queries
  for(int j=0; j<m; j++){
    K::Point_2 p = queries[j].first;
    K::Point_2 q = queries[j].second;
    Triangulation::Vertex_handle v1 = t.nearest_vertex(p);
    Triangulation::Vertex_handle v2 = t.nearest_vertex(q);
    
    K::FT d1 = CGAL::squared_distance(p, v1->point())*4;  //Distance between start of the mission and closest jammer
    K::FT d2 = CGAL::squared_distance(q, v2->point())*4;  //Same for the end
    
    res_all = max(res_all, d1); //Update the max_res for all missions with these distances (min radius to at least be under an umbrella at start and and)
    res_all = max(res_all, d2);
    if(v1->info()!=v2->info()){ //If not linked to the same jammer, add the mission to the list (for second question), no need if the same jammer (automatic same component)
      all.push_back(make_pair(v1->info(), v2->info()));
    }
    
    if(d1>r or d2>r){//Check that start and end of the mission are already under umbrella
      cout << 'n';
    }else{
      if(uf.Find(v1->info())==uf.Find(v2->info())){ //If same component, mission accomplished
        cout << 'y';
        res_ok = max(res_ok, d1); //Then update res_ok and ok with this mission (same as with res_all and all)
        res_ok = max(res_ok, d2);
        if(v1->info()!=v2->info()){
          ok.push_back(make_pair(v1->info(), v2->info()));
        }
      }else{
        cout << 'n';
      }
    }
  }
  //Sort the edges by increasing length
  sort(edges.begin(), edges.end());
  int n1 = all.size();
  int n2 = ok.size();
  
  UnionFind uf2(n);//Second UF structure
  
  //Progressively add more edges in the UF until all missions (or all successful missions) are done
  int i1 = 0; int i2 = 0;
  int j = 0;
  while(i1<n1 or i2<n2){
    uf2.Union(edges[j].second.first, edges[j].second.second);
    //All missions
    while(i1<n1 and (uf2.Find(all[i1].first)==uf2.Find(all[i1].second))){
      i1++;
      res_all = max(res_all, edges[j].first);
    }
    //Accomplished missions
    while(i2<n2 and (uf2.Find(ok[i2].first)==uf2.Find(ok[i2].second))){
      i2++;
      res_ok = max(res_ok, edges[j].first);
    }
    j++;
  }
  //OUTPUT
  cout << endl << res_all << endl << res_ok << endl;
}

int main(){
  cout << fixed << setprecision(0);
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}