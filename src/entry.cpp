#include "entry.h"
#include <algorithm>

std::istream& operator>>(std::istream& in, Entry& data)
{
    Entry tmp;
    std::string amount;
    char type;
    if (in >> std::ws >> tmp.accountNumber >> std::ws >> type >> std::ws >> amount ) {
      tmp.debit = type == 'D';
      amount.erase(std::remove(amount.begin(), amount.end(), '.'), amount.end());
      tmp.amountCents = stoi(amount);
      data.swap(tmp);
    }
    return in;
}
