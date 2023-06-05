#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "entry.h"
#include <fstream>
#include <string>
#include <vector>

struct Transaction {
  std::string date;
  std::string title;
  std::vector<std::string> comments;
  std::vector<Entry> entries;

  void swap(Transaction& other) noexcept {
    using std::swap;
    swap(date, other.date);
    swap(title, other.title);
    swap(comments, other.comments);
    swap(entries, other.entries);
  }

  friend std::istream& operator>>(std::istream& in, Transaction& data);
};

#endif
