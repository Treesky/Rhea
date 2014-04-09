/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:31
 * @file l1rlr_owlqn_algorithm.cpp
 * @description 
 */

#include "l1rlr_owlqn_algorithm.h"

namespace Rhea
{

double L1rlrOwlqnAlgorithm::predict(Vec<double> &ins)
{
    return 0.0;
}

int L1rlrOwlqnAlgorithm::init()
{
    if (NULL == _data)
    {
        std::cout << "in L1rlrOwlqnAlgorithm Data is NULL! Please init DATA before init ALGORITHM!" << std::endl;
        return -1;
    }
    else
    {
        _iters = 100;
        _alphas = new (std::nothrow) double[_data->feature_num()];
        if (_alphas == NULL)
        {
            DEL_ARRAY_POINTER(_alphas);
        }
        _dir.set_size(_data->feature_num());
        _steepest_dir.set_size(_data->feature_num());
        _sub_grad.set_size(_data->feature_num());
        _conver_threshold = 1e-5;
        _f_xk1 = 0;
        _f_xk = 0;
        _prev_val = std::numeric_limits<double>::infinity(); 
        _line_search_steps = 50;
        _l1weight = 1.0;
    }
    return 0;
}

int L1rlrOwlqnAlgorithm::save_model(std::string filename)
{
    std::ofstream model_out(filename.c_str());
    if (!model_out.is_open())
    {
        std::cout << "Open model file [" << filename << "] failed" << std::endl;
    }
    return 0;
}

int L1rlrOwlqnAlgorithm::load_model(std::string filename)
{
    return 0;
}

L1rlrOwlqnAlgorithm::L1rlrOwlqnAlgorithm()
{
}

int L1rlrOwlqnAlgorithm::newton_direction()
{
    uint32_t m = _ss.size();

    if (0 != m)
    {
        for (uint32_t i = 0; i < m; i--)
        {
            _alphas[i] = _dir.dot_product(*_ss[m-i-1]) / _rhos[m-i-1];
            _ys[m-i-1]->scale( - _alphas[i] );
            _dir.add_vec(*_ys[m-i-1]);
        }

        double scalar = _ys[m-1]->l2_norm();
        scalar = _rhos[m-1] / ( scalar * scalar);
        _dir.scale(scalar);
        double beta = 0.0;

        for (uint32_t i = m-1; i >= 0; ++i)
        {
            beta = _dir.dot_product(*_ys[m-i-1]) / _rhos[m-i-1];
            _ss[m-i-1]->scale(_alphas[i] - beta);
            _dir.dot_product( *_ss[m-i-1] );
        }
    }
    return 0;
}

int L1rlrOwlqnAlgorithm::orthant_projection()
{
    for (size_t i = 0; i < _data->feature_num(); ++i)
    {
        if (_dir[i] * _steepest_dir[i] <= 0)
        {
            _dir[i] = 0;
        } 
    }
    return 0;
}

bool L1rlrOwlqnAlgorithm::check_convergence()
{
    if ( _prev_val != std::numeric_limits<double>::infinity() )
    {
        double impro = (_prev_val - _f_xk) / fabs(_f_xk);
        std::cout << "Iter: "<< _iter << " stop criter: " << impro << " func value: " << _f_xk << std::endl;
        if (impro < _conver_threshold)
        {
           return true; 
        }
    }
    else
    {
        std::cout << " (first iter cannot evaluate)" << std::endl;
    }
    return false;
}

bool L1rlrOwlqnAlgorithm::check_direction()
{
    if ( 0 == _l1weight )
    {
        return _dir.dot_product(_g_xk);
    }
    else
    {
        double val = 0.0;
        for (size_t i = 0; i < _data->feature_num(); ++i)
        {
            val += _sub_grad[i] * _dir[i];
        }
        return val;
    }
    return true;
}

int L1rlrOwlqnAlgorithm::shift_xk_xk1()
{
    DenseVec<double> * s_xk1 = NULL, * y_xk1 = NULL;
    if (_ss.size() < _m)
    {
        s_xk1 = new (std::nothrow) DenseVec<double>();
        y_xk1 = new (std::nothrow) DenseVec<double>();
        if ( NULL == s_xk1 || NULL == y_xk1 )
        {
            std::cout << "locate memory for s_xk1, y_xk1 failed!" << std::endl;
            return -1;
        }
        s_xk1->set_size(_data->feature_num());
        y_xk1->set_size(_data->feature_num());
    }
    else
    {
        s_xk1 = _ss.front();
        _ss.pop_front();
        y_xk1 = _ys.front();
        _ys.pop_front();
        _rhos.pop_front();
    }

    for (uint32_t i = 0; i < _data->feature_num(); ++i)
    {
        (*s_xk1)[i] = _x_k1[i] - _x_k[i];
        (*y_xk1)[i] = _g_xk1[i] - _g_xk[i]; 
        _x_k[i] = _x_k1[i];
        _g_xk[i] = _g_xk1[i];
    }
    double rho = s_xk1->dot_product(*y_xk1);

    _ss.push_back(s_xk1);
    _ys.push_back(y_xk1);
    _rhos.push_back(rho);

    _prev_val = _f_xk;
    _f_xk = _f_xk1;

    return 0;
}

int calc_grad(const DenseVec<double> &ins, DenseVec<double> &grad)
{
    return 0;
}

int L1rlrOwlqnAlgorithm::train()
{
    // the main difference between this implementation and OWLQN paper, is we calc gradient after value
    // this is because the calc procedure between value & gradient is similar, we canc calc gradient after
    // value with a little calc cost
    double alpha = 0.0, backoff = 0.0;  
    _iter = 0;

    for (_iter = 0; _iter < _iters; ++_iter )
    {
        // 1.0 check convergence
        if (check_convergence())
        {
            std::cout << "OWLQN convergenced!" << std::endl;
            break;
        }
        // 2.0 get LBFGS Newton Direction
        // 2.1 tow-loop calc p_k = - H_k g_xk
        if (newton_direction() != 0)
        {
            std::cout << "Calc LBFGS Newton Direction failed!" << std::endl;
            break;
        }
        // 2.2 Orthant Projection
        if (orthant_projection() != 0)
        {
            std::cout << "Orthant projection failed!" << std::endl;
            break;
        }
        // 2.3 check search direction
        double dir_grad_product = check_direction();
        if (dir_grad_product >= 0)
        {
            std::cout << "Check Search Direction failed! Please check gradient & search direction!" << std::endl;
            break;
        }
        // 3.0 line search for step length alpha_k
        // 3.1 set alpha & backoff init parameter
        if ( 0 == _iter )
        {
            alpha = 1 / _dir.l2_norm();
            backoff = 0.1;
        }
        for (uint32_t line_search_idx = 0; line_search_idx < _line_search_steps; ++line_search_idx)
        {
            // 3.2 get Next Point: x_k1
            for (uint32_t i = 0; i < _data->feature_num(); ++i)
            {
                _x_k1[i] = _x_k[i] + _dir[i] * alpha;
                // 3.2.5 Point Orthant Constraint
                if (_x_k1[i] * _x_k[i] < 0)
                {
                    _x_k1[i] = 0.0;
                }
            }
            // 3.3 calc f_xk1
            _f_xk1 = predict(_x_k1);
            // 3.4 check line search quit
            if (_f_xk1 <= _f_xk + 1e-4 * dir_grad_product * alpha)
            {
                break;
            }
            alpha *= backoff;
        }
        // 3.5 calc g_xk1
        calc_grad(_x_k1, _g_xk1);
        // 4.0 shift k & k1
        if (shift_xk_xk1() != 0)
        {
            std::cout << "Shift xk xk1 state failed!" << std::endl;
            break;
        }
    } 
    std::cout << "OWLQN train complete!" << std::endl;
    return 0;
}

}
