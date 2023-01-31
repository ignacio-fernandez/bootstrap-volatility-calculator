#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <charconv>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "parse.h"

// 
// Functions to trim white space from a function
//
// see https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
// for a more detailed discussion
//
// trim white space from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim white space from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim white space from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

using namespace std;
vector<string> split_string(const string &line, char sep) {
    vector<string> ret;
    size_t i0=0; // begin first item
    for (size_t i1=i0;i1<line.size();i1++) {
        if (line[i1]==sep) {
            string item=line.substr(i0,i1-i0);
            trim(item); // remove white space from beginning and end
            ret.push_back(item);
            i1++; //skip separator
            i0=i1; // new strin start
        }
    }
    // item after last comma
    string item=line.substr(i0,line.size());
    ret.push_back(item);
    return ret;
}


double parse_double(const string &item) {
    size_t pos;
    double value=stod(item,&pos);
    if (pos!=item.size()) {
        ostringstream ss;
        ss<<"Can not parse whole string '"<<item;
        ss<<"' to double, left over '"<<item.substr(pos)<<"'";
        throw std::runtime_error(ss.str());
    }
    return value;
}

double parse_double0(const string &item) {
    double value;
    auto begin=item.data();
    auto end=item.data()+item.size();
    auto res=from_chars(begin,end,value);
    if (res.ec!=errc()) {
        ostringstream ss;
        ss<<"Can not parse  double from  '"<<item<<"'";
        throw std::runtime_error(ss.str());
    }
    if (res.ptr!=end) {
        ostringstream ss;
        ss<<"Can not parse whole string '"<<item;
        ss<<"' to double, left over '"<<string(res.ptr,end)<<"'";
        throw std::runtime_error(ss.str());
    }  
    return value;   
}

double parse_bool(const std::string & item,const std::string & true_str,const std::string & false_str) {
    if (item==true_str) {
        return true;
    }
    else if (item==false_str) {
        return false;
    }
    else {
        ostringstream ss;
        ss<<"parse_bool: string '"<<item<<"' is neither true ("<<true_str<<") nor false ("<<false_str<<").";
        throw std::runtime_error(ss.str());
    }
}
