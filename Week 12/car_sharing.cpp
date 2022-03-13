// 1. Model the problem with a graph:
// - Each booking request is a node
// - Two bookings starting from the same station will be linked (the soonest pointing to the latest)
// - A booking arriving at station i will be linked to the bookings starting at i that begin after the arrival time of the car
// - The first bookings for each stations are linked to a start node (with capacity the number of cars in the stations at time 0)
// - The bookings linked to nothing (because no other booking can start after) are linked to the end node
// - Finally all the edges have costs representing the time/money lost along this edge (difference between the two departure time) minus some potential revenue

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,boost::property<boost::edge_capacity_t, long,boost::property<boost::edge_residual_capacity_t, long,boost::property<boost::edge_reverse_t, traits::edge_descriptor,boost::property <boost::edge_weight_t, long> >>>> graph; 

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;

typedef tuple<int,int,int,int> elt;

bool comp(elt a, elt b){
  return get<1>(a) < get<1>(b);
}

void testcase(){
  //INPUT
  int n, s; cin >> n >> s;
  vector<int> stations;
  for(int i=0; i<s; i++){
    int x; cin >> x;
    stations.push_back(x);
  }
  //We store the data in vector of vectors
  //To get all the bookings departing from station i (whatever the arrival is)
  //And all the bookings arriving at station i (whatever the departure is)
  vector<vector<pair<int,int>>> departures(s);
  vector<vector<elt>> arrivals(n);
  
  int maxt = 0;
  for(int i=0; i<n; i++){
    int si, ti, di, ai, pi; cin >> si >> ti >> di >> ai >> pi;
    departures[si-1].push_back(make_pair(di, i));
    elt e(di, ai, pi, i);
    arrivals[ti-1].push_back(e);
    maxt = max(maxt, ai); //Maximum time (ie last arrival in the whole graph)
  }
  
  //Graph
  graph G(n+2);
  edge_adder adder(G);
  
  int maxc = 1000; //Max number of cars
  
  int source = n;
  int target = n+1;
  
  //Edges
  //Every minute used costs 100, that way even we substracting the revenues (negative costs) we always have positive costs
  //And that way, we don't have to care about the number of edges taken in a path (for the shifting back of the costs)
  //Because all the cars that will arrived at the end (= flow) will have spent the same amount of time in the graph (so the same cost 100*time)
  //And the difference with the computed cost will be all the revenues
  for(int k=0; k<s; k++){
    sort(departures[k].begin(), departures[k].end()); //Sort this by departure time
    sort(arrivals[k].begin(), arrivals[k].end(), comp); //And this by arrival time
    
    int ld = departures[k].size();
    int la = arrivals[k].size();
    
    //In the same station, departing bookings are linked together (in the form of a linked chain)
    //Meaning that if you don't choose a certain booking, you can just jump to the next departing one (at the same station)
    //With a loss a time corresponding to the difference in departure times
    if(ld>0){
      adder.add_edge(source, departures[k][0].second, stations[k], 100*departures[k][0].first);//First one is linked to the start with capacity = number of cars in station k
      adder.add_edge(departures[k][ld-1].second, target, maxc, 100*(maxt-departures[k][ld-1].first));//Last one linked to the end (don't forget remaining time)
      for(int j=0; j<ld-1; j++){
        adder.add_edge(departures[k][j].second, departures[k][j+1].second, maxc, 100*(departures[k][j+1].first-departures[k][j].first));//Link a departure with the next one (with cost 100 for every minute lost)
      }
    }
    //Then we link bookings from different stations
    //We have arrivals[k] sorted by arrival time and each element in it is a booking arriving at station k
    //And they all need to be linked to something (because the booking can be chosen)
    //So we link them to departures at station k (or the end if no departure left)
    //Therefore, we explore arrivals[k] by increasing arrival time and departure[k] by increasing departure time
    //And link every booking from arrivals to the first next departing booking (that leave after the arrival time of course)
    //We don't need to link it to the other later departures because departures are already linked together in a station (done just before)
    int i=0; int j=0;
    while(i<la){//We need to take care of all of them
      if(j<ld){
        if(get<1>(arrivals[k][i])>departures[k][j].first){//While the departure leaves too early, go to the next one
          j++;
        }else{
          adder.add_edge(get<3>(arrivals[k][i]), departures[k][j].second, 1, 100*(departures[k][j].first-get<0>(arrivals[k][i]))-get<2>(arrivals[k][i])); //if this one is good, add the edge and go to the next arrival
          //Do not forget to substract the revenue in the cost: c = 100*(next_booking_departure - this_booking_departure) - revenue_for_this_booking
          i++;
        }
      }else{
        adder.add_edge(get<3>(arrivals[k][i]), target, 1, 100*(maxt-get<0>(arrivals[k][i]))-get<2>(arrivals[k][i])); //If no more departures, link to the end (with remaining time)
        i++;
      }
    }
  }
  
  //Flow + Cost + OUTPUT
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  long cost = boost::find_flow_cost(G);
  long flow = boost::push_relabel_max_flow(G, source, target);
  
  cout << flow*maxt*100-cost << endl; //For all the cars (= flow), they spent a time max_t at cost 100, the difference being the revenues earned along the way
  
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--){
    testcase();
  }
}