#include <catch.hpp>

#include <queue.h>

#include <vector>
#include <deque>
#include <random>
#include <stdexcept>

void Check(const Queue& actual, const std::vector<int>& expected) {
    REQUIRE(actual.Size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        REQUIRE(actual.Front() == expected[i]);
        // нужно копировать, потому что Front не даёт доступ по индексу
        Queue copy = actual;
        for (size_t j = 0; j < i; ++j) {
            copy.Pop();
        }
        REQUIRE(copy.Front() == expected[i]);
    }
}

TEST_CASE("Queue has constructors", "[queue]") {
    {
        Queue q;
        REQUIRE(q.Size() == 0u);
        REQUIRE(q.Empty());
    }
    {
        Queue q;
        q.Push(1);
        q.Push(2);
        q.Push(3);
        Queue copy(q);
        REQUIRE(copy.Size() == 3u);
        REQUIRE(copy.Front() == 1);
        REQUIRE(copy.Back() == 3);
    }
    {
        Queue q;
        q.Push(1);
        q.Push(2);
        Queue moved(std::move(q));
        REQUIRE(moved.Size() == 2u);
        REQUIRE(moved.Front() == 1);
        REQUIRE(moved.Back() == 2);
        REQUIRE(q.Size() == 0u);
    }
}

TEST_CASE("Basic methods", "[queue]") {
    Queue q;
    REQUIRE(q.Empty());

    q.Push(1);
    REQUIRE(q.Size() == 1u);
    REQUIRE(q.Front() == 1);
    REQUIRE(q.Back() == 1);

    q.Push(2);
    q.Push(3);
    REQUIRE(q.Size() == 3u);
    REQUIRE(q.Front() == 1);
    REQUIRE(q.Back() == 3);

    q.Pop();
    REQUIRE(q.Size() == 2u);
    REQUIRE(q.Front() == 2);
    REQUIRE(q.Back() == 3);

    q.Pop();
    q.Pop();
    REQUIRE(q.Empty());
    REQUIRE(q.Size() == 0u);

    q.Push(42);
    REQUIRE(q.Front() == 42);
    REQUIRE(q.Back() == 42);
    q.Clear();
    REQUIRE(q.Empty());
}

TEST_CASE("Modifications with Front/Back", "[queue]") {
    Queue q;
    q.Push(1);
    q.Push(2);
    q.Push(3);

    q.Front() = 100;
    REQUIRE(q.Front() == 100);

    q.Back() = 200;
    REQUIRE(q.Back() == 200);

    q.Pop();
    REQUIRE(q.Front() == 2);

    const Queue& cq = q;
    REQUIRE(cq.Front() == 2);
    REQUIRE(cq.Back() == 200);
}

TEST_CASE("Empty correctness") {
    const size_t test_size = 1e3;
    {
        Queue q;
        for (size_t i = 0; i < test_size; ++i) q.Push(static_cast<int>(i));
        for (size_t i = 0; i < test_size; ++i) q.Pop();
        REQUIRE(q.Size() == 0u);
        REQUIRE(q.Empty());
    }
    {
        Queue q;
        for (size_t i = 0; i < test_size; ++i) q.Push(static_cast<int>(i));
        q.Clear();
        REQUIRE(q.Size() == 0u);
        REQUIRE(q.Empty());
    }
}

TEST_CASE("Reallocations", "[queue]") {
    Queue q;
    const int iterations = 1e6;
    for (int i = 0; i < iterations; ++i) {
        q.Push(i);
    }
    REQUIRE(q.Size() == static_cast<size_t>(iterations));
    for (int i = 0; i < iterations; ++i) {
        REQUIRE(q.Front() == i);
        q.Pop();
    }
    REQUIRE(q.Empty());
}

TEST_CASE("Copy correctness", "[queue]") {
    Queue a;
    Queue b(a);
    b.Push(1);
    REQUIRE(a.Size() == 0u);
    REQUIRE(b.Size() == 1u);

    b = b;   // NOLINT
    REQUIRE(b.Size() == 1u);

    a = b;
    REQUIRE(a.Size() == 1u);
    REQUIRE(a.Front() == 1);

    b = std::move(a);
    REQUIRE(b.Size() == 1u);

    Queue c(std::move(b));
    REQUIRE(c.Size() == 1u);
    REQUIRE(c.Front() == 1);

    Queue d;
    d.Push(3);
    d.Push(4);
    d.Push(5);
    Queue e(d);
    REQUIRE(e.Size() == 3u);
    REQUIRE(e.Front() == 3);
    REQUIRE(e.Back() == 5);

    d.Swap(c);
    REQUIRE(e.Front() == 3);
    REQUIRE(d.Size() == 1u);
    REQUIRE(d.Front() == 1);
    REQUIRE(c.Size() == 3u);
    REQUIRE(c.Front() == 3);
}

TEST_CASE("Stress", "[queue]") {
    const int iterations = 1e6;
    Queue a;
    std::deque<int> b;
    std::mt19937 gen(735675);
    std::uniform_int_distribution<int> dist(1, 4);

    for (int i = 0; i < iterations; ++i) {
        int code = dist(gen);
        int value = gen();
        if (code == 1 || code == 2) {
            a.Push(value);
            b.push_back(value);
        } else if (code == 3) {
            if (!b.empty()) {
                a.Pop();
                b.pop_front();
            }
        } else {
            if (!b.empty()) {
                REQUIRE(a.Front() == b.front());
                REQUIRE(a.Back() == b.back());
            }
        }
    }

    REQUIRE(a.Size() == b.size());
    while (!b.empty()) {
        REQUIRE(a.Front() == b.front());
        a.Pop();
        b.pop_front();
    }
    REQUIRE(a.Empty());
}

TEST_CASE("Cycled buffer") {
    Queue q;
    const int iterations = 128;

    std::vector<int> v;
    for (int i = 0; i < iterations; ++i) {
        q.Push(i);
        v.push_back(i);
    }
    for (int i = 0; i < iterations; ++i) {
        REQUIRE(q.Front() == v[i]);
        q.Pop();
    }
    REQUIRE(q.Empty());

    // Много push/pop — проверка кольца
    Queue q2;
    std::deque<int> ref;
    for (int i = 0; i < 1000; ++i) {
        q2.Push(i);
        ref.push_back(i);
        if (i % 3 == 0) {
            q2.Pop();
            ref.pop_front();
        }
    }
    REQUIRE(q2.Size() == ref.size());
    while (!ref.empty()) {
        REQUIRE(q2.Front() == ref.front());
        q2.Pop();
        ref.pop_front();
    }
}