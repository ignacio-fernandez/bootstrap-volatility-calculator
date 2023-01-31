#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>

#include "Forward.h"
#include "parse.h"

using namespace std;



void parse_forwards(istream &input,
        const unordered_map<string,Discount> &discounts,
        unordered_map<string,ForwardYield> &forwards) {
    string line;
    if (!getline(input,line)) {
        throw runtime_error("File is empty.");
    }
    auto items=split_string(line);
    if ((items.size()!=2) || (items[0]!="valuation_date")) {
        ostringstream ss;
        ss<<"Expected 'valuation_date' but got '"<<items[0]<<"' instead.";
        throw runtime_error(ss.str());
    }
    Date valuation_date(items[1]);
    while (getline(input,line)) {
        auto items=split_string(line);
        if (items.size()==0) {
            continue; //skip empty lines
        }
        else if (items.size()!=3) { 
            ostringstream ss;
            ss<<"Expected underlyging,spot price,currency, but got '"<<line<<"' instead.";
            throw runtime_error(ss.str());

        }
        string underlying=items[0];
        double spot=parse_double(items[1]);
        string currency=items[2];
        auto it=discounts.find(currency);
        if (it==discounts.end()) {
            ostringstream ss;
            ss<<"ForwardYield for underlying '"<<underlying<<"' has unknown currency '"<<currency<<"'";
            throw runtime_error(ss.str());
        }
        const Discount &disc=it->second;
        Discount dividends=parse_discount(input,valuation_date);
        //discounts[currency]=parse_csv_discount(input,valuation_date);
        forwards.emplace(underlying,ForwardYield(spot,disc,dividends));
    }
}

void parse_forwards(istream &input,
        const unordered_map<string,Discount> &discounts,
         unordered_map<string,ForwardDividend> &forwards) {
    string line;
    if (!getline(input,line)) {
        throw runtime_error("File is empty.");
    }
    auto items=split_string(line);
    if ((items.size()!=2) || (items[0]!="valuation_date")) {
        ostringstream ss;
        ss<<"Expected 'valuation_date' but got '"<<items[0]<<"' instead.";
        throw runtime_error(ss.str());
    }
    Date valuation_date(items[1]);
    while (getline(input,line)) {
        auto items=split_string(line);
        if (items.size()==0) {
            continue; //skip empty lines
        }
        else if (items.size()!=3) { 
            ostringstream ss;
            ss<<"Expected underlyging,spot price,currency, but got '"<<line<<"' instead.";
            throw runtime_error(ss.str());

        }
        string underlying=items[0];
        double spot=parse_double(items[1]);
        string currency=items[2];
        auto it=discounts.find(currency);
        if (it==discounts.end()) {
            ostringstream ss;
            ss<<"ForwardYield for underlying '"<<underlying<<"' has unknown currency '"<<currency<<"'";
            throw runtime_error(ss.str());
        }
        const Discount &disc=it->second;
        auto [expiries,dividends]=parse_values(input,valuation_date);
        forwards.emplace(underlying,ForwardDividend(spot,disc,expiries,dividends));
    }
}