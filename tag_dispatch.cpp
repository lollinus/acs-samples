// Sample tag dispatch usage
// https://en.cppreference.com/w/cpp/iterator/iterator_tags#Example
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

// quite often implementation details are hidden in a dedicated namespace
namespace implementation_details {
template<class BDIter>
void alg(BDIter, BDIter, std::bidirectional_iterator_tag)
{
    std::cout << "1. legacy::alg() called for bidirectional iterator\n";
}

template<class RAIter>
void alg(RAIter, RAIter, std::random_access_iterator_tag)
{
    std::cout << "2. legacy::alg() called for random-access iterator\n";
}
} // namespace implementation_details

template<class Iter>
void alg(Iter first, Iter last)
{
    implementation_details::alg(first, last,
        typename std::iterator_traits<Iter>::iterator_category());
}

int main()
{
    std::list<int> l;
    alg(l.begin(), l.end()); // 1.

    std::vector<int> v;
    alg(v.begin(), v.end()); // 1.

//    std::istreambuf_iterator<char> i1(std::cin), i2;
//    alg(i1, i2);         // compile error: no matching function for call
}
