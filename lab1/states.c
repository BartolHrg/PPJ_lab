#include "states.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <inttypes.h>


VB_C_CPP_FILE (
    // IDN
    // BROJ
    // OP_PRIDRUZI
    // OP_PLUS
    // OP_MINUS
    // OP_PUTA
    // OP_DIJELI
    // L_ZAGRADA
    // D_ZAGRADA
    // KR_ZA
    // KR_OD
    // KR_DO
    // KR_AZ
        
    GLOBAL_PRIVATE FUNCTION initialHandleChar    OF (DECL c AS char END) AS states_HandleCharReturn END_FUNC_DECL
    GLOBAL_PRIVATE FUNCTION alphaHandleChar      OF (DECL c AS char END) AS states_HandleCharReturn END_FUNC_DECL
    GLOBAL_PRIVATE FUNCTION numericHandleChar    OF (DECL c AS char END) AS states_HandleCharReturn END_FUNC_DECL
    GLOBAL_PRIVATE FUNCTION mayCommentHandleChar OF (DECL c AS char END) AS states_HandleCharReturn END_FUNC_DECL
    GLOBAL_PRIVATE FUNCTION commentHandleChar    OF (DECL c AS char END) AS states_HandleCharReturn END_FUNC_DECL

    DECL line_no AS uint64_t END = 1;
    GLOBAL_PRIVATE FUNCTION output OF (DECL uniq_id AS char* END, DECL cause AS char* END) AS void
    START_FUNCTION
        printf("%s %" PRIu64 " %s\n", uniq_id, line_no, cause);
    END_FUNCTION

    GLOBAL_PRIVATE FUNCTION initialHandleChar OF (DECL c AS char END) AS states_HandleCharReturn
    START_FUNCTION
        IF      isalpha(c) THEN
            *states.handleChar = alphaHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        ELSE_IF isdigit(c) THEN
            *states.handleChar = numericHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        ELSE_IF c == '=' THEN
            // =
            output("OP_PRIDRUZI", "=");
        ELSE_IF c == '+' THEN
            // +
            output("OP_PLUS", "+");
        ELSE_IF c == '-' THEN
            // -
            output("OP_MINUS", "-");
        ELSE_IF c == '*' THEN
            // *
            output("OP_PUTA", "*");
        ELSE_IF c == '/' THEN
            // / or //
            *states.handleChar = mayCommentHandleChar;
            RETURN states_HandleCharReturn_OK;
        ELSE_IF c == '(' THEN
            // (
            output("L_ZAGRADA", "(");
        ELSE_IF c == ')' THEN
            // )
            output("D_ZAGRADA", ")");
        ELSE_IF c == '\n' THEN
            // \n
            ++line_no;
        END_IF
        RETURN states_HandleCharReturn_OK;
    END_FUNCTION

    DECL buffer AS mystring_MyString END = (mystring_MyString) {
        .capacity = 0,
        .length = 0,
        .str = NULL,
    };

    GLOBAL_PRIVATE FUNCTION alphaOutputKeyOrIdn OF () AS void 
    START_FUNCTION
        // keywords: za. az, od, do
        IF      buffer.str[0] == 'z' && buffer.str[1] == 'a' && buffer.str[2] == '\0' THEN output("KR_ZA", buffer.str);
        ELSE_IF buffer.str[0] == 'a' && buffer.str[1] == 'z' && buffer.str[2] == '\0' THEN output("KR_AZ", buffer.str);
        ELSE_IF buffer.str[0] == 'o' && buffer.str[1] == 'd' && buffer.str[2] == '\0' THEN output("KR_OD", buffer.str);
        ELSE_IF buffer.str[0] == 'd' && buffer.str[1] == 'o' && buffer.str[2] == '\0' THEN output("KR_DO", buffer.str);
        ELSE                                                                               output("IDN"  , buffer.str);
        END_IF
    END_FUNCTION
    GLOBAL_PRIVATE FUNCTION alphaHandleChar OF (DECL c AS char END) AS states_HandleCharReturn
    START_FUNCTION
        IF isalnum(c) THEN
            mystring.MyString_add(&buffer, c);
            RETURN states_HandleCharReturn_OK;
        ELSE
            alphaOutputKeyOrIdn();
            mystring.MyString_clear(&buffer);
            *states.handleChar = initialHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        END_IF
    END_FUNCTION

    GLOBAL_PRIVATE FUNCTION numericHandleChar OF (DECL c AS char END) AS states_HandleCharReturn
    START_FUNCTION
        // printf("%zu ", buffer.length);
        IF isdigit(c) THEN
            mystring.MyString_add(&buffer, c);
            RETURN states_HandleCharReturn_OK;
        ELSE
            output("BROJ", buffer.str);
            mystring.MyString_clear(&buffer);
            *states.handleChar = initialHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        END_IF
    END_FUNCTION

    GLOBAL_PRIVATE FUNCTION mayCommentHandleChar OF (DECL c AS char END) AS states_HandleCharReturn
    START_FUNCTION
        IF c == '/' THEN
            //
            *states.handleChar = commentHandleChar;
            RETURN states_HandleCharReturn_OK;
        ELSE
            // /
            output("OP_DIJELI", "/");
            *states.handleChar = initialHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        END_IF
    END_FUNCTION

    GLOBAL_PRIVATE FUNCTION commentHandleChar OF (DECL c AS char END) AS states_HandleCharReturn
    START_FUNCTION
        IF c == '\n' THEN
            *states.handleChar = initialHandleChar;
            RETURN states_HandleCharReturn_REPEAT;
        END_IF
        RETURN states_HandleCharReturn_OK;
    END_FUNCTION

    GLOBAL_PRIVATE FUNCTION states_StaticInit OF () AS void
    START_FUNCTION
        buffer.str = malloc(sizeof(*buffer.str));
    END_FUNCTION

    GLOBAL_PRIVATE DECL handleChar AS states_HandleCharFunc END = initialHandleChar;
    DECL states AS States const END = (States) {
        .handleChar = &handleChar,
        .staticInit = states_StaticInit,
    };

)

