// 1. Using a triangulation, first check if anybody reaches the end (no lights close enough for elimination)
// 2. If yes, the winners are just the ones never eliminated
// 3. If no, then simulate the whole game to find the ones eliminated in last

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

using namespace std;

void testcase(){
  //INPUT
  int n, m; cin >> n >> m;
  vector<K::Point_2> people;
  vector<long> radius;
  for(int i=0; i<n; i++){
    long x, y; cin >> x >> y;
    long r; cin >> r;
    K::Point_2 p(x,y);
    people.push_back(p);
    radius.push_back(r);
  }
  long h; cin >> h;
  vector<K::Point_2> lights;
  for(int j=0; j<m; j++){
    long x, y; cin >> x >> y;
    K::Point_2 p(x,y);
    lights.push_back(p);
  }
  
  //Triangulation
  Triangulation t;
  t.insert(lights.begin(), lights.end());
  
  //Check if anybody reaches the end (no lights close enough)
  bool end = false;
  vector<int> winners;
  for(int i=0; i<n; i++){
    Triangulation::Vertex_handle v = t.nearest_vertex(people[i]);
    long d = (radius[i]+h)*(radius[i]+h);
    if(CGAL::squared_distance(people[i], v->point())>=d){ //If so, this person is automatically a winner
      end = true;
      winners.push_back(i);
    }
  }
  //If winners at the end, just output them here and we're done
  if(end){
    for(int k=0; k<int(winners.size()); k++){
      cout << winners[k] << " ";
    }
    cout << endl;
  //Else, nobody reaches the end and we have to simulate the whole game
  //For each candidate, go one by one through each round until elimination (which we know will happen)
  }else{
    vector<int> elim(n);  //Store the elimination round for each candidate
    int max_val=0;
    for(int i=0; i<n; i++){
      int j=0;
      long d = (radius[i]+h)*(radius[i]+h);
      while(true){
        if(CGAL::squared_distance(people[i], lights[j])<d){
          elim[i] = j;
          max_val = max(max_val,j); //Store the max elimination round
          break;
        }else{
          j++;
        } 
      }
    }
    
    //OUTPUTs the persons eliminated in last
    for(int i=0; i<n; i++){
      if(elim[i]==max_val){
        cout << i << ' ';
      }
    }
    cout << endl;
  }
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}