#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "Date.h"
#include "Discount.h"

class ForwardYield
{
public:
  ForwardYield(double spot,
               const Discount &discount,
               const Discount &dividends) : m_spot(spot),
                                            m_discount(discount),
                                            m_dividends(dividends)
  {
    if (m_discount.valuation_date() != m_dividends.valuation_date())
    {
      std::ostringstream ss;
      ss << "ForwardYield discount valuation date '" << m_discount.valuation_date() << "'";
      ss << " does not agree with dividends valuation date '" << m_dividends.valuation_date() << "'";
    }
  }
  inline double forward(Date expiry) const
  {
    return m_spot * m_dividends.discount(expiry) /
           m_discount.discount(expiry);
  }
  Date valuation_date() const
  {
    return m_discount.valuation_date();
  }

private:
  double m_spot;
  Discount m_discount;
  Discount m_dividends;
};

void parse_forwards(
    std::istream &input,
    const std::unordered_map<std::string, Discount> &discount,
    std::unordered_map<std::string, ForwardYield> &forwards);

class ForwardDividend
{
public:
  ForwardDividend(double spot,
                  const Discount &discount,
                  const std::vector<Date> &dates,
                  const std::vector<double> dividends) : m_spot(spot),
                                                         m_discount(discount),
                                                         m_dates(dates),
                                                         m_dividends(dividends)
  {
  }
  double forward(Date expiry) const
  {
    auto last = std::lower_bound(m_dates.begin(), m_dates.end(), expiry);
    size_t lb = last - m_dates.begin();
    double S = m_spot;
    for (int i1 = 0; i1 < lb; i1++)
    {
      S -= m_dividends[i1] * m_discount.discount(m_dates[i1]);
    }
    return S / m_discount.discount(expiry);
  }
  Date valuation_date() const
  {
    return m_discount.valuation_date();
  }

private:
  double m_spot;
  Discount m_discount;
  std::vector<Date> m_dates;
  std::vector<double> m_dividends;
};

void parse_forwards(
    std::istream &input,
    const std::unordered_map<std::string, Discount> &discount,
    std::unordered_map<std::string, ForwardDividend> &forwards);