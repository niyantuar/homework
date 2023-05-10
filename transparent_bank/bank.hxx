#ifndef BANK_HXX
#define BANK_HXX
#include <memory>
#include <stdexcept>
#include <vector>
#include "mutex.hxx"
#include "bank_storage.hxx"

class Bank {
  public:
    Bank(BankStorage* storage, const Mutex& mutex);

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
    BankStorage* _storage;
    mutable Mutex _mutex;
};

#endif // BANK_HXX
