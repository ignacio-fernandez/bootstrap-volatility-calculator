#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "parse.h"
#include "Discount.h"
#include "Forward.h"
#include "Option.h"
#include "black_scholes.h"

template <class Forward>
std::tuple<std::vector<OptionPrice>,
           std::unordered_map<std::string, Discount>,
           std::unordered_map<std::string, Forward>>
parse_data(std::string options_filename,
           std::string discounts_filename,
          std::string forwards_filename)
{
    std::ifstream options_file(options_filename);
    if (!options_file)
    {
        std::ostringstream ss;
        ss << "Could not open option prices file from '" << options_filename;
        throw std::runtime_error(ss.str());
    }
    auto option_prices = parse_options_and_prices(options_file);
    options_file.close();


    std::ifstream discounts_file(discounts_filename);
    if (!discounts_file)
    {
        std::ostringstream ss;
        ss << "Could not open discounts file from '" << discounts_filename;
        throw std::runtime_error(ss.str());
    }
    auto discounts = parse_discounts(discounts_file);
    discounts_file.close();

    std::ifstream forwards_file(forwards_filename);
    if (!forwards_file)
    {
        std::ostringstream ss;
        ss << "Could not open forwards file from '" << forwards_filename;
        throw std::runtime_error(ss.str());
    }
    std::unordered_map<std::string, Forward> forwards;
    parse_forwards(forwards_file, discounts, forwards);
    forwards_file.close();

    return {option_prices, discounts, forwards};
}


std::vector<std::pair<Date,double>>
bootstrap_volatility(
    std::vector<OptionPrice>::const_iterator begin,
    std::vector<OptionPrice>::const_iterator end,
    const Discount &discount,
    const ForwardYield &forward)
{
	std::vector<std::pair<Date, double>> volatilities;
	for (auto it = begin; it != end; ++it) {
		Option option = it->option;
		double price = it->price;


		double df = discount.discount(option.expiry);
		double fwd_price = price / df;

		double F = forward.forward(option.expiry);
		double T = (option.expiry - discount.valuation_date()) / 365.0;

		double iv = bs_implied_vol(option.is_call, option.strike, T, F, fwd_price);

		volatilities.push_back({option.expiry, iv});
	}

	return volatilities;
}	


using namespace std;
int main(int argc, const char *argv[]) {
    if (argc!=4) {
        cerr<<"usage: "<<argv[0]<<" <option prices>  <discounts> <forwards>"<<endl;
        return -1;
    }   
    string options_filename=argv[1];
    string discounts_filename=argv[2];
    string forwards_filename=argv[3];
    
    auto [option_prices,discounts,forwards]=
             parse_data<ForwardYield>(
              options_filename,
              discounts_filename,
              forwards_filename);

    sort(option_prices.begin(), option_prices.end(), 
		    [](const OptionPrice &lhs, const OptionPrice &rhs) {
		    	if (lhs.option.underlying == rhs.option.underlying)
				return lhs.option.expiry < rhs.option.expiry;
			return lhs.option.underlying < rhs.option.underlying;
		    });

    ofstream output("volatilities.csv");
    if(!output)
    {
        std::ostringstream ss;
        ss << "Could not open forwards file from volatilities.csv";
        throw runtime_error(ss.str());
    }


    auto begin = option_prices.begin();
    while (begin != option_prices.end())
    {
	    string underlying = begin->option.underlying;
	    Discount disc = discounts.at(begin->option.currency);
	    ForwardYield fwd = forwards.at(underlying);

	    auto end = find_if(begin, option_prices.end(), 
			    [underlying](const OptionPrice &op) {
			    	return underlying != op.option.underlying;
			    });


	    auto volatilities = bootstrap_volatility(begin, end, disc, fwd);

	    output << underlying << endl;
	    for(auto it = volatilities.begin(); it != volatilities.end(); ++it)
	    {
		    Date d = get<Date>(*it);
		    double v = get<double>(*it);

		    output << d << "," << v << endl;
	    }

	    output << endl;

	    begin = end;
    }

    return 0;
}
