#ifndef CUSTOMER_HXX
#define CUSTOMER_HXX
#include <cstdint>
#include <stdexcept>

class CustomerFrozenException: public std::logic_error {
  public:
    explicit CustomerFrozenException();
};

class Customer {
  public:
    using balance_t = int64_t;
    struct BalanceInfo {
        balance_t current_balance;
        balance_t minimum_allowed_balance;
        balance_t maximum_allowed_balance;
    };
    Customer();
    BalanceInfo get_balance_info() const noexcept;
    void freeze() noexcept;
    void unfreeze() noexcept;
    void check_is_not_frozen() const;
    bool is_valid_transaction(balance_t value) const noexcept;
    void check_is_valid_transaction(balance_t value) const;
    void transaction(balance_t value);

    // CAUTION
    // This method does not check whether the transaction can be performed
    // and directly modifes the members of the class instance.
    // This is provided for preformance encrease and
    // ONLY can be used if the checks were performed elsewhere
    void unsafe_transaction(balance_t value) noexcept;

    void set_minimum_allowed(balance_t minimum_allowed);
    void set_maximum_allowed(balance_t maximum_allowed);

  private:
    bool _is_balance_valid(balance_t balance) const noexcept;
  private:
    balance_t _minimum_balance_allowed;
    balance_t _maximum_balance_allowed;
    balance_t _balance;
    bool _is_frozen;
    constexpr static balance_t _DEFAULT_MINIMUM_BALANCE_ALLOWED{0};
    constexpr static balance_t _DEFAULT_MAXIMUM_BALANCE_ALLOWED{1000};
};

#endif // CUSTOMER_HXX
