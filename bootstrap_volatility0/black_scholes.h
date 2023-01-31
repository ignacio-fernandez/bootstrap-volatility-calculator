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
double bs_price_fwd(bool isCall, double K, double T, 
          double F, double sigma);

//
// Implied Volatility of and  European option  
// as a functio of forward (undiscounted) option price 
//
// <isCall> true for calls , false for puts
// <K> option strike  
// <T> option expiry in years
// <F> forward of the options underlying asset
// <fwd_price> option price divided by discount factor
// returns implied volatility <sigma> that
// matches the <fwd_price> 
// within at most <tol> price error
//
double bs_implied_vol(bool is_call, double K, double T,
                          double F, double fwd_price,
                          double tol=1e-8);