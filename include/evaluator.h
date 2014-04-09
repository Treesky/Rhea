/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:06
 * @file evaluator.h
 * @description Evaluation for Algorithm Model; AUC, margin-AUC, MSE 
 */

#ifndef _EVALUATOR_
#define _EVALUATOR_

#include <iostream>
#include <algorithm.h>
#include <data.h>

namespace Rhea
{

struct EvalPair
{
    double pre_value;
    double act_value;
};
 
class Evaluator
{
public:
    virtual double evaluate(Algorithm &algo, Data<double> &data) = 0; 
    virtual ~Evaluator() {}
};

class AucEvaluator : public Evaluator
{
private:
    struct EvalPairAucComp
    {
        bool operator () (const EvalPair &a, const EvalPair &b)
        {
            return a.pre_value < b.pre_value;
        }
    };
public:
    double evaluate(Algorithm &algo, Data<double> &data)
    {
        std::vector<EvalPair> vec;
        for (uint32_t i = 0; i < data.instance_num(); ++i )
        {
            double value = algo.predict(data.get_ins(i));
            EvalPair pp;
            pp.pre_value = value;
            pp.act_value = data.get_label(i);
            vec.push_back(pp);
        }
        // sort
        std::sort(vec.begin(), vec.end(), EvalPairAucComp()); 
        double rank_ins = 0, M = 0, N = 0;
        // calc
        for ( size_t i = 0; i < vec.size(); ++i )
        {
            if (vec[i].act_value > 0)
            {
                rank_ins += i;
                M += 1;
            }
            else
            {
                N += 1;
            }
        }
        double auc = (rank_ins - M * (M + 1) / 2) / (M * N);
        return auc;
    }
};

class MaeEvaluator : public Evaluator
{
public:
    double evaluate(Algorithm &algo, Data<double> &data)
    {
        double mae = 0.0, N = 0.0,
               act_value = 0.0, pre_value = 0.0;
        for (uint32_t i = 0; i < data.instance_num(); ++i )
        {
            act_value = data.get_label(i);
            pre_value = algo.predict(data.get_ins(i));
            mae += (act_value - pre_value) * (act_value - pre_value);
            N += 1;
        }
        return mae / N;
    }
};

}

#endif
