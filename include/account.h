#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <fstream>
#include <string>

enum AccountType { asset, liability, expense, revenue};
struct Account {
  int number;
  std::string name;
  AccountType type;


  void swap(Account& other) noexcept {
    using std::swap;
    swap(number, other.number);
    swap(name, other.name);
    swap(type, other.type);
  }

  friend std::istream& operator>>(std::istream& in, Account& data);
};

#endif
