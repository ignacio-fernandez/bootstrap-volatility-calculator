#include <vector>
#include <cstddef>
#include <algorithm>

#include "interpolate.h"



// return linearly interpolation of y between ys[lb-1] and ys[lb]
// with flat extrapolation if x<=xs.front() or x>=xs.back();
// requires that:
//      array xs can not be empty
//      xs  and ys arrays must have the same size
double interpolate_lb(double x, size_t lb,
                      const std::vector<double> &xs, const std::vector<double> &ys) {
    if (lb==0) {
        return ys.front();
    }
    if (lb==ys.size()) {
        return ys.back();
    }
    else {
        double x0=xs[lb-1];
        double x1=xs[lb];
        double y0=ys[lb-1];
        double y1=ys[lb];
        return y0 + (x-x0)*(y1-y0)/(x1-x0);
    }

}

// linearly interpolate x  on the set of values 
//        xs[0],ys[0]
//        xs[1],ys[1]
//        xs[2],ys[2]
//        ...
// with constant flat extrapolation 
//   ( returns ys.front() if x<xs.front(), and ys.back() if x>xs.back())
//
// requires that
//       xs not empty
//       xs is sorted in increasing order
//       xs and ys have the same number of elements
double interpolate(double x, 
                   const std::vector<double> &xs,
                   const std::vector<double> &ys) {
    auto it=std::lower_bound(xs.begin(),xs.end(),x);
    auto lb=it-xs.begin();
    return interpolate_lb(x,lb,xs,ys);
}

#include <iostream>

using namespace std;
int test() {
    vector<double> xs={0,1,2,3,4.0};
    vector<double> ys={1,4,9,16.0,25.0};
    vector<double> tests={-1.0,-0.1,0.0,0.1,1.5,2.0,3.9,4.0,5.0};
    for (double x:tests) {
        auto ubp=std::lower_bound(begin(xs),end(xs),x);
        cout<<"test value: "<<x<<"lower bound = "<<ubp-begin(xs)<<endl;
        cout<<"\t interpolated y ="<<interpolate(x,xs,ys)<<endl;
    }
    return 0;
}