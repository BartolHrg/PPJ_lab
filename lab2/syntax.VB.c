//
// Created by Bartol on 10.11.2021..
//

#include "syntax.VB.h"

#include "mystack.VB.h"
#include "output.VB.h"

VB_C_CPP_FILE(
	ENUM syntax_ErrorStatus
	START
		syntax_ErrorStatus_OK = 0,
		syntax_ErrorStatus_ERROR,
	END
	
	STRUCT syntax_Step
	START
		DECL proc AS_FUNC OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END;
		DECL indent AS uint32_t END;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_new OF (
		DECL indent AS uint32_t END,
		DECL proc AS_FUNC OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	) AS syntax_Step*
	START
		DECL this AS syntax_Step* END = malloc(sizeof(syntax_Step));
		this->indent = indent;
		this->proc = proc;
		RETURN this;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_program              OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_listaNaredbi         OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_naredba              OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_naredbaPridruzivanja OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_zaPetlja             OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_e                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_eLista               OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_t                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_tLista               OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_p                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_DO                OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_AZ                OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_D_ZAGRADA         OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus END
	
	FUNCTION syntax_init OF () AS void
	START
		lexres_init();
		output_init();
	END
	
	GLOBAL_PRIVATE DECL lexres_list_index AS uint32_t END = 0;
	GLOBAL_PRIVATE DECL syntax_error_cause AS char* END;
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_program OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<program>");
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_listaNaredbi));
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_listaNaredbi OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<lista_naredbi>");
		FORK IF lexres_list_index < lexres_list_length && lexres_list[lexres_list_index].id != lexres_ID_KR_AZ THEN
			MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_listaNaredbi));
			MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_naredba));
		ELSE
			output_output(indent + 1, "$");
		END
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_naredba OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<naredba>");
		FORK IF lexres_list_index < lexres_list_length && lexres_list[lexres_list_index].id == lexres_ID_KR_ZA THEN
			MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_zaPetlja));
		ELSE
			MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_naredbaPridruzivanja));
		END
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_naredbaPridruzivanja OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<naredba_pridruzivanja>");
		DECL entity AS lexres_Entity END;
		FORK IF lexres_list_index < lexres_list_length THEN
			entity = lexres_list[lexres_list_index++];
			FORK IF entity.id != lexres_ID_IDN THEN
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			output_output(indent + 1, entity.source);
		ELSE
			syntax_error_cause = NULL;
			RETURN syntax_ErrorStatus_ERROR;
		END
		FORK IF lexres_list_index < lexres_list_length THEN
			entity = lexres_list[lexres_list_index++];
			FORK IF entity.id != lexres_ID_OP_PRIDRUZI THEN
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			output_output(indent + 1, entity.source);
		ELSE
			syntax_error_cause = NULL;
			RETURN syntax_ErrorStatus_ERROR;
		END
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_e));
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_zaPetlja             OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<za_petlja>");
		DECL entity AS lexres_Entity END;
		FORK IF lexres_list_index < lexres_list_length THEN
			entity = lexres_list[lexres_list_index++];
			FORK IF entity.id != lexres_ID_KR_ZA THEN
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			output_output(indent + 1, entity.source);
		ELSE
			syntax_error_cause = NULL;
			RETURN syntax_ErrorStatus_ERROR;
		END
		FORK IF lexres_list_index < lexres_list_length THEN
			entity = lexres_list[lexres_list_index++];
			FORK IF entity.id != lexres_ID_IDN THEN
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			output_output(indent + 1, entity.source);
		ELSE
			syntax_error_cause = NULL;
			RETURN syntax_ErrorStatus_ERROR;
		END
		FORK IF lexres_list_index < lexres_list_length THEN
			entity = lexres_list[lexres_list_index++];
			FORK IF entity.id != lexres_ID_KR_OD THEN
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			output_output(indent + 1, entity.source);
		ELSE
			syntax_error_cause = NULL;
			RETURN syntax_ErrorStatus_ERROR;
		END

		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_KR_AZ));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_listaNaredbi));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_e));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_KR_DO));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_e));
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_e                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<E>");
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_eLista));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_t));
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_eLista               OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<E_lista>");
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			FORK IF entity.id == lexres_ID_OP_PLUS || entity.id == lexres_ID_OP_MINUS THEN
				output_output(indent + 1, entity.source);
				MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_e));
				RETURN syntax_ErrorStatus_OK;
			ELSE
				--lexres_list_index;
			END
		END
		output_output(indent + 1, "$");
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_t                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<T>");
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_tLista));
		MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_p));
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_tLista               OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<T_lista>");
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			FORK IF entity.id == lexres_ID_OP_PUTA || entity.id == lexres_ID_OP_DIJELI THEN
				output_output(indent + 1, entity.source);
				MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_t));
				RETURN syntax_ErrorStatus_OK;
			ELSE
				--lexres_list_index;
			END
		END
		output_output(indent + 1, "$");
		RETURN syntax_ErrorStatus_OK;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_p                    OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		output_output(indent, "<P>");
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			output_output(indent + 1, entity.source);
			FORK IF entity.id == lexres_ID_OP_PLUS || entity.id == lexres_ID_OP_MINUS THEN
				MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_p));
			ELSE_IF entity.id == lexres_ID_IDN || entity.id == lexres_ID_BROJ THEN
				PASS;
			ELSE_IF entity.id == lexres_ID_L_ZAGRADA THEN
				MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_KR_D_ZAGRADA));
				MyStack_push(stack, syntax_Step_new(indent + 1, syntax_Step_e));
			ELSE
				--lexres_list_index;
				syntax_error_cause = entity.source;
				RETURN syntax_ErrorStatus_ERROR;
			END
			RETURN syntax_ErrorStatus_OK;
		END
		syntax_error_cause = NULL;
		RETURN syntax_ErrorStatus_ERROR;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_DO                OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			FORK IF entity.id == lexres_ID_KR_DO THEN
				output_output(indent, entity.source);
				RETURN syntax_ErrorStatus_OK;
			END
			syntax_error_cause = entity.source;
		ELSE
			syntax_error_cause = NULL;
		END
		RETURN syntax_ErrorStatus_ERROR;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_AZ                OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			FORK IF entity.id == lexres_ID_KR_AZ THEN
				output_output(indent, entity.source);
				RETURN syntax_ErrorStatus_OK;
			END
			syntax_error_cause = entity.source;
		ELSE
			syntax_error_cause = NULL;
		END
		RETURN syntax_ErrorStatus_ERROR;
	END
	
	GLOBAL_PRIVATE FUNCTION syntax_Step_KR_D_ZAGRADA         OF (DECL stack AS MyStack* END, DECL indent AS uint32_t END) AS syntax_ErrorStatus
	START
		FORK IF lexres_list_index < lexres_list_length THEN
			DECL entity AS lexres_Entity END = lexres_list[lexres_list_index++];
			FORK IF entity.id == lexres_ID_D_ZAGRADA THEN
				output_output(indent, entity.source);
				RETURN syntax_ErrorStatus_OK;
			END
			syntax_error_cause = entity.source;
		ELSE
			syntax_error_cause = NULL;
		END
		RETURN syntax_ErrorStatus_ERROR;
	END
	
	FUNCTION syntax_analyze OF () AS void
	START
		DECL stack AS MyStack END;
		MyStack_init (&stack, syntax_Step_new(0, syntax_Step_program));
		
		int i = 0;
		LOOP WHILE !MyStack_isEmpty(&stack) DO
			DECL step AS syntax_Step* END = MyStack_pop(&stack);
			DECL error AS syntax_ErrorStatus END = step->proc(&stack, step->indent);
			free(step);
			FORK IF error == syntax_ErrorStatus_ERROR THEN
				output_clear();
				output_error(syntax_error_cause);
				BREAK;
			END
		END
		
		FORK IF lexres_list_index < lexres_list_length && MyStack_isEmpty(&stack) THEN
			output_clear();
			output_error(NULL);
		END
		
		MyStack_clear(&stack);
	END
	
	FUNCTION syntax_end OF () AS void
	START
		lexres_end();
		output_end();
	END
)