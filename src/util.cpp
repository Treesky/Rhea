/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:31
 * @file util.cpp
 * @description 
 */

#include "util.h"

uint32_t sign_str(std::string line)
{
    // Donald E. Knuth: The Art of Computer Programming, volumes 3
    size_t h = line.size();
    for (size_t i = 0; i < line.size(); ++i )
    {
        h = ((h << 5) ^ (h >> 27)) ^ line[i];
    }
    return h;
}

uint32_t sign_32_24(uint32_t sign)
{
    static const uint32_t MASK24 = ((uint32_t) 1 << 24) - 1;
    uint32_t sign3 = sign >> 24 & 0xFF;
    sign = sign & MASK24;
    sign ^= sign3;
    sign ^= sign3 << 4;
    sign ^= sign3 << 8;
    sign ^= sign3 << 12;
    sign ^= sign3 << 16;
    return sign;
}

double ran_uniform()
{
    return rand()/((double)RAND_MAX + 1);
}

double ran_gaussian()
{
    // Joseph L. Leva: A fast normal Random number generator
    double u,v, x, y, Q;
    do {
        do {
            u = ran_uniform();
        } while (u == 0.0); 
        v = 1.7156 * (ran_uniform() - 0.5);
        x = u - 0.449871;
        y = std::abs(v) + 0.386595;
        Q = x*x + y*(0.19600*y-0.25472*x);
        if (Q < 0.27597) { break; }
    } while ((Q > 0.27846) || ((v*v) > (-4.0*u*u*std::log(u)))); 
    return v / u;
}

double ran_gaussian(double mean, double stdev)
{
    if ((stdev == 0.0) || (std::isnan(stdev))) {
        return mean;
    } else {
        return mean + stdev*ran_gaussian();
    }
}

namespace Rhea
{

Sign2FeatureID * Sign2FeatureID::_p_ins = NULL;
Sign2FeatureID::CGabor Sign2FeatureID::_cgabor;

}
