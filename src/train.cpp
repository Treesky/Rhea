/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:31
 * @file train.cpp
 * @description 
 */

#include <iostream>

#include "fm_rec_algorithm.h"
#include "l1rlr_owlqn_algorithm.h"
#include "util.h"
#include "evaluator.h"
#include "auc_onepass_algorithm.h"

using namespace Rhea;

void exit_with_help()
{
    printf("%s\n", "Usage: ./train algorithm_type [options] train_set_file model_file");
    printf("%s\n", "    algorithm_type:");
    printf("%s\n", "        fm_rec (factor model for recommendation)");
    printf("%s\n", "        afm_rec (auto-regular factor model for recommendation)");
    printf("%s\n", "        l1rlr_owlqn (owlqn for l1-lr)");
    printf("%s\n", "        l2rlr_sgd (sgd for l2r-lr)");
    printf("%s\n", "        l2r_newton (newton method for l2r-lr)");
    printf("%s\n", "        l2rlr_auto-regular-sgd (auto regular-sgd for l2r-lr)");
    printf("%s\n", "        l1rlr_cdn (cdn for l1r-lr)");
    printf("%s\n", "        l1rlr_glmnet (glmnet for l1r-lr)");
    printf("%s\n", "        one_pass_auc_sgd ( for one-pass-auc-optim)");
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        exit_with_help();
        return 0;
    }

    char * prob_type = argv[1];

    if ( 0 == strcmp(prob_type, "fm_rec") )
    {
        SparseData<double> * dt = new SparseData<double>();
        dt->load_from_txt("heart_scale");
        Rhea::FMRecAlgorithm fm_rec_algo;
        fm_rec_algo.set_data(dt);
        fm_rec_algo.init();
        fm_rec_algo.train();
        fm_rec_algo.save_model("fm_rec.model.out");
        AucEvaluator auc_eval;
        std::cout << "AUC Evaluation: " << auc_eval.evaluate(fm_rec_algo, *dt) << std::endl;
        MaeEvaluator mae_eval;
        std::cout << "Mae Evaluation: " << mae_eval.evaluate(fm_rec_algo, *dt) << std::endl;
    }
    else if ( 0 == strcmp(prob_type, "l1rlr_owlqn") )
    {
        SparseData<double> * dt = new SparseData<double>();
        dt->load_from_txt("heart_scale");
        Rhea::L1rlrOwlqnAlgorithm owlqn_algo;
        owlqn_algo.set_data(dt);
        owlqn_algo.init();
        owlqn_algo.train();
        owlqn_algo.save_model("owlqn.model.out");
        AucEvaluator auc_eval;
        std::cout << "AUC Evaluation: " << auc_eval.evaluate(owlqn_algo, *dt) << std::endl;
        MaeEvaluator mae_eval;
        std::cout << "Mae Evaluation: " << mae_eval.evaluate(owlqn_algo, *dt) << std::endl;
    }
    else if ( 0 == strcmp(prob_type, "one_pass_auc_sgd") )
    {
        SparseData<double> * dt = new SparseData<double>();
        dt->load_from_txt("heart_scale");
        Rhea::AucOnepassAlgorithm auc_algo;
        auc_algo.set_data(dt);
        auc_algo.init();
        auc_algo.train();
        auc_algo.save_model("auc.model.out");
        AucEvaluator auc_eval;
        std::cout << "AUC Evaluation: " << auc_eval.evaluate(auc_algo, *dt) << std::endl;
        MaeEvaluator mae_eval;
        std::cout << "Mae Evaluation: " << mae_eval.evaluate(auc_algo, *dt) << std::endl;
    }
    else
    {
        exit_with_help();
    }

    return 0;
}
