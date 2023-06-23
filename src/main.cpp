#include "core.hpp"

int main() {
  // read data
  auto gr = read_graph();
  auto components = read_components();
  // Floyd algorithm to find dist between every pair
  floyd(gr);
  // count vertex degrees
  auto degrees = get_degrees(gr.size(), components);
  // find (c-1)
  auto components_vertex = get_components_vertex(components);
  int ans = INF;
  std::vector<std::pair<int, int>> res(components_vertex.size());
  choose(components_vertex, gr, 1, res, degrees, ans);
  // add initial
  for (const auto& c : components) {
    for (auto [u, v] : c) {
      ans += gr[u][v];
    }
  }
  std::cout << ans;
  return 0;
}
