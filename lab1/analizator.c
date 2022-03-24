#include "VB.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "states.h"

VB_C_CPP_FILE(
    int count = 0;
    
    FUNCTION main OF () AS int
    START_FUNCTION
    
        states.staticInit();
        
        FOR DECL c AS char END ='\0';
            PREP IF c == '\0' THEN c = getchar(); END_IF
            COND c != EOF
            STEP
        START_FOR
            // if (count++ > 100) break;
            DECL result AS states_HandleCharReturn END = (*states.handleChar)(c);
            IF      result == states_HandleCharReturn_OK     THEN
                c = '\0';
            ELSE_IF result == states_HandleCharReturn_REPEAT THEN
                PASS;
            ELSE
                fprintf(stderr, "something went wrong");
            END_IF
        END_FOR
    END_FUNCTION
)
