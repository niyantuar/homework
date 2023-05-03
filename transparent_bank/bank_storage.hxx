#ifndef BANK_STORAGE_HXX
#define BANK_STORAGE_HXX

#include "customer.hxx"
#include <cstddef>
#include <stdexcept>


struct BankStorage {
    size_t user_count;
    Customer customers[];
    Customer& at(size_t index) {
        if (index >= user_count) {
            throw std::range_error{"Customers out of range"};
        }
        return (*this)[index];
    }
    Customer& operator[](size_t index) {
        return customers[index];
    }
};

#endif // BANK_STORAGE_HXX
