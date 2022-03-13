// This problem is quite similar to H1N1, see more details in the code there
// 1. Triangulation with the trees, store the escaping radius for each face in info
// 2. Update the escaping radius of the faces using a priority queue
// 3. Final decision
// For some reasons, this problem requires the use of EPEC kernel to calculate everything properly (usually for Triangulation problems, EPIC is enough)

#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::All_faces_iterator Face_iterator;

using namespace std;

//This is really similar to code in H1N1, more details there
void explore(Triangulation& t){
  priority_queue<pair<K::FT, Triangulation::Face_handle>> q;
  for(Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
    if(t.is_infinite(f)){
      for(int k=0; k<3; k++){
        Triangulation::Face_handle f2 = f->neighbor(k);
        if(!t.is_infinite(f2)){
          K::FT d = CGAL::squared_distance(f->vertex((k+1)%3)->point(), f->vertex((k+2)%3)->point());
          q.push(make_pair(d, f2));
        }
      }
    }else{
      //The difference in this problem is that it is also possible to escape directly from the face (and not just by moving to infinite face)
      //So the escaping value can also be changed depending on the face circumcircle, that we calculate here
      auto c = CGAL::circumcenter(f->vertex(0)->point(),f->vertex(1)->point(),f->vertex(2)->point());
      K::FT d = CGAL::squared_distance(c, f->vertex(0)->point());
      q.push(make_pair(d, f));
    }
  }
  
  //Everthing is the same as in H1N1 here
  while(!q.empty()){
    K::FT d = q.top().first;
    Triangulation::Face_handle f = q.top().second;
    q.pop();
    
    if(d>f->info()){
      f->info() = d;
      
      for(int k=0; k<3; k++){
        Triangulation::Face_handle f2 = f->neighbor(k);
        if(!t.is_infinite(f2)){
          K::FT d2 = CGAL::squared_distance(f->vertex((k+1)%3)->point(), f->vertex((k+2)%3)->point());
          q.push(make_pair(min(d,d2), f2));
        }
      }
    }
  }
}

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  K::FT r; cin >> r;
  vector<K::Point_2> trees;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    trees.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  //Init of the faces info
  for(Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
    f->info() = -1;
  }
  
  //Update of the faces info
  explore(t);
  
  for(int j=0; j<m; j++){
    long x, y; cin >> x >> y;
    K::FT s; cin >> s;
    K::Point_2 p(x,y);
    
    Triangulation::Vertex_handle v = t.nearest_vertex(p);
    Triangulation::Face_handle f = t.locate(p);
    
    K::FT l = r+s;
    //If initial position is too close, then not possible
    if(CGAL::squared_distance(v->point(), p)<l*l){
      cout << 'n';
    }else{
      //If infinite face, always working
      if(f->info()==-1){
        cout << 'y';
      }else{
        //Else, look at the face info
        if(f->info()<4*l*l){
          cout << 'n';
        }else{
          cout << 'y';
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