#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <set>
#include <list>
using namespace std;

template <typename T> void print_vector(vector<T> &v) {
    for (T e : v) {
        cout << e << " ";
    }
    cout << endl;
}

// specialization for bool type to print literal instead of digit
template <> void print_vector<bool>(vector<bool> &v) {
    for (bool e : v) {
        cout << (e ? "T" : "F") << " ";
    }
    cout << endl;
}

// Generic print container
template <template <class, class...> class C, class T, class... Args>
void print_container(C<T, Args...> &c) {
    for (T val : c) {
        cout << val << " ";
    }
    cout << endl;
}

// specialization for bool type to print literal instead of digit
template <template <typename T=bool, class...> class C, class... Args>
void print_container(C<bool, Args...> &c) {
    for (bool b : c) {
        cout << (b ? "T" : "F") << " ";
    }
    cout << endl;
}

int main() {
    vector<long> v1 = {-10, 20, -20, 40, 50, 20};
    vector<bool> v3(v1.size());
    print_container(v1);
    cout << endl;

    transform(v1.begin(), v1.end(), v3.begin(), [](auto v) -> bool { return v < 0; });
    print_container(v3);

    cout << "Print set\n";
    set<long> s1{v1.cbegin(), v1.cend()};
    print_container(s1);

    set<bool> s3{v3.cbegin(), v3.cend()};
    print_container(s3);

    cout << "Print list\n";
    list<long> l1{v1.cbegin(), v1.cend()};
    print_container(l1);

    list<bool> l3{v3.cbegin(), v3.cend()};
    print_container(l3);

    return 0;
}
