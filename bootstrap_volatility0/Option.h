//
// Option.h   Vanilla (Call/Put) equity option
//
#pragma once 
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include "Date.h"

struct Option {
  std::string underlying; 
  std::string currency;
  double notional;
  bool is_call; 
  Date expiry; 
  double strike;
};

std::vector<Option> parse_options(std::istream &input);

struct OptionPrice {
  Option option;
  double price;
};

std::vector<OptionPrice> parse_options_and_prices(std::istream &input);

inline std::ostream &operator<<(std::ostream &os,const Option &opt) {
  std::cout<<opt.underlying<<","<<opt.currency<<","<<opt.notional<<",";
  std::cout<<(opt.is_call?"Call":"Put")<<","<<opt.expiry<<","<<opt.strike;
  return os;
}

inline std::ostream &operator<<(std::ofstream &os,const Option &opt) {
  os<<opt.underlying<<","<<opt.currency<<","<<opt.notional<<",";
  os<<(opt.is_call?"Call":"Put")<<","<<opt.expiry<<","<<opt.strike;
  return os;
}
