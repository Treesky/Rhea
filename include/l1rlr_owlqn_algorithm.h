/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:10
 * @file l1rlr_owlqn_algorithm.h
 * @description OWLQN for l1-regularized logistic regression
 *  Galen Andrew, Jianfeng Gao (2007): Scalable Training of L1-Regularized Log-Linear Models, in Proceedings of the 24 th International Conference on Machine Learning (ICML 2007), Corvallis, OR. 
 */

#ifndef _L1RLR_OWLQN_ALGORITHM_
#define _L1RLR_OWLQN_ALGORITHM_

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <deque>

#include "data.h"
#include "algorithm.h"

namespace Rhea
{

class L1rlrOwlqnAlgorithm : public Algorithm
{
private:
    uint32_t _iters;
    uint32_t _iter;
    uint32_t _m;
    std::deque< DenseVec<double>* > _ys;
    std::deque< DenseVec<double>* > _ss;
    std::deque<double> _rhos;
    std::deque<double> _pre_vals;
    double * _alphas;
    DenseVec<double> _dir;
    DenseVec<double> _steepest_dir;
    double _conver_threshold;
    DenseVec<double> _g_xk1;
    DenseVec<double> _g_xk;
    DenseVec<double> _x_k1;
    DenseVec<double> _x_k;
    DenseVec<double> _sub_grad;
    double _f_xk1;
    double _f_xk;
    double _prev_val;
    uint32_t _line_search_steps;
    double _l1weight;
    double _l2weight;

    int newton_direction();
    int orthant_projection();
    double check_direction();
    bool check_con_convergence();
    bool check_avg_convergence();

    int shift_xk_xk1();
    int l1_direction();

public:
    virtual double predict(Vec<double> &ins);
    virtual int init();
    virtual int save_model(std::string filename);
    virtual int load_model(std::string filename);
    int lr_func_val(DenseVec<double> &w, double &func_val, DenseVec<double> &gradient); 
    L1rlrOwlqnAlgorithm();
    ~L1rlrOwlqnAlgorithm();
    virtual int train();
};

}

#endif
