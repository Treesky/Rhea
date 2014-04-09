/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:04
 * @file algorithm.h
 * @description: Algorithm Iterface
 */

#ifndef _ALGORITHM_
#define _ALGORITHM_

#include <iostream>

#include "data.h"

namespace Rhea
{

class Algorithm
{
protected:
    Data<double> *_data;

public:

    virtual int save_model(std::string filename) = 0;
    virtual int set_data(Data<double> *data);
    virtual int init();
    virtual ~Algorithm();
    virtual double predict(Vec<double> &ins) = 0;
    virtual int train() = 0;
};

}

#endif
