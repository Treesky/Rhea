/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:05
 * @file data.h
 * @description Common Data Structure; SparseMatrix; DenseMatrix; SparseVec; DenseVec; SparseAtom 
 */

#ifndef _DATA_
#define _DATA_

#include <iostream>
#include <vector>

#include "util.h"

namespace Rhea
{

template <class T>
struct SparseAtom
{
    T value;
    uint32_t idx;
};

template <class T>
class Vec
{
};

template<class T>
class DenseVec : public Vec<T>
{
private:
    T * _atoms;
    uint32_t _length;

public:
    DenseVec() : _atoms(NULL), _length(0) {}

    ~DenseVec()
    {
        DEL_ARRAY_POINTER(_atoms);
    }

    uint32_t length()
    {
        return _length;
    }

    T& operator[] (uint32_t i)
    {
        if (i >= 0 && i < _length)
        {
            return _atoms[i];
        }
        else
        {
            printf("operator [%u] for DenseVec failed, total size %u\n", i, _length);
            return _atoms[0];
        }
    }

    int init(const T val)
    {
        for (uint32_t i = 0; i < _length; ++i)
        {
            _atoms[i] = val;
        }
        return 0;
    }
        
    int set_size(const uint32_t length)
    {
        DEL_ARRAY_POINTER(_atoms);
        _length = length;
        if (0 != length)
        {
            _atoms = new (std::nothrow) T[_length];
            if (NULL == _atoms)
            {
                LOG_WARNING("new DenseVec failed!");
                return -1;
            }
        }
        return 0;
    }

    T l2_norm()
    {
        T value = 0;
        for (uint32_t i = 0; i < _length; ++i)
        {
            value += _atoms[i] * _atoms[i];
        }
        return sqrt(value);
    }

    T l1_norm()
    {
        T value = 0;
        for (uint32_t i = 0; i < _length; ++i)
        {
            value += (_atoms[i] > 0 ? _atoms[i] : -_atoms[i]);
        }
        return value;
    }

    T dot_product(DenseVec<T> &oper)
    {
        if (_length != oper.length())
        {
            std::cout << "Dot product operator should occured between two vector with same dimention! [" << _length <<
               "] * [" << oper.length() << "]"  << std::endl;
            return 0;
        }
        else
        {
            T value = 0;
            for (uint32_t i = 0; i < oper.length(); ++i)
            {
                value += _atoms[i] * oper[i];
            }
            return value;
        }
    }

    int add_vec(DenseVec<T> &oper)
    {
        for (uint32_t i = 0; i < _length; ++i)
        {
            _atoms[i] += oper[i];
        }
        return 0;
    }

    int add_mult(DenseVec<T> &oper_vec, double scalar)
    {
        for (uint32_t i = 0; i < _length; ++i)
        {
            _atoms[i] += (oper_vec[i] * scalar);
        }
        return 0;
    }

    int scale(const double &oper)
    {
        for (uint32_t i = 0; i < _length; ++i )
        {
            _atoms[i] *= oper;
        }
        return 0;
    }
};



template <class T>
class SparseVec : public Vec<T>
{
private:
    SparseAtom<T> * _atoms;
    uint32_t _length;

public:
    SparseVec() : _atoms(NULL), _length(0) { }

    uint32_t length() { return _length;}
    int set_length(const uint32_t &length) { _length = length; return 0;}
    int set_atoms(SparseAtom<T> * atoms) { _atoms = atoms; return 0;}
    int set_size(uint32_t length);
    ~SparseVec() {}

    SparseAtom<T>& operator[] (uint32_t i)
    {
        if (i >= 0 && i < _length)
        {
            return _atoms[i];
        }
        else
        {
            printf("operator [%u] for SparseVec failed, total size %u\n", i, _length);
            return _atoms[0];
        }
    }

    double dot_product(DenseVec<double> &instance)
    {
        double value = 0.0;
        for (uint32_t i = 0; i < _length; ++i)
        {
            value += instance[_atoms[i].idx] * _atoms[i].value;
        }
        return value;
    }

