#include <iostream>
#include <vector>
#include <algorithm>
#include "complex_num.hxx"

int main(){
	// input N complex numbers, sort them by modulus
	// output a sorted sequence of complex numbers
    std::size_t n;
    std::cin >> n;
    std::vector<ComplexNum> complex_numbers(n);
    for (auto& el : complex_numbers) {
        std::cin >> el;
    }
    std::sort(
            complex_numbers.begin(), complex_numbers.end(),
            [](ComplexNum left, ComplexNum right) -> bool {
                return left.get_abs() < right.get_abs();
            }
    );

    for (auto el : complex_numbers) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

	return 0;
}
