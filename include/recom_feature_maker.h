/**
 * @author: treesky - treesky.cn@gmail.com
 * @last modified 2014-04-09 17:27
 * @file recom_feature_maker.h
 * @description Extract LIBSVM style features from <user, item, score, time, info1, ... >  
 * features include:
 *  user_id
 *  item_id
 *  item IDF feature
 *  weekday 
 *  hour
 *  day in month
 *  info1
 *  ...
 */
#ifndef _RECOM_FEATURE_MAKER_
#define _RECOM_FEATURE_MAKER_

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ext/hash_map>
#include <time.h>

#include "data.h"
#include "util.h"

namespace Rhea
{

struct RecomInfo
{
    std::string user_id;
    std::string item_id;
    std::string action_score;
    std::string time;
    std::vector<std::string> infos;
};

class Feature
{
protected:
    char sign_buff[SIGN_BUFF_LEN];
    uint32_t buff_len;
    uint32_t prefix;
public:
    Feature()
    {
        sign_buff[0] = '\0';
        buff_len = 0;
    }
    int virtual make_feature(RecomInfo &recom_info, 
                             std::vector< SparseAtom<double> > &features) = 0;

    virtual ~Feature() {};
};

class UserIDFeature : public Feature
{
public:
    UserIDFeature()
    {
        prefix = 1 << 24;
    }

    int make_feature(RecomInfo &recom_info, 
                     std::vector< SparseAtom<double> > &features)
    {
        SparseAtom<double> feature;
        uint32_t sign = sign_str(recom_info.user_id);
        sign = prefix | sign_32_24(sign);
        uint32_t fea_idx = Sign2FeatureID::get_instance()->get_fea_idx(sign);
        feature.idx = fea_idx;
        feature.value = 1;
        features.push_back(feature);
        return 0;
    }
};

class ItemIDFeature : public Feature
{
public:

    ItemIDFeature()
    {
        prefix = 2 << 24;
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        SparseAtom<double> feature;
        uint32_t sign = sign_str(recom_info.user_id);
        sign = prefix | sign_32_24(sign);
        uint32_t fea_idx = Sign2FeatureID::get_instance()->get_fea_idx(sign);
        feature.idx = fea_idx;
        feature.value = 1;
        features.push_back(feature);
        return 0;
    }
};

class ItemIDFFeature : public Feature
{
private:
    __gnu_cxx::hash_map<uint32_t, std::vector<std::string> * > id_size;
public:
    ItemIDFFeature()
    {
        prefix = 3 << 24;
    }

    ItemIDFFeature(std::string filename)
    {
        std::ifstream fin(filename.c_str());
        if ( !fin.is_open() )
        {
            std::cout << "open ItemIDFFeature file failed!" << std::endl;
            return;
        }

        std::string idf_line, token;
        uint32_t user_id = 0;
        while ( getline(fin, idf_line) )
        {
            std::stringstream stringsplitter(idf_line);

            std::pair<uint32_t, std::vector<std::string> * > pp;
            getline(stringsplitter, token, ' ');
            user_id = sign_str(token);
            pp.first = user_id;
            pp.second = new std::vector<std::string>();
            id_size.insert(pp);
            while ( getline(stringsplitter, token, ' ') )
            {
                pp.second->push_back(token);
            }
        }
        std::cout << "load ItemIDFFeature file success!" << std::endl;

        fin.close();
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint32_t user_id = sign_str(recom_info.user_id);
        if (id_size.find(user_id) != id_size.end())
        {
            std::vector<std::string> * vec = id_size[user_id];
            for (uint32_t i = 0; i < vec->size(); ++i )
            {
                SparseAtom<double> atom;
                std::string item_id = (*vec)[i];
                atom.value = 1.0 / vec->size();
                atom.idx = Sign2FeatureID::get_instance()->get_fea_idx(prefix | sign_32_24( sign_str(item_id)) );
                features.push_back(atom);
            }
        }
        return 0;
    }

    ~ItemIDFFeature()
    {
        __gnu_cxx::hash_map<uint32_t, std::vector<std::string> * >::iterator iter;
        for (iter = id_size.begin(); iter != id_size.end(); ++iter )
        {
            DEL_POINTER(iter->second);
        }
    }
};

class WeekdayFeature : public Feature
{
public:
    WeekdayFeature()
    {
        prefix = 4 << 24;
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = Sign2FeatureID::get_instance()->get_fea_idx(prefix | time.tm_wday);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        features.push_back(atom);
        return 0;
    }
};

class HourFeature : public Feature
{
public:
    HourFeature()
    {
        prefix = 5 << 24;
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = Sign2FeatureID::get_instance()->get_fea_idx(prefix | time.tm_hour);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        features.push_back(atom);
        return 0;
    }
};

class DayFeature : public Feature
{
public:
    DayFeature()
    {
        prefix = 6 << 24;
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = Sign2FeatureID::get_instance()->get_fea_idx(prefix | time.tm_mday);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        features.push_back(atom);
        return 0;
    }
};

}
#endif /* _RECOM_FEATURE_MAKER_ */
