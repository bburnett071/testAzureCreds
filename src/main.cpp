#include "account.h"
#include "transaction.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

bool isJournalValid(std::vector<Transaction> *transactions){
  bool valid = true;
  //validate the transactions
  std::for_each( transactions->begin(), transactions->end(), [&valid](Transaction &t){
    //confirm the debits and credits are in balance
    int sum = 0;
    std::for_each( t.entries.begin(), t.entries.end(), [&sum](Entry &e){
      sum += e.amountCents * (e.debit ? 1 : -1);
    });
    if (sum != 0) {
      std::cout<<"Transaction '"<<t.title<<"' is out of balance"<<std::endl;
      valid = false;
    }
  });
  return valid;
}

typedef std::map<int, int> AccountBalanceMap;
typedef std::map<int, Account> AccountMap;
typedef std::map<int, std::vector<Entry*>> AccountJournalMap;

AccountBalanceMap calculateBalance(const AccountMap accounts, AccountJournalMap entries){
  AccountBalanceMap balance;
  for (auto const &[key, val]: accounts){
    int d = 1, c = 1;
    int sum = 0;
    switch( val.type ){
      case AccountType::asset:
      case AccountType::expense:
        //assets and expenses are debit accounts, meaning debits are an increase, all others are credit
        //accounts, where a credit is an increase.
        d = 1;
        c = -1;
        break;
      default:
        d = -1;
        c = 1;
    }

    std::for_each(entries[key].begin(), entries[key].end(), [&sum, &d, &c](Entry* e){
      sum += e->debit ? d * e->amountCents : c * e->amountCents;
    });
    balance[key] = sum;
  }
  return balance;
}

void printBalanceSheet(const AccountMap accounts, AccountBalanceMap balance){
  int totals[3]{0,0,0};
  AccountType loop[]{ AccountType::asset, AccountType::liability, AccountType::expense };
  int index = 0;
  std::cout<<"*******************************"<<std::endl;
  std::cout<<"** BALANCE SHEET            ***"<<std::endl;
  std::cout<<"*******************************"<<std::endl;
  std::cout<<"Assets"<<std::endl;
  while(index < 3){
    auto filter = loop[index];
    for (auto const &[key, val]: accounts){
      if( val.type != filter || balance[key] == 0){
        continue;
      }
      std::cout<<val.number<<std::setw(40)<<val.name<<"\t"<<balance[key]<<std::endl;
      totals[index] += balance[key];
    }

    std::cout<<std::setw(45)<<"Total:\t"<<totals[index]<<std::endl;
    std::cout<<std::endl;
    index += 1;
    if(index == 1) {
      std::cout<<"Liabilities"<<std::endl;
    } else if( index == 2) {
      std::cout<<"Expenses"<<std::endl;
    }
  }
  std::cout<<"Total Liabilities and Equity: "<<totals[1] + totals[2]<<std::endl;
}

void printTrialBalance(const AccountMap accounts, AccountJournalMap entries){
  //Trial Balance
  std::cout<<std::endl;
  std::cout<<"*******************************"<<std::endl;
  std::cout<<"** TRIAL BALANCE            ***"<<std::endl;
  std::cout<<"*******************************"<<std::endl;
  std::cout<<std::setw(70)<<"Debit\tCredit"<<std::endl;
  int debitTotal = 0, creditTotal = 0;
  for (auto const &[key, val]: accounts){
    int debits = 0, credits = 0;
    std::for_each(entries[key].begin(), entries[key].end(), [&debits, &credits](Entry* e){
        if( e->debit) {
          debits += e->amountCents;
        } else {
          credits += e->amountCents;
        }
    });
    if(debits == 0 && credits == 0 ){
      continue;
    }
    debitTotal += debits;
    creditTotal += credits;
    std::cout<<key<<" "<<std::setw(40)<<val.name<<"\t"<<std::setw(15)<<debits<<"\t"<<credits<<std::endl;
  }
  std::cout<<std::setw(63)<<debitTotal<<"\t"<<creditTotal<<std::endl;
}

int main(){

  //open the GL accounts file
  std::ifstream f_accounts("./data/accounts.tsv");
  if(!f_accounts){
    std::cout<<"Unable to read accounts.tsv"<<std::endl;
    return -1;
  }

  //read the accounts and sort into a vector
  Account account;
  AccountMap accounts;
  while(f_accounts >> account){
    accounts[account.number] =account;
  }

  //read the transaciton file
  std::ifstream f_transactions("./data/test.txn");
  if(!f_transactions){
    std::cout<<"Unable to read test.txn"<<std::endl;
    return -2;
  }

  Transaction txn;
  std::vector<Transaction> transactions;
  //use our custom stream parsing to extract transactions from the file
  while(f_transactions >> txn){
    transactions.emplace_back(txn);
  }

  //validate the transactions
  if(!isJournalValid(&transactions)){
    return -3;
  }

  //process the full journal
  AccountJournalMap fullJournal;
  //keep date based snapshots too
  std::map<std::string, AccountJournalMap> trialJournal;
  //find the starting date
  std::string day = transactions[0].date;
  trialJournal[day] = AccountJournalMap{};
  //process all the transactions
  std::for_each( transactions.begin(), transactions.end(), [&fullJournal, &trialJournal, &day](Transaction &t){
    if (t.date.compare(day) != 0){
      //found a new day, start a new incremental group
      day = t.date;
      trialJournal[day] = std::map<int, std::vector<Entry*>>{};
    }

    //normal processing...
    std::for_each(t.entries.begin(), t.entries.end(), [&fullJournal, &trialJournal, &day](Entry &e){
      fullJournal[e.accountNumber].emplace_back(&e);
      trialJournal[day][e.accountNumber].emplace_back(&e);
    });
  });

  //process the balance sheet and trial balance for the full period
  auto balance = calculateBalance(accounts, fullJournal);

  //TODO: and now process per day

  //Balance Sheet shows Assets, less Liabilities and Expenses
  printBalanceSheet(accounts, balance);
  printTrialBalance(accounts, fullJournal);
}
