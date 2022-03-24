#ifndef STATES_H
#define STATES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "VB.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "mystring.h"

VB_C_CPP_FILE(

ENUM states_HandleCharReturn
START_ENUM
    states_HandleCharReturn_OK,
    states_HandleCharReturn_ERROR,
    states_HandleCharReturn_REPEAT,
END_ENUM

TYPEDEF FP states_HandleCharFunc OF (char) AS states_HandleCharReturn END

STRUCT States
START_STRUCT
    DECL handleChar AS states_HandleCharFunc* END;
    DECL FP staticInit OF () AS void END;
END_STRUCT
DEFINE_LATER DECL states AS States const END;

)

#ifdef __cplusplus
}
#endif

#endif // STATES_H