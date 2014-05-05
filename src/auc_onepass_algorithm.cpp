/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-23 14:47
 * @file auc_onepass_algorithm.cpp
 * @description 
 */

#include "auc_onepass_algorithm.h"

namespace Rhea
{

AucOnepassAlgorithm::AucOnepassAlgorithm()
{
}

AucOnepassAlgorithm::~AucOnepassAlgorithm()
{
}

double AucOnepassAlgorithm::predict(Vec<double> &ins)
{
    SparseVec<double> instance = static_cast< SparseVec<double> &>(ins);
    double value = 0.0;
    for (uint32_t i = 0; i < instance.length(); i++)
    {
        value += w[instance[i].idx] * instance[i].value;
    }
    return value;
} 

int AucOnepassAlgorithm::init()
{
    uint32_t dim = _data->instance_num();
    center_p.set_size(dim);
    center_n.set_size(dim);
    out_matrix_p.set_size(dim, dim);
    out_matrix_n.set_size(dim, dim);
    w.set_size(dim);
    return 0;
}

int AucOnepassAlgorithm::save_model(std::string filename)
{
    SparseData<double> * local_data = (SparseData<double> *) _data;
    std::ofstream model_out(filename.c_str());
    if (!model_out.is_open())
    {
        std::cout << "Open model file to write failed!" << std::endl;
    }
    model_out << b << " " << eta << " " << local_data->feature_num() << std::endl;  
    for (uint32_t i = 0; i < local_data->feature_num(); ++i)
    {
        model_out << w[i] << " ";
    }
    model_out << std::endl;
    model_out.close();
    return 0;
}

int AucOnepassAlgorithm::load_model(std::string filename)
{
    uint32_t feature_num = 0;
    std::ifstream model_in(filename.c_str());
    if (!model_in.is_open())
    {
        std::cout << "Open model file to read failed!" << std::endl;
    }

    std::string line;
    std::string token;
    getline(model_in, line);

    sscanf(line.c_str(), "%lf %lf %u", b, eta, feature_num);
    getline(model_in, line);

    std::stringstream stringsplitter(line.c_str());
    uint32_t idx = 0;
    while ( std::getline(stringsplitter, token, ' '))
    {
        w[idx++] = atof(token.c_str());
    }
    model_in.close();

    return 0;
}

int AucOnepassAlgorithm::train()
{
    uint32_t dim = _data->feature_num();
    num_p = 0;
    num_n = 0;
    double wx = 0.0;
    double w_center_n = 0.0;
    double w_center_p = 0.0;
    DenseVec<double> w_outmatrix_n;
    w_outmatrix_n.set_size(dim);
    DenseVec<double> w_outmatrix_p;
    w_outmatrix_p.set_size(dim);

    std::cout << "Start training AucOnepass Model" << std::endl;
    SparseData<double>  dt = static_cast< SparseData<double> &> (*_data);
    for (uint32_t iter_idx = 0; iter_idx < iters; ++iter_idx)
    {
        for (uint32_t ins_idx = 0; ins_idx < dt.instance_num(); ++ins_idx)
        {
            wx = 0.0;
            w_center_n = 0.0;
            SparseVec<double> instance = static_cast< SparseVec<double> &> (dt.get_ins(ins_idx));
            if (_data->get_label(ins_idx) == 1)
            {
                num_p += 1;
                double value = 0;
                for (uint32_t i = 0, ins_i = 0; i < dim; ++i)
                {
                    if ( i == instance[ins_i].idx )
                    {
                        value = instance[i].value;
                        ins_i++;
                    }
                    else
                    {
                        value = 0.0;
                    }
                    center_p[i] += (value - center_p[i])/num_p;
                    wx += value * w[i];
                    w_center_n += center_n[i] * w[i];
                    double value_j = 0;
                    for (uint32_t j = 0, ins_j = 0; j < dim; ++j)
                    {
                        if ( j == instance[ins_j].idx )
                        { 
                            value_j = instance[i].value;
                        }
                        else
                        {
                            value_j = 0.0;
                        }
                        out_matrix_p[i][j] += (value * value_j - out_matrix_p[i][j])/num_p;
                    }
                    w_outmatrix_n[i] = 0.0;
                } 
                // calc w_outmatrix_n,
                for (uint32_t i = 0; i < dim; ++i)
                {
                    for (uint32_t j = 0; j < dim; ++j)
                    {
                        w_outmatrix_n[i] += w[i] * out_matrix_n[j][i];
                    }
                }
                // update w[i]
                if (num_n > 0)
                {
                    for (uint32_t i = 0, ins_i = 0; i < dim; ++i)
                    {
                        if ( i == instance[ins_i].idx )
                        {
                            value = instance[i].value;
                            ins_i ++;
                        }
                        else
                        {
                            value = 0.0;
                        }
                        w[i] += eta * (value - center_n[i] - wx * value - w_outmatrix_n[i] +  wx * center_n[i] + w_center_n * value - b * w[i]);
                    }
                }
            }
            else
            {
                num_n += 1;
                double value = 0;
                for (uint32_t i = 0, ins_i = 0; i < dim; ++i)
                {
                    if ( i == instance[ins_i].idx )
                    {
                        value = instance[i].value;
                        ins_i++;
                    }
                    else
                    {
                        value = 0.0;
                    }
                    center_n[i] += (value - center_n[i])/num_n;
                    wx += value * w[i];
                    w_center_p += center_p[i] * w[i];
                    double value_j = 0;
                    for (uint32_t j = 0, ins_j = 0; j < dim; ++j)
                    {
                        if ( j == instance[ins_j].idx )
                        { 
                            value_j = instance[i].value;
                        }
                        else
                        {
                            value_j = 0.0;
                        }
                        out_matrix_n[i][j] += (value * value_j - out_matrix_n[i][j])/num_n;
                    }
                    w_outmatrix_n[i] = 0.0;
                } 
                // calc w_outmatrix_n,
                for (uint32_t i = 0; i < dim; ++i)
                {
                    for (uint32_t j = 0; j < dim; ++j)
                    {
                        w_outmatrix_p[i] += w[i] * out_matrix_p[j][i];
                    }
                }
                // update w[i]
                if (num_n > 0)
                {
                    for (uint32_t i = 0, ins_i = 0; i < dim; ++i)
                    {
                        if ( i == instance[ins_i].idx )
                        {
                            value = instance[i].value;
                            ins_i ++;
                        }
                        else
                        {
                            value = 0.0;
                        }
                        w[i] += eta * (value - center_p[i] - wx * value - w_outmatrix_p[i] +  wx * center_p[i] + w_center_p * value - b * w[i]);
                    }
                }
 
            }
        }
    }
    return 0;
}

}
