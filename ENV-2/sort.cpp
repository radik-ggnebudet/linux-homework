#include "sort.h"

void sort_complex(std::vector<complex>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        for (size_t j = i + 1; j < arr.size(); j++) {
            if (arr[j].abs() < arr[i].abs()) {
                complex tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}
