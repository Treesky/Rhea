#include <iostream>

#include "fm_rec_algorithm.h"
#include "util.h"
#include "evaluator.h"

using namespace Rhea;

void exit_with_help()
{
    printf("%s\n", "Usage: ./train algorithm_type [options] train_set_file model_file");
    printf("%s\n", "    algorithm_type:");
    printf("%s\n", "        fm_rec (factor model for recommendation)");
    printf("%s\n", "        afm_rec (auto-regular factor model for recommendation)");
    printf("%s\n", "        l1rlr_owlqn (owlqn for l1-lr)");
    printf("%s\n", "        l2r_lr_sgd (sgd for l2r-lr)");
    printf("%s\n", "        l2r_newton (newton method for l2r-lr)");
    printf("%s\n", "        l2r_lr_auto-regular-sgd (sgd for l2r-lr)");
    printf("%s\n", "        l1rlr-cdn (cdn for l1r-lr)");
    printf("%s\n", "        l1rlr-glmnet (glmnet for l1r-lr)");
    printf("%s\n", "        sgd-auc ( for auc-optim)");
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
        AucEvaluator eval;
        std::cout << "AUC Evaluation: " << eval.evaluate(fm_rec_algo, *dt) << std::endl;
        MaeEvaluator mae;
        std::cout << "Mae Evaluation: " << mae.evaluate(fm_rec_algo, *dt) << std::endl;
    }
    else
    {
        exit_with_help();
    }

    return 0;
}
