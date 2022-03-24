//
// Created by Bartol on 10.11.2021..
//

#include "lexres.VB.h"

#define REACHED_LINE_END(c) (((c) == EOF) || ((c) == '\n') || ((c) == '\r'))
#define STRING_SPACE(length) (((length) + 1) * sizeof(char))

VB_C_CPP_FILE(

	GLOBAL_PRIVATE DECL lexers_inputLine_EOF AS bool END = false;
	GLOBAL_PRIVATE FUNCTION lexers_inputLine OF () AS char*
	START
		DECL length AS uint32_t END = 0;
		DECL line AS char* END = malloc(STRING_SPACE(length));

		FOR DECL c AS char END; LOOP c = getchar(); WHILE REACHED_LINE_END(c) && c != EOF END
			PREP FORK IF c == EOF THEN lexers_inputLine_EOF = true; END
			COND !REACHED_LINE_END(c)
			STEP c = getchar();
		START
			line[length] = c;
			++length;
			line = realloc(line, STRING_SPACE(length));
		END

		line[length] = '\0';
		RETURN line;
	END

	DECL lexres_list_length AS uint32_t       END = 0;
	DECL lexres_list        AS lexres_Entity* END;

	GLOBAL_PRIVATE FUNCTION lexres_startsWith OF (DECL str AS char* END, DECL preffix AS char* END) AS bool
	START
		DECL a AS uint64_t END = strlen(str);
		DECL b AS uint64_t END = strlen(preffix);
		FORK IF a < b THEN
			RETURN false;
		END
		FOR DECL i AS uint64_t END = 0;
			PREP
			COND i < b
			STEP ++i;
		START
			FORK IF str[i] != preffix[i] THEN
				RETURN false;
			END
		END
		RETURN true;
	END

	GLOBAL_PRIVATE FUNCTION lexres_getID OF (DECL line AS char* END) AS lexres_ID
	START
		FORK IF lexres_startsWith(line, "IDN")         THEN RETURN lexres_ID_IDN;
		ELSE_IF lexres_startsWith(line, "BROJ")        THEN RETURN lexres_ID_BROJ;
		ELSE_IF lexres_startsWith(line, "OP_PRIDRUZI") THEN RETURN lexres_ID_OP_PRIDRUZI;
		ELSE_IF lexres_startsWith(line, "OP_PLUS")     THEN RETURN lexres_ID_OP_PLUS;
		ELSE_IF lexres_startsWith(line, "OP_MINUS")    THEN RETURN lexres_ID_OP_MINUS;
		ELSE_IF lexres_startsWith(line, "OP_PUTA")     THEN RETURN lexres_ID_OP_PUTA;
		ELSE_IF lexres_startsWith(line, "OP_DIJELI")   THEN RETURN lexres_ID_OP_DIJELI;
		ELSE_IF lexres_startsWith(line, "L_ZAGRADA")   THEN RETURN lexres_ID_L_ZAGRADA;
		ELSE_IF lexres_startsWith(line, "D_ZAGRADA")   THEN RETURN lexres_ID_D_ZAGRADA;
		ELSE_IF lexres_startsWith(line, "KR_ZA")       THEN RETURN lexres_ID_KR_ZA;
		ELSE_IF lexres_startsWith(line, "KR_OD")       THEN RETURN lexres_ID_KR_OD;
		ELSE_IF lexres_startsWith(line, "KR_DO")       THEN RETURN lexres_ID_KR_DO;
		ELSE_IF lexres_startsWith(line, "KR_AZ")       THEN RETURN lexres_ID_KR_AZ;
		END
		RETURN -1;
	END

	FUNCTION lexres_init OF () AS void
	START
		lexres_list = malloc(0);
		LOOP
			DECL line AS char* END = lexers_inputLine();
		WHILE !lexers_inputLine_EOF DO
			DECL id AS lexres_ID END = lexres_getID(line);
			lexres_list = realloc(lexres_list, (++lexres_list_length) * sizeof(lexres_Entity));
			lexres_list[lexres_list_length - 1] = (lexres_Entity) {
				.source = line,
				.id = id,
			};

		END
	END

	FUNCTION lexres_end OF () AS void
	START
		FOR DECL i AS uint32_t END = 0; DECL entity AS lexres_Entity END;
			PREP entity = lexres_list[i];
			COND i < lexres_list_length
			STEP ++i;
		START
			free(entity.source);
		END
		free(lexres_list);
	END
)

/**/

