#include "transaction.h"
#include <sstream>
#include <iostream>

std::istream& operator>>(std::istream& in, Transaction& data)
{
    Transaction tmp;
    //read the date
    if (!std::getline(in, tmp.date)){
      return in;
    }

    std::string line;
    while(std::getline(in, line)){
      //read the title
      if( line.find(":") == 0 ){
        tmp.title = line.assign(++line.begin(), line.end());
      }

      //read any comments
      else if (line.find("#") == 0 ) {
        tmp.comments.emplace_back(line);
      }

      else if (line.find("^") == 0 ) {
        //close the record
        data.swap(tmp);
        return in;
      }

      //otherwise we need to read entries
      else {
        std::stringstream ss;
        ss << line;

        Entry e;
        if(! (ss >> e) ) {
          return in;
        }
        tmp.entries.emplace_back(e);
      }
    }

    return in;
}
