#pragma once

struct Sum {

    template<typename T, typename U>
    auto operator() (const T& a, const U& b) const {
        return a + b;
    }

};

struct Prod {
    template<typename T, typename U>
    auto operator() (const T& a, const U& b) const {
        return a * b;
    }
};

struct Concat {
    template <typename T, typename U>
    auto operator() (T a1, const U& a2) const {
        a1.insert(a1.end(), a2.begin(), a2.end());
        return a1;
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    while (first != last) {
        init = func(init, *first);
        ++first;

    }
    return init;


}

class Length {
    public: 

    explicit Length(int* length): length_(length){
        *length_ = 0;

    }

    template <typename T, typename U>
    T operator() (T init, const U&)const {
        ++(*length_);
        return init;
    }


    private:
    int* length_;
};
