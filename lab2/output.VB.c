//
// Created by Bartol on 11.11.2021..
//

#include "output.VB.h"

#include <string.h>

#define STRING_SPACE(length) (((length) + 1) * sizeof(char))

VB_C_CPP_FILE(
	GLOBAL_PRIVATE DECL buffer          AS char*    END;
	GLOBAL_PRIVATE DECL buffer_length   AS uint32_t END = 0;
	GLOBAL_PRIVATE DECL buffer_capacity AS uint32_t END = 0;
	
	FUNCTION output_init OF () AS void
	START
		buffer = malloc (sizeof(char));
		buffer[0] = '\0';
	END
	
	FUNCTION output_output OF (DECL indent AS uint32_t END, DECL str AS char* END) AS void
	START
		DECL new_len AS uint32_t END = buffer_length + indent + strlen(str) + 1;
		FORK IF new_len > buffer_capacity THEN
			buffer_capacity = new_len;
			buffer = realloc(buffer, STRING_SPACE(buffer_capacity));
		END
		FOR DECL i AS uint32_t END = 0;
			PREP
			COND i < indent
			STEP ++i;
		START
			buffer[buffer_length + i] = ' ';
		END
		strcpy(buffer + buffer_length + indent, str);
		buffer_length = new_len;
		buffer[buffer_length-1] = '\n';
		buffer[buffer_length] = '\0';
//		for(int i=0;i<indent;++i)putchar(' ');printf(str);putchar('\n');
	END
	
	FUNCTION output_clear OF () AS void
	START
		buffer[0] = '\0';
		buffer_length = 0;
	END
	
	FUNCTION output_error OF (DECL source AS char* END) AS void
	START
		FORK IF source == NULL THEN
			printf("err kraj\n");
		ELSE
			printf("err %s\n", source);
		END
	END
	
	FUNCTION output_end OF () AS void
	START
		printf(buffer);
		free(buffer);
		buffer_length = 0;
		buffer_capacity = 0;
	END
)