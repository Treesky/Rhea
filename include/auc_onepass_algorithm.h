/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-23 14:48
 * @file auc_onepass_algorithm.h
 * @description One-Pass AUC Optimization
 * Wei Gao (2013): One-Pass AUC Optimization, in Proceedings of the 30th International Conference on Machine Learning, Atlanta, Georgia, USA.  
 */

#ifndef _AUC_ONEPASS_ALGORITHM_
#define _AUC_ONEPASS_ALGORITHM_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "data.h"
#include "algorithm.h"

namespace Rhea
{

class AucOnepassAlgorithm : public Algorithm
{
private:
    double num_p;
    double num_n;
    double b;
    double eta;
    DenseVec<double> center_p;
    DenseVec<double> center_n;
    DenseMatrix<double> out_matrix_p;
    DenseMatrix<double> out_matrix_n;
    DenseVec<double> w;
    uint32_t iters;
public:
    virtual double predict(Vec<double> &ins);
    virtual int init();
    virtual int save_model(std::string filename);
    virtual int load_model(std::string filename);
    AucOnepassAlgorithm();
    ~AucOnepassAlgorithm();
    virtual int train();
};

}

#endif