    double dot_product(SparseVec<double> &instance)
    {
        double value = 0.0;
        // two pointer, need sorted
        uint32_t i = 0, j = 0;
        while ( i < _length && j < instance.length() )
        {
            if (_atoms[i].idx == instance[j].idx )
            {
                value += _atoms[i].value * instance[j].value;
            }
            else if (_atoms[i].idx < instance[j].idx)
            {
                ++i;
            }
            else if (_atoms[i].idx > instance[j].idx)
            {
                ++j;
            }
        }
        return value;
    }
};

template <class T>
class SparseMatrix
{
private:
    SparseVec <T> * _vecs;
    uint32_t _rows;
    uint32_t _columns;

public:
    SparseMatrix() : _vecs(NULL), _rows(0), _columns(0) { }

    ~SparseMatrix()
    {
        DEL_ARRAY_POINTER(_vecs);
    }

    uint32_t columns() { return _columns; }
    int set_columns(uint32_t columns) { _columns = columns; return 0; } 
    uint32_t rows() {return _rows; }
    int set_rows(uint32_t rows) { _rows = rows; return 0; }
    int set_size(uint32_t rows);

    // NOTICE: the Inverse of SparseMatrix may be not SPARSE, so be careful
    int inverse(SparseMatrix &inver);

    SparseVec<T>& operator[] (uint32_t i)
    {
        if (i >= 0 && i < _rows)
        {
            return _vecs[i];
        }
        else
        {
            printf("operator [%u] for SparseMatrix failed, total size %u\n", i, _rows);
            return _vecs[0];
        }
    }
};

template <class T>
class DenseMatrix
{
public:
    DenseVec<T> * _vecs;
    uint32_t _columns;
    uint32_t _rows;

    DenseMatrix() : _vecs(NULL), _columns(0), _rows(0) { }

    ~DenseMatrix()
    {
        if (_rows > 0)
        {
            DEL_ARRAY_POINTER(_vecs);
        }
    }

    int init(T mean, T var)
    {
        srand(1988);
        for (uint32_t i = 0; i < _columns ; ++i )
        {
            for (uint32_t j = 0; j < _rows; ++j)
            {
                _vecs[j][i] = ran_gaussian(mean, var);
            }
        }
        return 0;
    }

    int set_size(uint32_t rows, uint32_t columns)
    {
        DEL_ARRAY_POINTER(_vecs);
        if (columns * rows != 0)
        {
            _rows = rows;
            _vecs = new (std::nothrow) DenseVec<T>[_rows];
            if (NULL == _vecs)
            {
                LOG_WARNING("new DenseMatrix failed!");
                return -1;
            }

            for (uint32_t i = 0; i < _rows; ++i)
            {
                if (_vecs[i].set_size(columns) != 0)
                {
                    return -1;
                }
            }
            _columns = columns;
        }
        return 0;
    }

    DenseVec<T>& operator[] (uint32_t i)
    {
        if (i >=0 && i < _rows)
        {
            return _vecs[i]; 
        }
        else
        {
            printf("operator [%u] for DenseVec<double> failed, total size [%u]\n", i, _rows);
            return _vecs[0];
        }
    }

};

template<class T>
class Data
{
public:
    virtual Vec<T>& get_ins(uint32_t) = 0;
    virtual uint32_t instance_num() = 0;
    virtual int get_label(uint32_t) = 0;
    virtual ~Data() {};
    virtual uint32_t feature_num() = 0;
};

template<class T>
class SparseData : public Data<T>
{
    virtual Vec<T>& get_ins(uint32_t) = 0;
    virtual uint32_t instance_num() = 0;
    virtual int get_label(uint32_t) = 0;
    virtual uint32_t feature_num() = 0;
    virtual ~SparseData() {};
};

template<>
class SparseData<double> : public Data<double> 
{
public:
    SparseMatrix<double> _matrix;
    DenseVec<int> _labels;

    void load_from_txt(std::string filename);
    void output();

    virtual ~SparseData() {}

    SparseVec<double>& operator[] (uint32_t i)
    {
        return _matrix[i];
    }

    virtual Vec<double>& get_ins(uint32_t i)
    {
        return _matrix[i];
    }

    int get_label(uint32_t i)
    {
        return _labels[i];
    }

    uint32_t instance_num()
    {
        return _matrix.rows();
    }  

    uint32_t feature_num()
    {
        return _matrix.columns();
    }

};

template<>
class SparseData<int> : public Data<int>
{
};

template<class T>
class DenseData : public Data<T>
{
};

template<>
class DenseData<double> : public Data<double>
{
};

template<>
class DenseData<int> : public Data<int>
{
};

}

#endif
