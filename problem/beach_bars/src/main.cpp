#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <algorithm>

void testcase() {
  int n;
  std::cin >> n;
  std::vector<int> parasols = {};
  parasols.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    int v;
    std::cin >> v;
    parasols.push_back(v);
  }
  
  std::sort(parasols.begin(), parasols.end());
  
  std::deque<int> candidates = {};
  int best_parasols = 0;
  int best_radius = INT_MAX;
  
  
  int j = 0;
  for (int i = 0; i < n; ++i) {
    
    while (j < n && parasols[j] - parasols[i] <= 200) {
      ++j;
    }
    
    int n_parasols = j - i;
    int sector_length = parasols[j - 1] - parasols[i] + 1;
    int radius = sector_length / 2;

    if (n_parasols == best_parasols) {
      
      if (radius == best_radius) {

        if (sector_length % 2 == 0) {
          candidates.push_back(parasols[i] + radius - 1);
        }
        candidates.push_back(parasols[i] + radius);
        
      } else if (radius < best_radius) {
        best_radius = radius;
        candidates.clear();

        if (sector_length % 2 == 0) {
          candidates.push_back(parasols[i] + radius - 1);
        }
        candidates.push_back(parasols[i] + radius);
      }
    } else if (n_parasols > best_parasols) {
      best_parasols = n_parasols;
      best_radius = radius;
      candidates.clear();
      
      if (sector_length % 2 == 0) {
        candidates.push_back(parasols[i] + radius - 1);
      }
      candidates.push_back(parasols[i] + radius);
    }
  }

  std::cout << best_parasols << " " << best_radius << std::endl;
  for (int i = 0; (unsigned int) i < candidates.size(); ++i) {
    std::cout << candidates[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}