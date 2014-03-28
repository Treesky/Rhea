#ifndef _UTIL_
#define _UTIL_

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

#endif /* _UTIL */
