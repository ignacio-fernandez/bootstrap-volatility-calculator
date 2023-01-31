#pragma once
#include <string>
#include <vector>

std::vector<std::string> split_string(const std::string &line,char sep=',');
double parse_double(const std::string & item );
double parse_bool(const std::string & item,const std::string & true_str,const std::string & false_str);
