#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

int main(int args, char** argv)
{
    std::cout << "int is trivially: " << std::is_trivially_constructible<int>::value << std::endl;

    std::cout << "vector<int> is trivially: " << std::is_trivially_constructible<vector<int>>::value
              << std::endl;

    return 0;
}
