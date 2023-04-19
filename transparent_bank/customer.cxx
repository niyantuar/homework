#include "customer.hxx"
#include <stdexcept>


Customer::Customer()
    : _balance{}
    , _is_frozen{false} {}

bool Customer::_is_balance_valid(balance_t balance) const noexcept {
  return minimum_balance_allowed <= balance and
         balance <= maximum_balance_allowed;
}
Customer::BalanceInfo Customer::get_balance_info() const noexcept {
    return {_balance, minimum_balance_allowed, maximum_balance_allowed};
}

CustomerFrozenException::CustomerFrozenException()
    : std::logic_error{"Cannot perform any operation on a frozen "
                       "customer except unfreeze"} {}
void Customer::unfreeze() noexcept { _is_frozen = false; }
void Customer::freeze() noexcept { _is_frozen = true; }
void Customer::check_is_not_frozen() const {
  if (_is_frozen) {
    throw CustomerFrozenException{};
  }
}
bool Customer::is_valid_transaction(balance_t value) const noexcept {
  return _is_balance_valid(_balance + value);
}
void Customer::check_is_valid_transaction(balance_t value) const {
    if(not _is_balance_valid(_balance + value)) {
        throw std::logic_error{"Invalid transaction"};
    }
}
void Customer::transaction(balance_t value) {
  check_is_not_frozen();
  check_is_valid_transaction(value);
  _balance += value;
}
void Customer::unsafe_transaction(balance_t value) noexcept {
  _balance += value;
}
