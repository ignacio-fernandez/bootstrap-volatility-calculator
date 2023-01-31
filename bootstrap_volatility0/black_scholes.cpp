#include <cmath>

#include "bisect.h"

double ndtr(double z) {
    return 0.5*(1+std::erf(z/std::sqrt(2)));
};

//
// Black's pricing formula:
// European option  forward (undiscounted) price as a function of
//    the asset's forwar
// <isCall> true for calls , false for puts
// <K> option strike  
// <T> option expiry in years
// <F> forward of the options underlying asset
// <sigma> underlying's  volatility
// returns forwared (undiscounted)  option's price
//


double bs_price_fwd(bool is_call, double K, double T,
                    double F, double sigma) {  
    double ds=std::max(0.000001,sigma*std::sqrt(T));
    double dsig=0.5*ds*ds;
    double d2=(log(F/K)-dsig)/ds;
    double d1=d2+ds;
    double opt;
    if (is_call)
        opt= F*ndtr(d1) - K*ndtr(d2);
    else
        opt= K*ndtr(-d2) - F*ndtr(-d1);
    return opt;
}

double bs_implied_vol(bool is_call, double K, double T,
		double F, double fwd_price, double tol=1e-8)
{
	return bisect(
	    [&](double sigma) { return bs_price_fwd(is_call, K, T, F, sigma); }, fwd_price, 0, 1, tol);
}
