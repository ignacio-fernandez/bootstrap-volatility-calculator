#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Option.h"

#include "Date.h"
#include "parse.h"
#include "black_scholes.h"

using namespace std;


  

vector<Option> parse_options(istream &input) {
    vector<Option> options;
    string line;
    if (!getline(input,line)) //header
        throw std::runtime_error("Empty options file, expected header");
    // could check here that the column order matches our assumption
    while (getline(input,line)) {
        auto items=split_string(line);
        if (items.size()!=6) {
            ostringstream ss;
            ss<<"Invalid option line '"<<line<<"'\n";
            ss<<"  expected underlying,currency,notional,Call/Put,expiry,strike";
            throw std::runtime_error(ss.str());
        }
        std::string underlying=items[0]; 
        std::string currency=items[1];    
        double notional=parse_double(items[2]); 
        bool is_call=parse_bool(items[3],"Call","Put"); 
        Date expiry(items[4]);
        double strike=parse_double(items[5]);
        options.emplace_back(Option{underlying,currency,notional,is_call,expiry,strike});
    }
    return options;
}

vector<OptionPrice> parse_options_and_prices(istream &input) {
    vector<OptionPrice> options;
    string line;
    if (!getline(input,line)) //header
        throw std::runtime_error("Empty options file, expected header");
    // could check here that the column order matches our assumption
    while (getline(input,line)) {
        auto items=split_string(line);
        if (items.size()!=7) {
            ostringstream ss;
            ss<<"Invalid option line '"<<line<<"'\n";
            ss<<"  expected underlying,currency,notional,Call/Put,expiry,strike,price";
            throw std::runtime_error(ss.str());
        }
        std::string underlying=items[0]; 
        std::string currency=items[1];    
        double notional=parse_double(items[2]); 
        bool is_call=parse_bool(items[3],"Call","Put"); 
        Date expiry(items[4]);
        double strike=parse_double(items[5]);
        double price=parse_double(items[6]);
        options.emplace_back(OptionPrice{Option{underlying,currency,notional,is_call,expiry,strike},price});
    }
    return options;
}



