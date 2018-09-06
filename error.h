#ifndef ERROR_H
#define ERROR_H

typedef enum {
    ER_OK = 0,
    ER_Syntax,
    ER_Unbound,
    ER_Args,
    ER_Type
} Error;

#endif
