#ifndef _UTIL_
#define _UTIL_

#include <iostream>
#include <cmath>
#include <ext/hash_map>

#define DEL_POINTER(x)                      \
do                                          \
{                                           \
    if (NULL != x)                          \
    {                                       \
        delete x;                           \
        x = NULL;                           \
    }                                       \
} while(false)                              \


#define DEL_ARRAY_POINTER(x)                \
do                                          \
{                                           \
    if (NULL != x)                          \
    {                                       \
        delete [] x;                        \
        x = NULL;                           \
    }                                       \
} while(false)                              \

#define LOG(info, level)    \
    printf("LOG\t"#level"\t"#info"\n")  \

#define LOG_WARNING(info) \
    LOG(info, WARNING)  \

#define	RAND_MAX	2147483647
#define SIGN_BUFF_LEN 1024

uint32_t sign_str(std::string line);

uint32_t sign_32_24(uint32_t sign);

uint32_t get_fea_idx(uint32_t sign);

double ran_uniform();

double ran_gaussian();

double ran_gaussian(double mean, double stdev); 

namespace Rhea
{

class Sign2FeatureID
{
private:
    /*
    __gnu_cxx::hash_map<uint32_t, uint32_t> _sign_dict;
    uint32_t _local_id;
    */
    Sign2FeatureID() { }
    /*
    class CGabor
    {
        public:
            ~CGabor()
            {
                DEL_POINTER(_p_ins);
            }
    };
    static CGabor _cgabor;
    */
    static Sign2FeatureID *_p_ins;

public:
    static Sign2FeatureID * get_instance()
    {
        _p_ins = NULL;
            //new (std::nothrow) Sign2FeatureID();
        /*
        if (NULL == _p_ins)
        {
            _p_ins = new (std::nothrow) Sign2FeatureID();
            if (NULL == _p_ins)
            {
                std::cout << "new Sign2FeatureID instance failed!" << std::endl;
            }
        }
        return _p_ins;
        */
        return _p_ins;
    }

    uint32_t get_fea_idx(uint32_t sign)
    {
        /*
        if (_sign_dict.find(sign) == _sign_dict.end())
        {
            std::pair<uint32_t, uint32_t> p(sign, _local_id);
            _sign_dict.insert(p);
            ++_local_id;
            return _local_id - 1;
        }
        else 
        {
            return _sign_dict[sign];
        }
        */
        return 0;
    }
};

}

#endif /* _UTIL */
