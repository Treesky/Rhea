/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:31
 * @file fm_rec_algorithm.cpp
 * @description 
 */

#include "fm_rec_algorithm.h"

namespace Rhea
{

int Algorithm::set_data(Data<double> *data)
{
    _data = data;
    return 0;
}

int Algorithm::init()
{
    _data = NULL;
    return 0;
}

Algorithm::~Algorithm()
{
    DEL_POINTER(_data);
}

FMRecAlgorithm::FMRecAlgorithm()
{
    w0 = 0.0;

    reg0 = 0.0;
    regv = 0.0;
    regw = 0.0;

    _num_factor = 1;
    _iters = 1;
    _learn_rate = 0;

    _init_mean = 0;
    _init_stdev = 0.1;
}

int FMRecAlgorithm::init()
{
    w0 = 0.0;
    reg0 = regv = regw = 0.0;
    _num_factor = 8;
    _iters = 1;

    SparseData<double> * local_data = (SparseData<double> *) _data;
    w.set_size(local_data->feature_num());
    w.init(0);

    v.set_size(local_data->feature_num(), _num_factor);
    v.init(_init_mean, _init_stdev);

    sum.set_size(_num_factor);
    sum_square.set_size(_num_factor);

    _learn_rate = 0.1;
    return 0;
}

int FMRecAlgorithm::train()
{
    std::cout << "start training FMRec Model" << std::endl; 
    SparseData<double> * dt = (SparseData<double> *)_data;
    for (uint32_t iter_idx = 0; iter_idx < _iters; ++iter_idx)
    {
        for (uint32_t ins_idx = 0; ins_idx < dt->_matrix.rows(); ++ins_idx)
        {
            SparseVec<double> instance = static_cast< SparseVec<double>& >(dt->get_ins(ins_idx));

            double p = predict(instance);
            double mult = dt->get_label(ins_idx) ;
            mult = -mult * (1.0 - 1.0 / (1.0 + exp( -mult * p)));

            w0 = w0 - _learn_rate * (mult + reg0 * w0);

            for (uint32_t fea_idx = 0; fea_idx < instance.length(); ++fea_idx )
            {
                w[instance[fea_idx].idx] = w[instance[fea_idx].idx] - _learn_rate * (mult * instance[fea_idx].value + regw * w[instance[fea_idx].idx]);
            }

            for (uint32_t factor_idx = 0; factor_idx < _num_factor; ++factor_idx )
            {
                for (uint32_t fea_idx = 0; fea_idx < instance.length(); ++fea_idx )
                {
                    double grad = instance[fea_idx].value * (sum[factor_idx] - v[instance[fea_idx].idx][factor_idx] * instance[fea_idx].value);
                    v[instance[fea_idx].idx][factor_idx] = v[instance[fea_idx].idx][factor_idx] - _learn_rate * (mult * grad + regv * v[instance[fea_idx].idx][factor_idx]);
                }
            }
       }
    }
    std::cout << "end training FMRec model" << std::endl;
    return 0;
}

double FMRecAlgorithm::predict(Vec<double> &vec_ins)
{
    SparseVec<double> ins = static_cast<SparseVec<double> &>(vec_ins); 
    double result = 0;
    result += w0;
    for (uint32_t fea_idx = 0; fea_idx < ins.length(); ++fea_idx)
    {
        result += w[ins[fea_idx].idx] * ins[fea_idx].value;
    }
    for (uint32_t factor_idx = 0; factor_idx < _num_factor; ++factor_idx)
    {
        sum[factor_idx] = 0;
        sum_square[factor_idx] = 0;
        for  (uint32_t fea_idx = 0; fea_idx < ins.length(); ++fea_idx )
        {
            double d = v[ins[fea_idx].idx][factor_idx] * (ins[fea_idx].value);
            sum[factor_idx] += d;
            sum_square[factor_idx] += d * d;
        }
        result += 0.5 * (sum[factor_idx] * sum[factor_idx] - sum_square[factor_idx]);
    }
    return result;
}

int FMRecAlgorithm::save_model(std::string filename)
{
    SparseData<double> * local_data = (SparseData<double> *) _data;
    std::ofstream model_out(filename.c_str());
    if (!model_out.is_open())
    {
        std::cout << "open model file to write failed!" << std::endl;
    }
    model_out << reg0 << " " << regv <<  " " << regw << std::endl;
    model_out << w0 << std::endl;
    model_out << local_data->feature_num() << " " << _num_factor << std::endl;
    for (size_t i = 0; i < local_data->feature_num(); ++i )
    {
       model_out << w[i] << " "; 
    }
    model_out << std::endl;

    for (size_t i = 0; i < _num_factor; ++i )
    {
        for (size_t j = 0; j < local_data->feature_num(); ++j )
        {
            model_out << v[j][i] << " ";
        }
        model_out << std::endl;
    }
    
    model_out.close();
    return 0;
}

int FMRecAlgorithm::load_model(std::string filename)
{
    std::ifstream model_in(filename.c_str());
    if (!model_in.is_open())
    {
        std::cout << "model file open to read failed!" << std::endl;
        return -1;
    }
    model_in.close();

    std::string line;
    getline(model_in, line);
    sscanf(line.c_str(), "%lf %lf %lf", &reg0, &regv, &regw);
    return 0;
}

}
