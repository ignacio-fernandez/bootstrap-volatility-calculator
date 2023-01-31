#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>

#include "Discount.h"
#include "interpolate.h"
#include "parse.h"

using namespace std;

Discount::Discount(Date valuation_date, 
            const std::vector<Date> &dates,
            const std::vector<double> &rates): 
            m_valuation_date(valuation_date),
            m_Ts(dates.size()),
            m_rate(rates) 
{
        if (dates.size()!=rates.size()) {
            ostringstream ss;
            ss<<"dates vector size ("<<dates.size();
            ss<<") is different from rates vector size (";
            ss<<rates.size()<<")";
            throw runtime_error(ss.str());
        }
        for (int i1=0;i1<dates.size();i1++) {
            m_Ts[i1]=(dates[i1]-m_valuation_date)/365;
        }   
}

double Discount::discount(Date expiry) const 
{
            double T=get_T(expiry); 
            double rate=interpolate(T,m_Ts,m_rate);
            // expiry in years from valuation date
            return exp(-T*rate); 
} 


Discount parse_discount0(istream &input, Date valuation_date) {
    string line;
    vector<Date> expiries;
    vector<double> rates;
    while (getline(input,line)) {
        auto items=split_string(line);
        if ((items.size()==1)&&(items[0]=="")) 
           break;  // empty line means end of data
        Date date=Date(items[0]);
        double rate=parse_double(items[1]);
        expiries.push_back(date);
        rates.push_back(rate);
    }
    return Discount(valuation_date,expiries,rates);
}

std::pair<std::vector<Date>,std::vector<double>> parse_values(istream &input, Date valuation_date) {
    string line;
    vector<Date> expiries;
    vector<double> rates;
    Date date0=valuation_date;
    while (getline(input,line)) {
        auto items=split_string(line);
        //should be trim(items[0])
        if ((items.size()==1)&&(items[0]=="")) {
           break;  // empty line means end of data
        }
        if (items.size()!=2) {
            std::runtime_error("Expected date and rate.");
        }
        Date date=Date(items[0]);
        double rate=parse_double(items[1]);
        if (!(date0<date)) {
            ostringstream ss;
            ss<<"parse_values dates must be extrictly increasing, but "<<date<<" is not later than "<<date0;
            throw std::runtime_error(ss.str());
        }
        date0=date;// for next check
        expiries.push_back(date);
        rates.push_back(rate);
    }
    return std::pair{expiries,rates};
}
Discount parse_discount(istream &input, Date valuation_date) {
    auto [expiries,rates]=parse_values(input,valuation_date);
    if (expiries.empty()) {
        throw std::runtime_error("Expiries can not be empty when constructing a Discount object.");
    }
    return Discount(valuation_date,expiries,rates);
}

unordered_map<string,Discount> parse_discounts0(istream &input) {
    unordered_map<string,Discount> discounts;
    string line;
    auto items=split_string(line);
    Date valuation_date(items[1]);
    while (getline(input,line)) {
        auto items=split_string(line);
        if (items.size()==0) {
            continue; //skip empty lines
        }
        string currency=items[0];
        discounts.emplace(currency,parse_discount(input,valuation_date));
    }
    return discounts;
}

unordered_map<string,Discount> parse_discounts(istream &input) {
    unordered_map<string,Discount> discounts;
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
        else if (items.size()!=1) { 
            ostringstream ss;
            ss<<"Expected currency, but got '"<<line<<"' instead.";
            throw runtime_error(ss.str());

        }
        string currency=items[0];
        if (discounts.find(currency)!=discounts.end()) {
            ostringstream ss;
            ss<<"Multiple definitions of currency '"<<currency<<"'";
            throw runtime_error(ss.str());
        }
        try {
            discounts.emplace(currency,parse_discount(input,valuation_date));
        }
        catch (const std::runtime_error &error) {
            ostringstream ss;
            ss<<"Could not parte discount curve for currency '"<<currency<<"': "<<error.what();
            throw std::runtime_error(ss.str());
        }
    }
    return discounts;
}