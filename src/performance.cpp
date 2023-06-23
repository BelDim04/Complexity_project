#include "core.hpp"

#include <benchmark/benchmark.h>

static void test(benchmark::State& state) {
  for (auto _ : state) {
    srand(time(0));

    int n = state.range(0);
    int p = state.range(1);
    auto gr = make_graph(n, p);
    int c = state.range(2);
    auto components = make_components(c, gr);

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
    for (const auto &c: components) {
      for (auto [u, v]: c) {
        ans += gr[u][v];
      }
    }
  }
}

BENCHMARK(test)
    ->ArgsProduct({benchmark::CreateDenseRange(8, 25, 1), benchmark::CreateDenseRange(10, 90, 10), benchmark::CreateDenseRange(1, 6, 1)})
    ->Threads(8)
    ->Repetitions(1)
    ->Complexity();
