#pragma once
#include <string>
#include <iostream>
//
//
//
//
class Date {
public: Date() = default;
    Date(int yyyy, int mm, int dd);
    Date(const std::string & date);  
    // copy constructor
    Date(const Date &date) : m_days(date.m_days){};
    // number of days between two dates as a double
    friend double operator-(Date lhs, Date rhs);
    // increase date by <days> number of days
    friend Date operator+(Date lhs, double days);
    // test ordering of dates
    friend bool operator==(Date lhs, Date rhs);
    friend bool operator!=(Date lhs, Date rhs);
    friend bool operator<(Date lhs, Date rhs);
    // read and write from stream
    friend std::ostream &operator<<(std::ostream &os,Date date);
    friend std::istream &operator>>(std::istream &os,Date &date);
private:
    double m_days; };

