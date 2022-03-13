// 1. Do a triangulation with the trees, and store an index for each vertex (with info)
// 2. With the given radius, use a Union Find structure to put together trees that are close enough, and add bones close enough to them
// 3. At the same time, store the edges between two trees, and between a bone and its tree
// 4. Sort these edges and initiate a second Union Find structure
// 5. Progressively add the edges (from shortest to longest) until having enough bone in one of the component (given by max_bone) and keep track of the lengths

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

//Union Find structure, but modified for the problem
struct UnionFind{
  vector<int> P;
  vector<int> B;  //here each component also has a number of bones attached to it
  int max_bone = 0; //and we keep the maximal number of bones in a component
  
  UnionFind(int n){
    for(int i=0; i<n; i++){
      P.push_back(i);
      B.push_back(0);
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
      B[b] += B[a];
      max_bone = max(max_bone, B[b]); //when merging two components, also add their bones
    }
  }
  //we can also add a single new bone to a tree (and therefore to its component)
  void add_bone(int i){
    B[Find(i)]++;
    max_bone = max(max_bone, B[Find(i)]);
  }
  
  int get_max(){
    return max_bone;
  }
};

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  long s; cin >> s;
  int k; cin >> k;
  
  vector<K::Point_2> trees;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    trees.push_back(p);
  }
  
  vector<K::Point_2> bones;
  for(int j=0; j<m; j++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    bones.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  //Each vertex in the triangulation has an index (for the UnionFind)
  int id = 0;
  for(Vertex_iterator v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++){
    v->info() = id;
    id++;
  }
  
  UnionFind uf(n); //First Union Find for the first question
  
  //For each bone, if close enough to a tree (nearest_vertex), then add it to the tree bone count (in Union Find)
  vector<pair<K::FT, int>> bone_edges;
  for(int j=0; j<m; j++){
    Triangulation::Vertex_handle v = t.nearest_vertex(bones[j]);
    if(CGAL::squared_distance(bones[j], v->point())<=s/4){
      uf.add_bone(v->info());
    }
    bone_edges.push_back(make_pair(CGAL::squared_distance(bones[j], v->point()), v->info())); //At the same time, store the bone-tree edges
  }
  
  //Merge two trees together in the UF if they are close enough (explore edges of the triangulation)
  vector<pair<K::FT, pair<int,int>>> tree_edges;
  for(Edge_iterator e = t.finite_edges_begin(); e!= t.finite_edges_end(); e++){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
    if(CGAL::squared_distance(v1->point(), v2->point())<=s){
      uf.Union(v1->info(), v2->info());
    }
    tree_edges.push_back(make_pair(CGAL::squared_distance(v1->point(), v2->point()), make_pair(v1->info(), v2->info()))); //At the same time, store the tree-tree edges
  }
  
  //This gives us the first result
  cout << uf.get_max() << " ";
  
  UnionFind uf2(n); //Initiate a second UF
  
  //Sort the edges (shortest to longest)
  sort(bone_edges.begin(), bone_edges.end());
  sort(tree_edges.begin(), tree_edges.end());
  
  //Progressively add edges (tree-tree or bone-tree) in the UF (whatever comes first in term of length)
  //Stops when enough bones
  K::FT max_q = 0;
  int i = 0; int j = 0;
  int n1 = bone_edges.size();
  int n2 = tree_edges.size();
  while(uf2.max_bone<k){
    //If all bones already added to their trees
    if(i==n1){
      uf2.Union(tree_edges[j].second.first, tree_edges[j].second.second);
      max_q = tree_edges[j].first;
      j++;
    //If all trees already linked together
    }else if(j==n2){
      uf2.add_bone(bone_edges[i].second);
      max_q = bone_edges[i].first*4;
      i++;
    //Else find the shortest between next tree-tree edge and next bone-tree edge
    }else{
      if(bone_edges[i].first*4<tree_edges[j].first){
        uf2.add_bone(bone_edges[i].second);
        max_q = bone_edges[i].first*4;
        i++;
      }else{
        uf2.Union(tree_edges[j].second.first, tree_edges[j].second.second);
        max_q = tree_edges[j].first;
        j++;
      }
    }
  }
  
  //This gives us the second result
  cout << max_q << endl;
}

int main(){
  cout << fixed << setprecision(0);
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}