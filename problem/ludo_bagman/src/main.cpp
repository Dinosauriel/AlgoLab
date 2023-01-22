#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator


void add_edge(int from, int to, long capacity, long cost, graph &G) {
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


void testcase() {
  int e, w, m, d;
  std::cin >> e >> w >> m >> d;
  
  int p, l;
  std::cin >> p >> l;
  
  // std::cout << "e, w, m, d: " << e << ", " << w << ", " << m << ", " << d << std::endl;
  
  std::vector<int> nondiff_ed_u(m);
  std::vector<int> nondiff_ed_v(m);
  std::vector<int> nondiff_ed_r(m);
  
  std::vector<int> diff_ed_u(d);
  std::vector<int> diff_ed_v(d);
  std::vector<int> diff_ed_r(d);
  
  for (int i = 0; i < m; ++i) {
    int u, v, r;
    std::cin >> u >> v >> r;
    nondiff_ed_u[i] = u;
    nondiff_ed_v[i] = v;
    nondiff_ed_r[i] = r;
  }
  
  for (int i = 0; i < d; ++i) {
    int u, v, r;
    std::cin >> u >> v >> r;
    diff_ed_u[i] = u;
    diff_ed_v[i] = v;
    diff_ed_r[i] = r;
  }
  
  if (p < std::max(e, w) * l) {
    std::cout << "No schedule!" << std::endl;
    return;
  }
  
  graph G(2 * e + 2 * w + 6);
  auto c_map = boost::get(boost::edge_capacity, G);
  // auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  int source = 2 * e + 2 * w;
  int sink = source + 1;
  
  int e_oblig = sink + 1;
  int e_optio = sink + 2;
  int w_oblig = sink + 3;
  int w_optio = sink + 4;
  
  add_edge(source, e_oblig, l * e, 0, G);
  add_edge(source, e_optio, p - (l * e), 0, G);
  
  add_edge(w_oblig, sink, l * w, 0, G);
  add_edge(w_optio, sink, p - (l * w), 0, G);
  
  int e_nondiff = 0;
  int e_diff = e;
  int w_nondiff = 2 * e;
  int w_diff = 2 * e + w;
  
  for (int i = 0; i < m; ++i) {
    add_edge(e_nondiff + nondiff_ed_u[i], w_nondiff + nondiff_ed_v[i], 1, nondiff_ed_r[i], G);
  }
  
  for (int i = 0; i < d; ++i) {
    add_edge(e_diff + diff_ed_u[i], w_diff + diff_ed_v[i], 1, diff_ed_r[i], G);
  }
  
  for (int i = 0; i < e; ++i) {
    add_edge(e_oblig, e_nondiff + i, l, 0, G);
    add_edge(e_optio, e_nondiff + i, p, 0, G);
    add_edge(e_optio, e_diff + i,    p, 0, G);
  }
  
  for (int i = 0; i < w; ++i) {
    add_edge(w_nondiff + i, w_oblig, l, 0, G);
    add_edge(w_nondiff + i, w_optio, p, 0, G);
    add_edge(w_diff + i,    w_optio, p, 0, G);
  }
  
  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  
  // Iterate over all edges leaving the source to sum up the flow values.
  int flow = 0;
  out_edge_it e_it, eend;
  for(boost::tie(e_it, eend) = boost::out_edges(boost::vertex(source, G), G); e_it != eend; ++e_it) {
      // std::cout << "edge from " << boost::source(*e_it, G) << " to " << boost::target(*e_it, G) 
      //     << " with capacity " << c_map[*e_it] << " and residual capacity " << rc_map[*e_it] << std::endl;
      flow += c_map[*e_it] - rc_map[*e_it];     
  }
  // std::cout << "s-out flow " << flow << std::endl;
  
  if (flow < p) {
    std::cout << "No schedule!" << std::endl;
    return;
  }
  
  std::cout << cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false); // Always!
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}