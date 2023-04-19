#include "bank.hxx"
QueryManager::QueryManager(customers_ptr_t customers) : _customers{customers} {}
Customer::BalanceInfo QueryManager::get_balance_info(size_t index) const {
  check_is_index_valid(index);
  return (*_customers)[index].get_balance_info();
}
void QueryManager::check_is_index_valid(size_t index) const {
  if (index >= _customer_count) {
    throw std::range_error{"Invalid index"};
  }
}
void QueryManager::freeze(size_t index) {
  check_is_index_valid(index);
  (*_customers)[index].freeze();
}
void QueryManager::unfreeze(size_t index) {
  check_is_index_valid(index);
  (*_customers)[index].unfreeze();
}
void QueryManager::send_money(size_t from, size_t to,
                              Customer::balance_t value) {
  check_is_index_valid(from);
  check_is_index_valid(to);
  if (value <= 0) {
    throw std::logic_error{"Sent value cannot be non positive"};
  }

  auto &sender = (*_customers)[from];
  auto &receiver = (*_customers)[to];
  sender.check_is_not_frozen();
  receiver.check_is_not_frozen();
  if (sender.is_valid_transaction(-value) and
      receiver.is_valid_transaction(value)) {
    sender.unsafe_transaction(-value);
    receiver.unsafe_transaction(value);
  }
}
