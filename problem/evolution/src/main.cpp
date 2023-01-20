#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
// Given a species and an age, find the oldest
// ancestor of this species which is not older than the specified age.
struct query {
  int i;
  int age;
};

int layer_anc(int species,
      std::vector<int> const& layer,
      std::vector<int> const& topo_a) {
  // find the ancestor of species inside this layer
  // std::cout << "finding " << topo_a[species] << " in [";
  // for (auto it = layer.begin(); it != layer.end(); ++it) {
  //   std::cout << topo_a[*it] << " ";
  // }
  // std::cout << "]" << std::endl;
  
  int lo = 0;
  int up = layer.size();
  
  while (lo + 1 != up) {
    int mid = (lo + up) / 2;
    
    if (topo_a[layer[mid]] > topo_a[species]) {
      up = mid;
    } else {
      lo = mid;
    }
  }
  
  // std::cout << "returning " << layer[lo] << std::endl;
  
  return layer[lo];
}

int dfs(int i, int d, int t, 
      std::vector<std::set<int>> const& children,
      std::vector<int> & depth,
      std::vector<int> & topo_a,
      std::vector<int> & topo_order) {
  topo_order.push_back(i);
  depth[i] = d;
  topo_a[i] = t;
  
  for (auto c_i = children[i].begin(); c_i != children[i].end(); ++c_i) {
    t = dfs(*c_i, d + 1, t + 1, children, depth, topo_a, topo_order);
  }
  return t;
}

void testcase() {
  int n, q;
  std::cin >> n >> q;
  
  std::map<std::string, int> spec_i;
  std::vector<std::string> spec_name(n);
  std::vector<int> age(n);
  
  for (int i = 0; i < n; ++i) {
    std::string name;
    int a;
    std::cin >> name >> a;
    spec_i.insert(std::make_pair(name, i));
    spec_name[i] = name;
    age[i] = a;
  }
  
  std::vector<int> parent(n, -1);
  std::vector<std::set<int>> children(n);
  
  for (int i = 0; i < n - 1; ++i) {
    std::string s, p;
    std::cin >> s >> p;
    parent[spec_i[s]] = spec_i[p];
    children[spec_i[p]].insert(spec_i[s]);
  }
  
  
  // find LUCA (= root)
  int LUCA = -1;
  for (int i = 0; i < n; ++i) {
    if (parent[i] == -1)
      LUCA = i;
  }

  // depth of the species in the tree of life
  std::vector<int> depth(n, 0);
  // topological index of the species
  std::vector<int> topo_a(n);
  // topological ordering of the species
  std::vector<int> topo_order(0);
  topo_order.reserve(n);
  dfs(LUCA, 0, 0, children, depth, topo_a, topo_order);
  
  int max_d = *std::max_element(depth.begin(), depth.end());

  // fill up layers (in topological ordering!)
  std::vector<std::vector<int>> layers(max_d + 1);
  for (int i = 0; i < n; ++i) {
    int species_i = topo_order[i];
    layers[depth[species_i]].push_back(species_i);
  }

  std::vector<struct query> Q(q);
  for (int i = 0; i < q; ++i) {
    std::string s;
    int b;
    std::cin >> s >> b;
    Q[i].age = b;
    Q[i].i = spec_i[s];
  }

  for (int i = 0; i < q; ++i) {
    int species = Q[i].i;
    int target_age = Q[i].age;
    
    int lo_d = 0;
    int up_d = depth[species];
    
    
    while (lo_d != up_d) {
      int mid_d = (lo_d + up_d) / 2;
      
      int anc = layer_anc(species, layers[mid_d], topo_a);
      // std::cout << "(" << lo_d << ", " << up_d << ") -> mid_d: " << mid_d << std::endl;

      if (age[anc] <= target_age) {
        up_d = mid_d;
      } else {
        // ancestor is too old
        lo_d = mid_d + 1;
      }
    }

    int anc = layer_anc(species, layers[lo_d], topo_a);
    std::cout << spec_name[anc] << " ";
  }
  std::cout << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false); // Always!
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}