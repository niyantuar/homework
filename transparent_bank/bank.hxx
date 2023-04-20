#ifndef BANK_HXX
#define BANK_HXX
#include <memory>
#include <stdexcept>
#include <vector>
#include "mutex.hxx"
#include "customer.hxx"

class Bank {
    public:
        using customers_t = std::vector<Customer*>;
        using customers_ptr_t = std::shared_ptr<customers_t>;
        Bank(customers_ptr_t customers, std::shared_ptr<Mutex> mutex);

        Customer::BalanceInfo get_balance_info(size_t index) const;
        void freeze(size_t index);
        void unfreeze(size_t index);
        void send_money(size_t from, size_t to, Customer::balance_t value);
        void transaction_to_every_valid_customer(Customer::balance_t value);
        void set_minimum_allowed(
            Customer::balance_t minimum_allowed, size_t index
        );
        void set_maximum_allowed(
            Customer::balance_t maximum_allowed, size_t index
        );
      private:
        customers_ptr_t _customers;
        mutable std::shared_ptr<Mutex> _mutex;
};

#endif // BANK_HXX
