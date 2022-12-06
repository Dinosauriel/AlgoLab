#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>> Graph;

typedef Traits::vertex_descriptor vertex_desc;
typedef Traits::edge_descriptor edge_desc;

void add_edge(Graph & g, int from, int to, long capacity) {
  auto c_map = boost::get(boost::edge_capacity, g);
  auto r_map = boost::get(boost::edge_reverse, g);
  const auto e = boost::add_edge(from, to, g).first;
  const auto rev_e = boost::add_edge(to, from, g).first;
  c_map[e] = capacity;
  c_map[rev_e] = 0; // reverse edge has no capacity!
  r_map[e] = rev_e;
  r_map[rev_e] = e;
}

int coords_i(int m, int row, int col) {
  return (row * m) + col;
}

void test_case() {
  int n, m;
  
  std::cin >> n;
  std::cin >> m;
  
  // std::cout << "(m, n, k, c) = (" << m << ", " << n << ", " << k << ", " << c << ")" << std::endl;
  
  
  Graph g(n);
  
  for (int i = 0; i < m; ++i) {
    int a, b, c;
    
    std::cin >> a;
    std::cin >> b;
    std::cin >> c;
    // std::cout << "knight (" << K_c[i] << ", " << K_r[i] << ")" << std::endl;
    add_edge(g, a, b, c);
  }
  
  // std::cout << "(source, sink) = (" << source << ", " << sink << ")" << std::endl; 
  
  long best_flow = LONG_MAX;
  int source = 0;
  for (int sink = 1; sink < n; ++sink) {
    long flow = boost::push_relabel_max_flow(g, source, sink);
    // std::cout << flow << ", ";
    best_flow = std::min(best_flow, flow);
    // same, but reverse
    flow = boost::push_relabel_max_flow(g, sink, source);
    // std::cout << flow << std::endl;
    best_flow = std::min(best_flow, flow);
  }
  
  std::cout << best_flow << std::endl;

  // // Retrieve the capacity map and reverse capacity map
  // const auto c_map = boost::get(boost::edge_capacity, g);
  // const auto rc_map = boost::get(boost::edge_residual_capacity, g);
  
  // // Iterate over all the edges to print the flow along them
  // auto edge_iters = boost::edges(g);
  // for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
  //   const edge_desc edge = *edge_it;
  //   const long flow_through_edge = c_map[edge] - rc_map[edge];
  //   std::cout << "edge from " << boost::source(edge, g) << " to " << boost::target(edge, g)
  //             << " runs " << flow_through_edge
  //             << " units of flow (negative for reverse direction). \n";
  // }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}
