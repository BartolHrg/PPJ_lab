#ifndef MYSTRING_H
#define MYSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "VB.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


VB_C_CPP_FILE(

    STRUCT mystring_MyString
    START_STRUCT
        DECL capacity AS size_t END;
        DECL length AS size_t END;
        DECL str AS char* END;
    END_STRUCT
    
    STRUCT MyString
    START_STRUCT
        DECL FP MyString_add   OF (mystring_MyString*, char) AS void END;
        DECL FP MyString_clear OF (mystring_MyString*)       AS void END;
    END_STRUCT
    
    DEFINE_LATER DECL mystring AS MyString const END;

)

#ifdef __cplusplus
}
#endif

#endif // MYSTRING_H