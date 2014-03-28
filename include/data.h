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
class SparseVec
{
private:
    SparseAtom<T> * _atoms;
    uint32_t _length;

public:
    SparseVec() : _atoms(NULL), _length(0) {}

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
class DenseVec
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

    double ran_uniform() {
        return rand()/((double)RAND_MAX + 1);
    }

    double ran_gaussian() {
        // Joseph L. Leva: A fast normal Random number generator
        double u,v, x, y, Q;
        do {
            do {
                u = ran_uniform();
            } while (u == 0.0); 
            v = 1.7156 * (ran_uniform() - 0.5);
            x = u - 0.449871;
            y = std::abs(v) + 0.386595;
            Q = x*x + y*(0.19600*y-0.25472*x);
            if (Q < 0.27597) { break; }
        } while ((Q > 0.27846) || ((v*v) > (-4.0*u*u*std::log(u)))); 
        return v / u;
    }

    double ran_gaussian(double mean, double stdev) {
        if ((stdev == 0.0) || (std::isnan(stdev))) {
            return mean;
        } else {
            return mean + stdev*ran_gaussian();
        }
    }

};

class Data
{
};

class SparseDataBase : public Data
{
};

class DenseDataBase : public Data
{
};

template<class T>
class SparseData : public SparseDataBase
{
};

template<>
class SparseData<double> : public SparseDataBase
{
public:
    SparseMatrix<double> _matrix;
    DenseVec<int> _labels;

    void load_from_txt(std::string filename);
    void output();

    SparseVec<double>& operator[] (uint32_t i)
    {
        return _matrix[i];
    }

    SparseVec<double>& get_ins(uint32_t i)
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
class SparseData<int> : public SparseDataBase
{
};

template<class T>
class DenseData : public DenseDataBase
{
};

template<>
class DenseData<double> : public DenseDataBase
{
};

template<>
class DenseData<int> : public DenseDataBase
{
};

}

#endif
