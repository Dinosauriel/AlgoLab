#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graph_utility.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;

struct score_pair {
  int id;
  int score;
};

bool score_pair_less_than(struct score_pair const& a, struct score_pair const& b) {
  return a.score < b.score;
}

bool intersect(std::set<int> const& a, std::set<int> const& b) {
  auto larger = a;
  auto smaller = b;
  if (b.size() > a.size()) {
    larger = b;
    smaller = a;
  }
  for (auto it = smaller.begin(); it != smaller.end(); ++it) {
    if (larger.contains(*it))
      return true;
  }
  return false;
}

void compute_subset(int root, std::vector<std::set<int>> & subset, std::vector<bool> & visited, Graph const& g) {
  if (visited[root]) {
    return;
  }

  visited[root] = true;
  subset[root].insert(root);
  auto neigh = boost::adjacent_vertices(root, g);
  for (auto neigh_it = neigh.first; neigh_it != neigh.second; ++neigh_it) {
    compute_subset(*neigh_it, subset, visited, g);
    subset[root].insert(subset[*neigh_it].begin(), subset[*neigh_it].end());
  }
}

// int get_subset_score(int root, std::vector<int> subset_score, std::vector<int> comp_score, std::vector<bool> visited, Graph const& g) {
//   if (visited[root]) {
//     return subset_score[root];
//   }

//   std::vector<bool> counted(visited.size());

//   visited[root] = true;
//   auto neigh = boost::adjacent_vertices(root, g);
//   subset_score[root] += comp_score[root];
//   counted[root] = true;
//   for (auto neigh_it = neigh.first; neigh_it != neigh.second; ++neigh_it) {
//     if (!counted[*neigh_it]) {
//       subset_score[root] += get_subset_score(*neigh_it, subset_score, comp_score, visited, g);
//       counted[*neigh_it] = true;
//     }
//   }
//   return subset_score[root];
// }

void test_case() {
  int n, m;
  
  std::cin >> n;
  std::cin >> m;
  
  std::vector<int> conv_score(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> conv_score[i];
    // std::cout << conv_score[i] << " ";
  }
  
  Graph g(n);
  
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u;
    std::cin >> v;
    boost::add_edge(u, v, g);
  }
  
  // std::vector<int> conn_comp(n);

  // int n_conn_comp = boost::strong_components(g, boost::make_iterator_property_map(conn_comp.begin(), boost::get(boost::vertex_index, g)));

  // std::cout << "there are " << n_conn_comp << " strongly connected components" << std::endl;

  // for (int i = 0; i < n; ++i) {
  //   std::cout << i << " " << conn_comp[i] << std::endl;
  // }

  // // determine root for each comp
  // std::vector<int> root(n_conn_comp, -1);
  // std::vector<int> comp_score(n_conn_comp);

  // for (int i = 0; i < n; ++i) {
  //   int comp = conn_comp[i];
  //   comp_score[comp] += conv_score[i];
  //   if (root[comp] == -1) {
  //     root[comp] = i;
  //   }
  // }

  // Graph g_acyclic(n_conn_comp);

  // auto edges = boost::edges(g);
  // for (auto edge_it = edges.first; edge_it != edges.second; ++edge_it) {
  //   auto u = boost::source(*edge_it, g);
  //   auto v = boost::target(*edge_it, g);

  //   int new_u = root[conn_comp[u]];
  //   int new_v = root[conn_comp[v]];
  //   boost::add_edge(new_u, new_v, g_acyclic);
  // }

  std::vector<std::set<int>> subsets(n);
  std::vector<bool> visited(n);
  std::vector<struct score_pair> subset_score(n, {0, 0});

  for (int i = 0; i < n; ++i) {
    compute_subset(i, subsets, visited, g);
    subset_score[i].id = i;
    std::cout << "subsets[" << i << "]: {";
    for (auto v_it = subsets[i].begin(); v_it != subsets[i].end(); ++v_it) {
      std::cout << *v_it << " ";
      subset_score[i].score += conv_score[*v_it];
    }
    std::cout << "}, score: " << subset_score[i].score << std::endl;
  }

  std::sort(subset_score.rbegin(), subset_score.rend(), score_pair_less_than);

  std::set<int> best_subset;
  int score = 0;
  for (int i = 0; i < n; ++i) {
    std::cout << "checking subset " << subset_score[i].id << std::endl;

    if (subset_score[i].score <= 0) {
      break;
    }

    int v = subset_score[i].id;
    if (intersect(best_subset, subsets[v])) {
      continue;
    }

    score += subset_score[i].score;
    best_subset.insert(subsets[v].begin(), subsets[v].end());
  }

  if (score == 0) {
    std::cout << "impossible" << std::endl;
    return;
  }

  std::cout << score << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}
