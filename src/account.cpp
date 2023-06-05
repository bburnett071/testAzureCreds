#include "account.h"

std::istream& operator>>(std::istream& in, Account& data)
{
    Account tmp;
    std::string type;
    if (// 1 Read the number. Then disicard leading white space before second field.
        (in >> tmp.number >> std::ws) &&
        // 2 Read the second field (which is terminated by tab)
        std::getline(in, tmp.name, '\t') &&
        // 3 Read the third field  (which is terminated by newline)
        std::getline(in, type)
       )
    {
        if (type == "Asset"){
          tmp.type = AccountType::asset;
        } else if (type == "Liability") {
          tmp.type = AccountType::liability;
        } else if (type == "Expense"){
          tmp.type = AccountType::expense;
        } else {
          tmp.type = AccountType::revenue;
        }

        // We have correctly read all the data we need from
        // the line so set the data object from the tmp value.
        data.swap(tmp);
    }
    return in;
}
