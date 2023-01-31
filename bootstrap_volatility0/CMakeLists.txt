cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

add_executable(test_implied_volatility0 test_implied_volatility0.cpp                              
                              black_scholes.h black_scholes0.cpp
              )

add_executable(bootstrap_volatility0 bootstrap_volatility0.cpp
                              bisect.h
                              interpolate.h interpolate.cpp                              
                              black_scholes.h black_scholes0.cpp
                              parse.h parse.cpp
                              Date.h Date.cpp
                              Discount.h Discount.cpp 
                              Forward.h parse_forwards.cpp
                              Option.h Option.cpp
                              )

