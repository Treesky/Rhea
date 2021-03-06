/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:07
 * @file fm_rec_algorithm.h
 * @description Recomendation Algorithm FM Model
 * Steffen Rendle (2010): Factorization Machines, in Proceedings of the 10th IEEE International Conference on Data Mining (ICDM 2010), Sydney, Australia. 
 */

#ifndef _FM_REC_ALGORITHM_
#define _FM_REC_ALGORITHM_

#include <iostream>

#include "data.h"
#include "algorithm.h"

namespace Rhea
{

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

    virtual double predict(Vec<double> &ins);
    virtual int init();
    virtual int save_model(std::string filename);
    virtual int load_model(std::string filename);
    FMRecAlgorithm();
    ~FMRecAlgorithm() {};
    virtual int train();
};

}
#endif /* _ALGORITHM_ */
