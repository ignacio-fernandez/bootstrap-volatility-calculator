#pragma once
#include <sstream>
#include <exception>

template <typename T> 
double bisect(T func,double y,  double x0, double x1, 
              double xtol=1e-8) {
    double dy0=func(x0)-y;
    if (dy0==0.0) return x0;
    double dy1=func(x1)-y;
    if (dy1==0.0) return x1;   
    if (dy0*dy1>0) {
            std::ostringstream ss;
            ss<<"bisect: value "<<y<<" not bracketed between f(";
            ss<<x0<<")="<<dy0+y<<" and f("<<x1<<")="<<dy0+1;
            throw std::runtime_error(ss.str());
    }   
    while (1) {
        double x=(x0+x1)/2;
        double dy=func(x)-y;
        if (fabs(dy)<xtol)
	    return x; 
        if (dy==0.0) return x;
        if (dy0*dy<0) {
            x1=x;
        }
        else {
            x0=x;
        }
        x=(x0+x1)/2.0;
    }
}
