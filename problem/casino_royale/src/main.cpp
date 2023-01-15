#include <iostream>
#include <vector>
#include <algorithm>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;

struct mission {
  int x;
  int y;
  int q;
  // how many qs are missed by taking this mission
  int opportunity_cost;
};

bool mission_less_than(struct mission const&  a, struct mission const& b) {
  return a.x < b.x;
}

edge_desc add_edge(graph &G, int from, int to, long capacity, long cost) {
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

  return e;
}

void testcase() {
  int n, m, l;
  std::cin >> n >> m >> l;
  
  int q_max = 0;
  std::vector<struct mission> missions(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> missions[i].x >> missions[i].y >> missions[i].q;
    q_max = std::max(missions[i].q, q_max);
  }

  std::sort(missions.begin(), missions.end(), mission_less_than);

  std::vector<int> opportunity_cost(n, 0);

  int m_i = 0;
  for (int i = 0; i < n - 1; ++i) {
    opportunity_cost[i + 1] = opportunity_cost[i];

    while (m_i < m && missions[m_i].x == i) {
      opportunity_cost[i + 1] += missions[m_i].q;
      ++m_i;
    }
  }

  for (int i = 0; i < m; ++i) {
    missions[i].opportunity_cost = opportunity_cost[missions[i].y] - opportunity_cost[missions[i].x] - missions[i].q;
  }

  graph G(n + 2);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int source = n;
  int sink = n + 1;
  
  add_edge(G, source, 0, l, 0);
  add_edge(G, n - 1, sink, l, 0);
  
  // add "dont take" edges
  for (int station = 0; station < n - 1; ++station) {
    add_edge(G, station, station + 1, l, opportunity_cost[station + 1] - opportunity_cost[station]);
    // std::cout << "station " << station << " has opportunity cost " << opportunity_cost[station + 1] - opportunity_cost[station] << std::endl;
  }

  // add "take" edges
  std::vector<edge_desc> m_edges;
  m_edges.reserve(m);
  
  for (int i = 0; i < m; ++i) {
    struct mission miss = missions[i];
    auto e = add_edge(G, miss.x, miss.y, 1, miss.opportunity_cost);
    m_edges.push_back(e);
    // std::cout << "mission (" << miss.x << ", " << miss.y << ", " << miss.q << ") has opportunity cost " << miss.opportunity_cost << std::endl;
  }

  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  long cost = boost::find_flow_cost(G);

  // std::cout << "cost: " << cost << std::endl;

  long result = 0;
  for (int i = 0; i < m; ++i) {
    if (rc_map[m_edges[i]] == 0) {
      // this mission was taken
      result += missions[i].q;
      // std::cout << "mission (" << missions[i].x << ", " << missions[i].y << ", " << missions[i].q << ") taken!" << std::endl;
    }
  }

  std::cout << result << std::endl;
}


int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i)
        testcase(); // Solve a particular test case
}