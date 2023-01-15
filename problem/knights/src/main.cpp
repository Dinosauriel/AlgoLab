#include <iostream>
#include <vector>
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
  int m, n, k, c;
  
  std::cin >> m;
  std::cin >> n;
  std::cin >> k;
  std::cin >> c;
  
  // std::cout << "(m, n, k, c) = (" << m << ", " << n << ", " << k << ", " << c << ")" << std::endl;
  
  std::vector<int> K_r(k);
  std::vector<int> K_c(k);
  
  for (int i = 0; i < k; ++i) {
    std::cin >> K_c[i];
    std::cin >> K_r[i];
    
    // std::cout << "knight (" << K_c[i] << ", " << K_r[i] << ")" << std::endl;
  }
  
  int n_vertices = (2 * n * m) + 2;
  Graph g(n_vertices);
  
  int source = n_vertices - 2;
  int sink = n_vertices - 1;
  
  // std::cout << "(source, sink) = (" << source << ", " << sink << ")" << std::endl; 
  
  for (int row = 0; row < n; ++row) {
    for (int col = 0; col < m; ++col) {
      int self_in = 2 * coords_i(m, row, col);
      int self_out = self_in + 1;
      
      add_edge(g, self_in, self_out, c);
      
      int north_in = 2 * coords_i(m, row - 1, col);
      int east_in = 2 * coords_i(m, row, col + 1);
      int south_in = 2 * coords_i(m, row + 1, col);
      int west_in = 2 * coords_i(m, row, col - 1);

      if (row == 0) {
        north_in = sink;
      }
      if (row == n - 1) {
        south_in = sink;
      }
      if (col == 0) {
        west_in = sink;
      }
      if (col == m - 1) {
        east_in = sink;
      }
      
      add_edge(g, self_out, north_in, 1);
      add_edge(g, self_out, east_in, 1);
      add_edge(g, self_out, south_in, 1);
      add_edge(g, self_out, west_in, 1);
    }
  }
  
  for (int i = 0; i < k; ++i) {
    int start = 2 * coords_i(m, K_r[i], K_c[i]);
    add_edge(g, source, start, 1);
  }

  long flow = boost::push_relabel_max_flow(g, source, sink);
  std::cout << flow << std::endl;
  
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
