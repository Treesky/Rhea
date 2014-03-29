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
    uint64_t prefix;
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

    int make_feature(RecomInfo &recom_info, 
                     std::vector< SparseAtom<double> > &features)
    {
        SparseAtom<double> feature;
        uint64_t sign = sign_str(recom_info.user_id);
        sign = prefix | sign_64_48(sign);
        uint32_t fea_idx = get_fea_idx(sign);
        feature.idx = fea_idx;
        feature.value = 1;
        features.push_back(feature);
        return 0;
    }
};

class ItemIDFeature : public Feature
{
public:
    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        SparseAtom<double> feature;
        uint64_t sign = sign_str(recom_info.user_id);
        sign = prefix | sign_64_48(sign);
        uint32_t fea_idx = get_fea_idx(sign);
        feature.idx = fea_idx;
        feature.value = 1;
        features.push_back(feature);
        return 0;
    }
};

class ItemIDFFeature : public Feature
{
private:
    __gnu_cxx::hash_map<uint32_t, std::vector<uint32_t> * > id_size;
public:
    ItemIDFFeature(std::string filename)
    {
        std::ifstream fin(filename.c_str());
        if ( !fin.is_open() )
        {
            std::cout << "open ItemIDFFeature file failed!" << std::endl;
            return;
        }

        std::string idf_line, token;
        uint32_t id = 0;
        while ( getline(fin, idf_line) )
        {
            std::stringstream stringsplitter(idf_line);

            getline(stringsplitter, token, '\t');
            sscanf(token.c_str(), "%u", &id);
            std::pair<uint32_t, std::vector<uint32_t> * > pp;
            pp.second = new std::vector<uint32_t>();
            id_size.insert(pp);
            pp.first = id;
            while ( getline(stringsplitter, token, '\t') )
            {
                sscanf(token.c_str(), "%u", &id);
                pp.second->push_back(id);
            }
        }
        std::cout << "load ItemIDFFeature file success!" << std::endl;

        fin.close();
    }

    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint32_t user_id = 0;
        sscanf(recom_info.user_id.c_str(), "%u", &user_id);
        if (id_size.find(user_id) != id_size.end())
        {
            std::vector<uint32_t> * vec = id_size[user_id];
            for (uint32_t i = 0; i < vec->size(); ++i )
            {
                SparseAtom<double> atom;
                uint64_t item_id = (*vec)[i];
                atom.value = (double)( item_id ) / vec->size();
                atom.idx = get_fea_idx(prefix | item_id);
            }
        }
        return 0;
    }

    ~ItemIDFFeature()
    {
        __gnu_cxx::hash_map<uint32_t, std::vector<uint32_t> * >::iterator iter;
        for (iter = id_size.begin(); iter != id_size.end(); ++iter )
        {
            DEL_POINTER(iter->second);
        }
    }
};

class WeekdayFeature : public Feature
{
    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = get_fea_idx(prefix | time.tm_wday);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        return 0;
    }
};

class HourFeature : public Feature
{
    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = get_fea_idx(prefix | time.tm_hour);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        return 0;
    }
};

class DayFeature : public Feature
{
    int make_feature(RecomInfo &recom_info,
                     std::vector< SparseAtom<double> > &features)
    {
        uint64_t time_stamp = 0L;
        sscanf(recom_info.time.c_str(), "%lu", &time_stamp);
        struct timeval tv = {time_stamp, 0};
        struct tm time;
        localtime_r(&tv.tv_sec, &time);
        uint32_t fea_id = get_fea_idx(prefix | time.tm_mday);
        SparseAtom<double> atom;
        atom.idx = fea_id;
        atom.value = 1;
        return 0;
    }
};

}
#endif /* _RECOM_FEATURE_MAKER_ */
