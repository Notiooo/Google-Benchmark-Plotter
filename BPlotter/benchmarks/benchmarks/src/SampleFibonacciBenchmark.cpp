#include <benchmark/benchmark.h>

int FibonacciIterative(int n)
{
    if (n <= 1)
    {
        return n;
    }
    int a = 0, b = 1, c = 0;
    for (int i = 2; i <= n; ++i)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

static void BM_FibonacciIterative(benchmark::State& state)
{
    int n = state.range(0);
    for (auto _: state)
    {
        benchmark::DoNotOptimize(FibonacciIterative(n));
    }
}

BENCHMARK(BM_FibonacciIterative)->RangeMultiplier(2)->Range(10, 40);