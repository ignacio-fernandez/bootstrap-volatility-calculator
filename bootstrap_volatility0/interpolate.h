#pragma once

#include <vector>

// linearly interpolate x  on the set of values 
//        xs[0],ys[0]
//        xs[1],ys[1]
//        xs[2],ys[2]
//        ...
// with constant extrapolation 
//   ( returns ys[0] if x<xs[0], and ys[size-1] if x>xx[size-1])
//
// requires that
//       xs not empty
//       xs is sorted in increasing order
//       xs and ys have the same number of elements
double interpolate(double x, const std::vector<double> &xs, const std::vector<double> &ys);