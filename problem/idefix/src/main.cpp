#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void dfs(int i, int c, K::FT s, 
      std::vector<std::vector<std::pair<int, K::FT>>> const& tree_neigh,
      std::vector<int> & component) {
  component[i] = c;
  for (size_t j = 0; j < tree_neigh[i].size(); ++j) {
    if (tree_neigh[i][j].second > s)
      // this tree is too far away
      continue;
    if (component[tree_neigh[i][j].first] != -1)
      // this tree was already visited
      continue;

    dfs(tree_neigh[i][j].first, c, s, tree_neigh, component);
  }
}


struct neigh {
  int a;
  int b;
  K::FT sq_d;
};

bool neigh_less_than(struct neigh const& a, struct neigh const& b) {
  return a.sq_d < b.sq_d;
}


int most_bones(int n, int m, K::FT s, 
        std::vector<std::vector<std::pair<int, K::FT>>> const& G, 
        std::vector<std::pair<K::Point_2, int>> const& trees,
        std::vector<K::Point_2> const& bones,
        std::vector<std::pair<int, K::FT>> nearest_tree) {

  // find connected components
  std::vector<int> tree_component(n, -1);
  int c = 0;
  for (int i = 0; i < n; ++i) {
    if (tree_component[i] == -1) {
      dfs(i, c, s, G, tree_component);
      ++c;
    }
  }

  std::vector<int> bones_per_component(c, 0);
  for (int bone = 0; bone < m; ++bone) {
    if (4 * nearest_tree[bone].second <= s) {
      bones_per_component[tree_component[nearest_tree[bone].first]] += 1;
    }
  }

  return *std::max_element(bones_per_component.begin(), bones_per_component.end());
}

void testcase() {
  int n, m, k;
  K::FT s;
  std::cin >> n >> m >> s >> k;
  
  std::vector<std::pair<K::Point_2, int>> trees;
  trees.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    trees.push_back(std::make_pair(K::Point_2(x, y), i));
  }
  
  std::vector<K::Point_2> bones;
  bones.reserve(m);
  for (int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    bones.push_back(K::Point_2(x, y));
  }
  
  std::vector<struct neigh> neighbours;
  
  Triangulation tree_triang;
  tree_triang.insert(trees.begin(), trees.end());

  // build the tree graph
  std::vector<std::vector<std::pair<int, K::FT>>> tree_neigh(n);
  
  std::set<K::FT> distances;

  for (Edge_iterator e_i = tree_triang.finite_edges_begin(); e_i != tree_triang.finite_edges_end(); ++e_i) {
    Triangulation::Edge e = *e_i;
    Triangulation::Vertex_handle v1 = e.first->vertex((e.second + 1) % 3);
    Triangulation::Vertex_handle v2 = e.first->vertex((e.second + 2) % 3);

    K::FT sq_d = CGAL::squared_distance(v1->point(), v2->point());
    tree_neigh[v1->info()].push_back(std::make_pair(v2->info(), sq_d));
    tree_neigh[v2->info()].push_back(std::make_pair(v1->info(), sq_d));
    
    distances.insert(sq_d);
  }
  
  std::vector<std::pair<int, K::FT>> nearest_tree(m);
  for (int i = 0; i < m; ++i) {
    auto tree = tree_triang.nearest_vertex(bones[i]);
    K::FT sq_d = CGAL::squared_distance(bones[i], tree->point());
    
    nearest_tree[i] = std::make_pair(tree->info(), sq_d);
    
    distances.insert(4 * sq_d);
  }
  
  int a = most_bones(n, m, s, tree_neigh, trees, bones, nearest_tree);
  
  
  std::vector<K::FT> distances_v(distances.size());
  
  int i = 0;
  for (auto d = distances.begin(); d != distances.end(); ++d) {
    distances_v[i] = *d;
    ++i;
  }
  
  int lo = 0;
  int up = distances_v.size() - 1;
  
  while (lo != up) {
    // std::cout << "(" << lo << ", " << up << ")" << std::endl;
    int di = (lo + up) / 2;
    int nb = most_bones(n, m, distances_v[di], tree_neigh, trees, bones, nearest_tree);

    if (nb >= k) {
      up = di;
    } else {
      lo = di + 1;
    }
  }
  K::FT q = distances_v[lo];
  // K::FT q = 0;
    // std::cout << "distance " << *d << " gives " << nb << " bones" << std::endl;

  // auto q_it = std::lower_bound(distances.begin(), distances.end(), k, 
  // [n, m, tree_neigh, trees, bones, nearest_tree] (int k, K::FT dist) {
  //   int nb = most_bones(n, m, dist, tree_neigh, trees, bones, nearest_tree);
  //   std::cout << "distance " << dist << " gives " << nb << " bones" << std::endl;
  //   std::cout << "i need " << k << " bones" << std::endl;
  //   return k < nb;
  // });
  // std::cout << "selected " << *q_it << std::endl;
  // K::FT q = distances[q_i];
  
  std::cout << a << " " << (long) q << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false); // Always!
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}