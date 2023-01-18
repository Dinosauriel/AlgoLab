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

int dpth(int i, int d, std::vector<int> & depth, std::vector<std::set<int>> const& children) {
  depth[i] = d;
  int max_d = d;
  for (auto c_i = children[i].begin(); c_i != children[i].end(); ++c_i) {
    max_d = std::max(max_d, dpth(*c_i, d + 1, depth, children));
  }
  return max_d;
}

void dfs(int i, int d, int k, 
      std::vector<int> & depth, std::vector<std::set<int>> const& children, 
      std::vector<int> & path, std::vector<int> & chunk_path, 
      std::vector<std::vector<uint16_t>> & anc_imm, std::vector<std::vector<uint16_t>> & anc_chunk) {
  path.push_back(i);
  if (depth[i] % k == 0) {
    chunk_path.push_back(i);
  }

  for (size_t j = path.size(); j > 0; --j) {
    anc_imm[i].push_back((uint16_t) path[j - 1]);
    if (path[j - 1] != i && depth[path[j - 1]] % k == 0) {
      break;
    }
  }

  for (size_t j = chunk_path.size(); j > 0; --j) {
    anc_chunk[i].push_back((uint16_t) chunk_path[j - 1]);
  }


  for (auto c_i = children[i].begin(); c_i != children[i].end(); ++c_i) {
    dfs(*c_i, d + 1, k, depth, children, path, chunk_path, anc_imm, anc_chunk);
  }

  path.pop_back();
  if (depth[i] % k == 0) {
    chunk_path.pop_back();
  }
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
  std::vector<int> depth(n, 0);
  int max_d = dpth(LUCA, 0, depth, children);
  
  std::vector<std::vector<uint16_t>> anc_chunk(n, std::vector<uint16_t>(0));
  std::vector<std::vector<uint16_t>> anc_imm(n, std::vector<uint16_t>(0));

  // k = chunk size
  int k = (int) std::sqrt(max_d);
  std::vector<int> path(0);
  std::vector<int> chunk_path(0);
  dfs(LUCA, 0, k, depth, children, path, chunk_path, anc_imm, anc_chunk);

  std::vector<struct query> Q(q);
  for (int i = 0; i < q; ++i) {
    std::string s;
    int b;
    std::cin >> s >> b;
    Q[i].age = b;
    Q[i].i = spec_i[s];
  }

  // for (int i = 0; i < n; ++i) {
  //   std::cout << "anc_imm[" << i << "]: ";
  //   for (size_t j = 0; j < anc_imm[i].size(); ++j) {
  //     std::cout << anc_imm[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }

  // for (int i = 0; i < n; ++i) {
  //   std::cout << "anc_chunk[" << i << "]: ";
  //   for (size_t j = 0; j < anc_chunk[i].size(); ++j) {
  //     std::cout << anc_chunk[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }

  for (int i = 0; i < q; ++i) {
    int species = Q[i].i;
    int target_age = Q[i].age;

    // first search chunk ancestry
    auto chunk_it = std::upper_bound(anc_chunk[species].begin(), anc_chunk[species].end(), target_age,
      [age] (int target_age, uint16_t v) {
        return target_age < age[v];
      });
    int chunk_i = chunk_it - anc_chunk[species].begin() - 1;

    std::vector<uint16_t> imm;
    if (chunk_i == -1) {
      imm = anc_imm[species];
    } else {
      imm = anc_imm[anc_chunk[species][chunk_i]];
    }
    
    // std::cout << "species " << species << " chunk_i " << chunk_i << std::endl;

    
    // then search immediate ancestry
    auto ancestor_it = std::upper_bound(imm.begin(), imm.end(), target_age,
      [age] (int target_age, uint16_t v) {
        return target_age < age[v];
      });
    int ancestor_i = ancestor_it - imm.begin() - 1;

    std::cout << spec_name[imm[ancestor_i]] << " ";
  }
  std::cout << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false); // Always!
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}