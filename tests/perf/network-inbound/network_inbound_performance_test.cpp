#include <benchmark/benchmark.h>
#include <iostream>

static void BM_DoHeavyWork(benchmark::State& state) {
    for (auto _ : state) {
        std::cout << "hello" << std::endl;
    }
}

BENCHMARK(BM_DoHeavyWork);
BENCHMARK_MAIN();
