#include "mystring.h"


VB_C_CPP_FILE(

    
    GLOBAL_PRIVATE FUNCTION mystring_MyString_add OF (DECL this AS mystring_MyString* END, DECL c AS char END) AS void
    START_FUNCTION
        IF this->length <= this->capacity THEN
            ++this->capacity;
            this->str = realloc(this->str, (this->capacity + 1) * sizeof(*this->str));
        END_IF
        this->str[this->length] = c;
        this->str[++this->length] = '\0';
    END_FUNCTION
    GLOBAL_PRIVATE FUNCTION mystring_MyString_clear OF (DECL this AS mystring_MyString* END) AS void
    START_FUNCTION
        this->length = 0;
    END_FUNCTION
    
    DECL mystring AS MyString const END = (MyString) {
        .MyString_add   = mystring_MyString_add,
        .MyString_clear = mystring_MyString_clear,
    };
)