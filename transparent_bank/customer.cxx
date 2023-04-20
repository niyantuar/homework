#include "customer.hxx"
#include <stdexcept>


Customer::Customer()
    : _balance{}
    , _is_frozen{false} {}

bool Customer::_is_balance_valid(balance_t balance) const noexcept {
  return _minimum_balance_allowed <= balance and
         balance <= _maximum_balance_allowed;
}
Customer::BalanceInfo Customer::get_balance_info() const noexcept {
    return {_balance, _minimum_balance_allowed, _maximum_balance_allowed};
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
        throw std::range_error{"Invalid transaction"};
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
void Customer::set_maximum_allowed(balance_t maximum_allowed) {
  if (_balance > maximum_allowed) {
    throw std::range_error{"Invalid maximum"};
  }
  _maximum_balance_allowed = maximum_allowed;
}
void Customer::set_minimum_allowed(balance_t minimum_allowed) {
  if (_balance > minimum_allowed) {
    throw std::range_error{"Invalid minimum"};
  }
  _minimum_balance_allowed = minimum_allowed;
}
