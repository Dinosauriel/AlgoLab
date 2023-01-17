#include <iostream>
#include <vector>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> f_traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, f_traits::edge_descriptor>>>> f_graph;



typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;

struct edge {
  bool is_lift;
  int x;
  int y;
  int z;
};

void add_edge(int from, int to, long capacity, f_graph& G) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
}

bool matching_exists(int max_distance, int a, int s, int c, int d, std::vector<std::vector<int>> const& distance) {
  f_graph flow_G(a + (c * s) + 2);
  int source = a + (c * s);
  int sink = a + (c * s) + 1;
  
  for (int i = 0; i < a; ++i) {
    add_edge(source, i, 1, flow_G);
  }
  
  for (int i = 0; i < s; ++i) {
    for (int cp = 1; cp <= c; ++cp) {
        add_edge(a + (c * i) + (cp - 1), sink, 1, flow_G);
    }
  }
  
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < s; ++j) {
        for (int cp = 1; cp <= c; ++cp) {
            if (distance[i][j] != -1 && distance[i][j] + cp * d <= max_distance) {
                add_edge(i, a + (c * j) + (cp - 1), 1, flow_G);
            }
        }
    }
  }
  
  long flow = boost::push_relabel_max_flow(flow_G, source, sink);
  return flow >= a;
}

void testcase() {
  int n, m, a, s, c, d;
  std::cin >> n >> m >> a >> s >> c >> d;
  
  std::vector<struct edge> edges(m);
  
  for (int i = 0; i < m; ++i) {
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;
    edges[i].is_lift = (w == 'L');
    edges[i].x = x;
    edges[i].y = y;
    edges[i].z = z;
  }
  
  std::vector<int> start(a);
  for (int i = 0; i < a; ++i) {
    std::cin >> start[i];
  }
  
  std::vector<int> shelters(s);
  for (int i = 0; i < s; ++i) {
    std::cin >> shelters[i];
  }
  
  graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < m; ++i) {
    edge_desc e = boost::add_edge(edges[i].x, edges[i].y, G).first;
    weights[e] = edges[i].z;
    
    if (edges[i].is_lift) {
      //also add reverse edge
      e = boost::add_edge(edges[i].y, edges[i].x, G).first;
      weights[e] = edges[i].z;
    }
  }
  
  // compute distance from each agent to each shelter
  std::vector<std::vector<int>> distance(a, std::vector<int>(s, -1));
  int max_d = 0;
  for (int i = 0; i < a; ++i) {
    std::vector<int> map(n);
    boost::dijkstra_shortest_paths(G, start[i],
      boost::distance_map(boost::make_iterator_property_map(
        map.begin(), boost::get(boost::vertex_index, G))));
        
    for (int j = 0; j < s; ++j) {
      int ds = map[shelters[j]];
      if (ds < INT_MAX) {
        distance[i][j] = ds;
        max_d = std::max(max_d, ds);
      }
    }
  }
  
  // perform binary search on distance
  // find the smallest distance that has a matching
  // distance is in [lo, up]
  int lo = 0;
  int up = max_d + 2 * d;
  while (up != lo) {
    // std::cout << "search: [" << lo << ", " << up << "]" << std::endl;
    
    int mid = (lo + up) / 2;
    
    if (matching_exists(mid, a, s, c, d, distance)) {
        // std::cout << "matching for max_dist " << mid << " exists" << std::endl;
      up = mid;
    } else {
        // std::cout << "matching for max_dist " << mid << " does not exist" << std::endl;
      lo = mid + 1;
    }
  }

  std::cout << lo << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}
