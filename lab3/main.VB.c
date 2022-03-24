//
// Created by Bartol on 30.12.2021..
//

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include <string.h>

VB_C_CPP_FILE(

	STRUCT Variable
	START
		DECL name AS char* END;
		DECL def_line AS size_t END;
	END
	DECL to_define AS bool END = false;
	DECL az_define AS bool END = false;
	DECL eq_define AS bool END = false;
	DECL curr_lvalue AS char* END;
	STRUCT Scope
	START
		DECL indent AS size_t END;
		DECL var_count AS size_t END;
		DECL variables AS Variable* END;
	END
	STRUCT ScopeListNode
	START
		DECL scope AS Scope END;
		DECL prev AS ScopeListNode* END;
	END
	STRUCT ScopeList
	START
		// DECL head AS ScopeListNode* END;
		DECL tail AS ScopeListNode* END;
	END
	FUNCTION ScopeList_add OF (DECL this AS ScopeList* END, DECL scope AS Scope END) AS void
	START
		DECL node AS ScopeListNode* END = malloc(sizeof(ScopeListNode));
		node->scope = scope;
		node->prev  = this->tail;
		this->tail  = node;
	END
	FUNCTION ScopeList_remove OF (DECL this AS ScopeList* END) AS void
	START
		DECL node AS ScopeListNode* END = this->tail;
		this->tail = node->prev;
		DECL scope AS Scope END = node->scope;
		FOR DECL i AS size_t END = 0;
			COND i < scope.var_count
			STEP ++i;
		START
			free(scope.variables[i].name);
		END
		free(scope.variables);
		free(node);
	END
	FUNCTION Scope_addVariable OF (DECL this AS Scope* END, DECL variable AS Variable END) AS void
	START
		++this->var_count;
		this->variables = realloc(this->variables, this->var_count * sizeof(Variable));
		this->variables[this->var_count - 1] = variable;
	END

	FUNCTION variableInScope OF (DECL name AS char* END, DECL scope AS Scope END) AS size_t
	START
		FOR DECL i AS size_t END = 0;
			COND i < scope.var_count
			STEP ++i;
		START
			FORK IF strcmp(name, scope.variables[i].name) == 0 THEN
				RETURN scope.variables[i].def_line;
			END
		END
		RETURN CAST(-1 AS size_t);
	END
	FUNCTION variableExists OF (DECL name AS char* END, DECL scopes AS ScopeList END) AS size_t
	START
		FOR DECL node AS ScopeListNode* END = scopes.tail;
			COND node != NULL
			STEP node = node->prev;
		START
			DECL res AS size_t END = variableInScope(name, node->scope);
			FORK IF res != CAST(-1 AS size_t) THEN
				RETURN res;
			END
		END
		RETURN CAST(-1 AS size_t);
	END

	ENUM ReadingState
	START
		INIT,
		NODE,
		TYPE_,
		LINE_NO,
		SOURCE,
	END
	FUNCTION main OF () AS int
	START
		// 3 types of line:
		//     \s*<\w+>
		//     \s+\w+ \d .+
		//     \s+$
		#define MAX_LEN_NODE (strlen("naredba_pridruzivanja") + 1)
		#define MAX_LEN_TYPE (strlen("OP_PRIDRUZI") + 1)


		DECL global_scope AS Scope END = (Scope) { // global
			.indent = 0,
			.var_count = 0,
			.variables = malloc(0),
		};
		DECL scopes AS ScopeList END = (ScopeList) {
			.tail = malloc(sizeof(ScopeListNode)),
		};
		*scopes.tail = (ScopeListNode) {
			.scope = global_scope,
			.prev = NULL,
		};
		FOR DECL c AS char END;
			DECL indent AS size_t END = 0;
			DECL node   AS char* END; DECL node_length   AS size_t END;
			DECL type   AS char* END; DECL type_length   AS size_t END;
			DECL source AS char* END; DECL source_length AS size_t END;
			DECL line_no AS size_t END = 1;
			DECL state AS ReadingState END = INIT;
			DECL i AS int END = 0;
			PREP c = getchar(); // fprintf(stderr, "\t<%u>", c); ++i;
			COND c != EOF && i <= 540
		START
			FORK state
			CASE INIT THEN
				FORK c
				CASE ' ' THEN
					++indent;
				CASE '\n' THEN
					indent = 0;
				CASE '<' THEN
					state = NODE;
					node = malloc(MAX_LEN_NODE * sizeof(char));
					node_length = 0;
				CASE '$' THEN
					PASS;
				DEFAULT
					state = TYPE_;
					type = malloc(MAX_LEN_TYPE * sizeof(char));
					type_length = 0;
					ungetc(c, stdin);
				END
			CASE NODE THEN
				FORK IF c == '>' THEN
					state = INIT;
					node[node_length] = '\0';
//					fprintf(stderr, "\t%d %d <%s>\n", az_define, eq_define, node);
					FORK IF strcmp("za_petlja", node) == 0 THEN
						to_define = true;
						az_define = true;
						DECL scope AS Scope END = (Scope) {
							.indent = indent,
							.var_count = 0,
							.variables = malloc(0),
						};
						ScopeList_add(&scopes, scope);
						to_define = true;
					ELSE IF strcmp("naredba_pridruzivanja", node) == 0 THEN
						to_define = true;
					ELSE IF strcmp("lista_naredbi", node) == 0 THEN
						az_define = false;
						eq_define = false;
					END
					// TODO
				ELSE
					node[node_length] = c;
					++node_length;
				END
			CASE TYPE_ THEN
				FORK IF c == ' ' THEN
					state = LINE_NO;
					line_no = 0;
					type[type_length] = '\0';
					// TODO
				ELSE
					type[type_length] = c;
					++type_length;
				END
			CASE LINE_NO THEN
				FORK IF c == ' ' THEN
					state = SOURCE;
					source = malloc(1);
					source_length = 0;
				ELSE
					line_no *= 10;
					line_no += c - '0';
				END
			CASE SOURCE THEN
				FORK IF c == '\n' THEN
					source[source_length] = '\0';
					FORK IF strcmp("KR_AZ", type) == 0 THEN
						ScopeList_remove(&scopes);
					ELSE IF strcmp("IDN", type) == 0 THEN
						FORK IF to_define THEN
							// we are defining/reassigning variable
							to_define = false;
							curr_lvalue = source;
							FORK IF az_define || variableExists(source, scopes) == CAST(-1 AS size_t) THEN
								Scope_addVariable(&scopes.tail->scope, (Variable) {
									.name = source,
									.def_line = line_no,
								});
//								fprintf(stderr, "def %zu %s\n", line_no, source);
								eq_define = true;
							END
						ELSE
							// using
//							fprintf(stderr, "\t\t%d <%s> <%s>\n", az_define, source, curr_lvalue);
							FORK IF (az_define || eq_define) && strcmp(source, curr_lvalue) == 0 THEN
								printf("err %zu %s\n", line_no, source);
								exit(0);
							END
							DECL def_line AS size_t END = variableExists(source, scopes);
							FORK IF def_line != CAST(-1 AS size_t) THEN
								printf("%zu %zu %s\n", line_no, def_line, source);
							ELSE
								printf("err %zu %s\n", line_no, source);
								exit(0);
							END
						END
					END
					// TODO
					ungetc(c, stdin);
					state = INIT;
				ELSE
					++source_length;
					source = realloc(source, (source_length + 1) * sizeof(char));
					source[source_length - 1] = c;
				END
			END
		END
	END
)