add_gtest(test_futex test.cpp mutex.cpp)
add_benchmark(bench_futex bench.cpp mutex.cpp)
