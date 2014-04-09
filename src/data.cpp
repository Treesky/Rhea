/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:30
 * @file data.cpp
 * @description 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "data.h"

namespace Rhea
{

template<class T>
int SparseVec<T>::set_size(uint32_t length)
{
    DEL_ARRAY_POINTER(_atoms);
    _length = length;
    if (0 != length)
    {
        _atoms = new (std::nothrow) SparseAtom<T>[length];
        if (NULL == _atoms)
        {
            LOG_WARNING("new SparseVec failed!");
            return -1;
        }
    }

    return 0;
}

void SparseData<double>::load_from_txt(std::string filename)
{
    uint32_t atom_num = 0; 

    std::ifstream fin;
    fin.open(filename.c_str());

    std::cout << "Start loading txt file:[" << filename << "] ......" << std::endl;

    std::string line;
    std::string token;
    uint32_t max_row_tokens = 0, row_tokens = 0, columns = 0, rows = 0;

    while ( getline(fin, line, '\n') )
    {
        std::stringstream stringsplitter(line.c_str());
        row_tokens = 0;
        while ( std::getline(stringsplitter, token, ' '))
        {
            ++ row_tokens;
            ++ atom_num;
        }
        max_row_tokens = max_row_tokens > row_tokens ? max_row_tokens : row_tokens;
        rows ++;
    }
    _labels.set_size(rows);

    std::cout << "Elements number:[" << atom_num << "]" << std::flush << std::endl;
    fin.close();

    SparseAtom<double> * atoms = 
        (SparseAtom<double> *) malloc( atom_num * sizeof( SparseAtom<double> )) ;

    _matrix.set_size(rows);

    rows = 0;
    fin.open(filename.c_str());
    std::string atom_value;
    std::string atom_idx;

    while ( getline(fin, line, '\n') )
    {
        row_tokens = 0;
        SparseVec<double> curr_vec; 
        curr_vec.set_atoms(atoms);
        std::stringstream stringsplitter(line);
        std::getline(stringsplitter, token, ' ');
        _labels[rows] = atoi(token.c_str());

        while (std::getline(stringsplitter, token, ' '))
        {
            std::stringstream atomsplitter(token);

            getline(atomsplitter, atom_idx, ':');
            getline(atomsplitter, atom_value, ':');

            atoms->value = atof(atom_value.c_str());
            atoms->idx = atoi(atom_idx.c_str());
            columns = columns > atoms->idx ? columns : atoms->idx;

            ++atoms;
            ++row_tokens;
        }
        curr_vec.set_length(row_tokens);
        _matrix[rows] = curr_vec;
        rows ++;
    }
    fin.close();

    _matrix.set_columns(columns + 1);
    std::cout << "load txt file [" << filename <<"] SUCCESS!" << std::flush << std::endl << std::endl;
}

void SparseData<double>::output()
{
    std::cout << "============== Sparse Matrix:" << _matrix.rows() 
        << "*" << _matrix.columns() << "=======================" << std::endl;
    for ( uint32_t i = 0; i < _matrix.rows() ; ++i )
    {
        std::cout << _labels[i];
        for (uint32_t j = 0; j < _matrix[i].length(); ++ j)
        {
            std::cout << " " << _matrix[i][j].idx << ":" << _matrix[i][j].value;
        }
        std::cout << std::endl;
    }
    std::cout << "===============================================" << std::endl;
}

template<class T>
int SparseMatrix<T>::set_size(uint32_t rows)
{
    DEL_ARRAY_POINTER(_vecs);
    _rows = rows;
    if (0 != _rows)
    {
        _vecs = new (std::nothrow) SparseVec<T>[rows];
        if (NULL == _vecs)
        {
            LOG_WARNING("new SparseVec failed\n");
            return -1;
        }
    }
    return 0;
}

}
