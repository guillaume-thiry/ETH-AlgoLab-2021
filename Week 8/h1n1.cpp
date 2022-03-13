// 1. Do a triangulation with all the infected people as vertices
// 2. For each face of the triangulation, store as info the max escape radius (under which you can escape everything while starting from this face)
// 3. Explore the faces using a priority queue while updating the faces info
// 4. For each escaping person, check if initial position is safe (else, already too late)
// 5. Then find the face in which the person is, and check if the radius of the person is below the escaping radius of the face

#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::All_faces_iterator Face_iterator;

using namespace std;

//To explore the faces, we start at the infinite faces (which are the exit ones)
//Then we check their neighbors: they can be escaped if the radius is small enough to move to infinite neighbors
//Therefore, the escaping radius is given by the border length (we can go from this face to the infinite neighbor by passing between these 2 infected persons)
//Then, we can propagate this to their neighbors...

//If face1 has escaping radius of r1 and face2 escaping radius of r2 and the passing radius between them is R,
//then if r2 < min(r1,R), it can be updated: first you go to face1 passing with radius R and start the escape at face1
//the new value for face2 is then min(r1,R)

//To do that, we use a priority queue to update as quickly as possible using the biggest values
//Else, it could take a very long time to propagate entirely

void explore(Triangulation& t){
  priority_queue<pair<K::FT, Triangulation::Face_handle>> q; //Priority queue
  for(Face_iterator f = t.all_faces_begin(); f!=t.all_faces_end(); f++){
    if(t.is_infinite(f)){ //Infinite face
      for(int i=0; i<3; i++){
        if(!t.is_infinite(f->neighbor(i))){
          Triangulation::Vertex_handle v1 = f->vertex((i+1)%3);
          Triangulation::Vertex_handle v2 = f->vertex((i+2)%3);
          K::FT d = CGAL::squared_distance(v1->point(), v2->point()); //Passing radius between the two faces
          q.push(make_pair(d, f->neighbor(i))); //Add the face to the queue for an update
        }
      }
    }
  }
  
  while(!q.empty()){
    //Take out top element
    K::FT d = q.top().first;
    Triangulation::Face_handle f = q.top().second;
    q.pop();
    
    //If new proposed value is better, update it
    if(d>f->info()){
      f->info() = d;
      //And only then, spread to neighbors
      for(int i=0; i<3; i++){
        if(!t.is_infinite(f->neighbor(i))){
          Triangulation::Vertex_handle v1 = f->vertex((i+1)%3);
          Triangulation::Vertex_handle v2 = f->vertex((i+2)%3);
          K::FT new_d = CGAL::squared_distance(v1->point(), v2->point()); //passing radius between faces
          K::FT final = min(new_d, d);  //new possible value for neighbor
          q.push(make_pair(final, f->neighbor(i))); //You could already here if the new value if better for the neighbor, but it can also be checked in the queue
        }
      }
    }
  }
  //Once the queue is empty, no more updates
}

void testcase(int n){
  //INPUT
  vector<K::Point_2> points;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    points.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(points.begin(), points.end());
  
  //Init of the faces info
  for(Face_iterator f = t.all_faces_begin(); f!=t.all_faces_end(); f++){
    f->info() = -1;
  }
  
  //Update the faces info
  explore(t);
  
  int m; cin >> m;
  for(int j=0; j<m; j++){
    double x, y; cin >> x >> y;
    double r; cin >> r;
    
    //If nearest point is closer than r, person already infected 
    K::Point_2 p(x,y);
    Triangulation::Vertex_handle v = t.nearest_vertex(p);
    if(CGAL::squared_distance(p,v->point())<r){
      cout << 'n';
    }else{
      //Else find the starting face
      Triangulation::Face_handle f = t.locate(p);
      if(f->info()==-1){  //If infinite face, already out
        cout << 'y';
      }else{
        //Else check with face info if escape is possible with that radius
        if(f->info()>=4*r){
          cout << 'y';
        }else{
          cout << 'n';
        }
      }
    }
  }
  cout << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n!=0){
    testcase(n);
    cin >> n;
  }
}