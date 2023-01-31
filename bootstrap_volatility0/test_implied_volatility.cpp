#include <iostream>
#include <fstream>
#include <cmath>

#include "black_scholes.h"

using namespace std;
int main() {
    bool is_call=true;
    double T=0.5;
    double strike=1.05;
    double df=0.98;
    double F=1.01;

    ofstream output("implied_volatility.csv");
    if (!output) {
	    cout << "cannot open file" << endl;
	    return 1;
    }

    output<<"price,volatility,error"<<endl;
    for (double price=0.;price<0.11;price+=0.005) {
	    double fwd_price = price / df;
	    double iv = bs_implied_vol(is_call, strike, T, F, fwd_price, 1e-12);
	    double error = abs(bs_price_fwd(is_call, strike, T, F, iv) - fwd_price);
	    output << price << "," <<  iv << "," << error << endl;
    }
}
