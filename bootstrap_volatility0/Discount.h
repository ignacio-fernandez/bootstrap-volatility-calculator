#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <unordered_map>

#include "Date.h"

class Discount {
    public:
        //Discount()=default;
        Discount (Date valuation_date, 
            const std::vector<Date> &dates,
            const std::vector<double> &rates);
        double discount(Date expiry) const;
        Date valuation_date() const {
            return m_valuation_date;
        }
    private: 
        Date m_valuation_date;
        std::vector<double> m_Ts;
        std::vector<double> m_rate;
        inline double get_T(Date expiry) const 
        { 
            return  (expiry-m_valuation_date)/365.0; 
        }
};

std::pair<std::vector<Date>,std::vector<double>> parse_values(std::istream &input, Date valuation_date);
Discount parse_discount(std::istream &input, Date valuation_date);
std::unordered_map<std::string,Discount> parse_discounts(std::istream &input);