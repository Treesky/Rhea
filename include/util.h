#ifndef _UTIL_
#define _UTIL_

#include <iostream>
#include <cmath>

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

uint64_t sign_str(std::string line);

uint64_t sign_64_48(uint64_t sign);

uint32_t get_fea_idx(uint64_t sign);

double ran_uniform();

double ran_gaussian();

double ran_gaussian(double mean, double stdev); 

#endif /* _UTIL */
