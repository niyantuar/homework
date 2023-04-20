#ifndef BANK_HXX
#define BANK_HXX
#include <memory>
#include <stdexcept>
#include <vector>
#include "customer.hxx"

class QueryManager {
    public:
        using customers_t = std::vector<Customer>;
        using customers_ptr_t = std::shared_ptr<customers_t>;
        QueryManager(customers_ptr_t customers);
        Customer::BalanceInfo get_balance_info(size_t index) const;
        void check_is_index_valid(size_t index) const;
        void freeze(size_t index);
        void unfreeze(size_t index);
        void send_money(size_t from, size_t to, Customer::balance_t value);

      private:
        customers_ptr_t _customers;
        size_t _customer_count;
};

#endif // BANK_HXX
