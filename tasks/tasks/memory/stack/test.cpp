#include <catch.hpp>

#include <vector>
#include <deque>
#include <random>

#include <stack.h>

void Check(const Stack& actual, const std::vector<int>& expected) {
    REQUIRE(actual.Size() == expected.size());
    Stack copy = actual;
    for (size_t i = 0; i < expected.size(); ++i) {
        REQUIRE(copy.Top() == expected[expected.size() - 1 - i]);
        copy.Pop();
    }
    REQUIRE(copy.Empty());
}

TEST_CASE("Stack has constructors", "[stack]") {
    {
        Stack a;
        REQUIRE(a.Size() == 0u);
        REQUIRE(a.Empty());
    }
    {
        Stack a;
        a.Push(1);
        a.Push(2);
        a.Push(3);
        Stack b(a);
        Check(b, std::vector<int>{1, 2, 3});
    }
    {
        Stack a;
        a.Push(1);
        a.Push(2);
        Stack b(std::move(a));
        Check(b, std::vector<int>{1, 2});
    }
}

TEST_CASE("Basic methods", "[stack]") {
    Stack a;
    REQUIRE(a.Empty());

    a.Push(1);
    REQUIRE(a.Size() == 1u);
    REQUIRE(a.Top() == 1);

    a.Push(3);
    a.Push(5);
    REQUIRE(a.Size() == 3u);
    REQUIRE(a.Top() == 5);

    a.Pop();
    REQUIRE(a.Size() == 2u);
    REQUIRE(a.Top() == 3);

    a.Pop();
    a.Pop();
    REQUIRE(a.Empty());
    REQUIRE(a.Size() == 0u);

    a.Push(42);
    REQUIRE(a.Top() == 42);
    a.Clear();
    REQUIRE(a.Empty());
}

TEST_CASE("Modifications with Top", "[stack]") {
    Stack a;
    a.Push(1);
    a.Push(2);
    a.Push(3);

    a.Top() = 100;
    REQUIRE(a.Top() == 100);

    a.Pop();
    REQUIRE(a.Top() == 2);

    const Stack& b = a;
    REQUIRE(b.Top() == 2);
}

TEST_CASE("Reallocations", "[stack]") {
    Stack a;
    const int iterations = 1e6;
    for (int i = 0; i < iterations; ++i) {
        a.Push(i);
    }
    REQUIRE(a.Size() == static_cast<size_t>(iterations));
    for (int i = iterations - 1; i >= 0; --i) {
        REQUIRE(a.Top() == i);
        a.Pop();
    }
    REQUIRE(a.Empty());
}

TEST_CASE("Copy correctness", "[stack]") {
    Stack a;
    Stack b(a);
    b.Push(1);
    Check(a, std::vector<int>());
    Check(b, std::vector<int>{1});

    b = b; 
    Check(b, std::vector<int>{1});

    a = b;
    Check(a, std::vector<int>{1});

    b = std::move(a);
    Check(b, std::vector<int>{1});

    Stack c(std::move(b));
    Check(c, std::vector<int>{1});

    Stack d;
    d.Push(3);
    d.Push(4);
    d.Push(5);
    Stack e(d);
    Check(e, std::vector<int>{3, 4, 5});

    d.Swap(c);
    Check(e, std::vector<int>{3, 4, 5});
    Check(d, std::vector<int>{1});
    Check(c, std::vector<int>{3, 4, 5});
}

TEST_CASE("Swap", "[stack]") {
    Stack a;
    a.Push(1);
    a.Push(2);
    Stack b;
    b.Push(10);
    b.Push(20);
    b.Push(30);

    a.Swap(b);
    Check(a, std::vector<int>{10, 20, 30});
    Check(b, std::vector<int>{1, 2});

    a.Swap(a);
    Check(a, std::vector<int>{10, 20, 30});
}

TEST_CASE("Stress", "[stack]") {
    const int iterations = 1e6;
    Stack a;
    std::vector<int> ref;

    std::mt19937 gen(735675);
    std::uniform_int_distribution<int> dist(1, 4);

    for (int i = 0; i < iterations; ++i) {
        int code = dist(gen);
        int value = gen();
        if (code == 1 || code == 2) {
            a.Push(value);
            ref.push_back(value);
        } else if (code == 3) {
            if (!ref.empty()) {
                a.Pop();
                ref.pop_back();
            }
        } else {
            if (!ref.empty()) {
                REQUIRE(a.Top() == ref.back());
            }
        }
    }

    REQUIRE(a.Size() == ref.size());
    for (int i = static_cast<int>(ref.size()) - 1; i >= 0; --i) {
        REQUIRE(a.Top() == ref[i]);
        a.Pop();
    }
    REQUIRE(a.Empty());
}

TEST_CASE("Empty correctness", "[stack]") {
    const size_t test_size = 1e3;
    {
        Stack a;
        for (size_t i = 0; i < test_size; ++i) a.Push(static_cast<int>(i));
        for (size_t i = 0; i < test_size; ++i) a.Pop();
        REQUIRE(a.Size() == 0u);
        REQUIRE(a.Empty());
    }
    {
        Stack a;
        for (size_t i = 0; i < test_size; ++i) a.Push(static_cast<int>(i));
        a.Clear();
        REQUIRE(a.Size() == 0u);
        REQUIRE(a.Empty());
    }
}