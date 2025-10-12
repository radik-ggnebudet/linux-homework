#include <iostream>
#include <vector>
#include "complex.h"
#include "sort.h"

int main() {
    std::vector<complex> arr;
    arr.push_back(complex(3, 4));
    arr.push_back(complex(1, 1));
    arr.push_back(complex(0, 2));
    arr.push_back(complex(2, 0));

    std::cout << "before sort:" << std::endl;
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i].print();
        std::cout << std::endl;
    }

    sort_complex(arr);

    std::cout << "after sort:" << std::endl;
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i].print();
        std::cout << std::endl;
    }

    return 0;
}
