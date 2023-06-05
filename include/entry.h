#ifndef ENTRY_H
#define ENTRY_H

#include "entry.h"
#include <fstream>
#include <string>
#include <vector>

struct Entry {
  int accountNumber;
  bool debit;
  int amountCents;

  void swap(Entry& other) noexcept {
    using std::swap;
    swap(accountNumber, other.accountNumber);
    swap(debit, other.debit);
    swap(amountCents, other.amountCents);
  }

  friend std::istream& operator>>(std::istream& in, Entry& data);
};

#endif
