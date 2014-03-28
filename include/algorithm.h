#ifndef _ALGORITHM_
#define _ALGORITHM_

#include <iostream>

#include "data.h"

namespace Rhea
{

class Algorithm
{
protected:
    Data *_data;

public:

    virtual int set_data(Data *data);
    virtual int init();
    virtual ~Algorithm(); 
    virtual double predict() {return 0;}
    virtual int train() {return 0;}
};

class FMRecAlgorithm : public Algorithm
{
private:
    DenseVec<double> sum, sum_square;

    double w0;
    DenseVec<double> w;
    DenseMatrix<double> v;

    double reg0;
    double regv;
    double regw;

    DenseVec<double> _label;

    uint32_t _num_factor;
    uint32_t _iters;
    double _learn_rate;
    double _init_mean;
    double _init_stdev;

public:

    double predict(SparseVec<double> &ins);
    int init();
    FMRecAlgorithm();
    ~FMRecAlgorithm() {};
    int train();
};

}
#endif /* _ALGORITHM_ */
