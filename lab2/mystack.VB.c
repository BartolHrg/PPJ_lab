//
// Created by Bartol on 11.11.2021..
//

#include "mystack.VB.h"

VB_C_CPP_FILE(
	FUNCTION MyStack_init    OF (DECL this AS MyStack* END, DECL first AS void* END) AS void
	START
		this->current = malloc(sizeof(MyStack_Node));
		this->current->prev = NULL;
		this->current->data = NULL;
		FORK IF first != NULL THEN
			DECL node AS MyStack_Node* END = malloc(sizeof(MyStack_Node));
			node->prev = this->current;
			node->data = first;
			this->current = node;
		END
	END

	FUNCTION MyStack_push    OF (DECL this AS MyStack* END, DECL item  AS void* END) AS void
	START
		DECL node AS MyStack_Node* END = malloc(sizeof(MyStack_Node));
		node->data = item;
		node->prev = this->current;
		this->current = node;
	END
	FUNCTION MyStack_pop     OF (DECL this AS MyStack* END) AS void*
	START
		DECL node AS MyStack_Node* END = this->current;
		this->current = node->prev;
		DECL tmp AS void* END = node->data;
		free(node);
		RETURN tmp;
	END
	FUNCTION MyStack_peek    OF (DECL this AS MyStack* END) AS void*
	START
		RETURN this->current->data;
	END
	FUNCTION MyStack_isEmpty OF (DECL this AS MyStack* END) AS bool
	START
		RETURN this->current->prev == NULL;
	END
	FUNCTION MyStack_clear   OF (DECL this AS MyStack* END) AS void
	START
		LOOP WHILE !MyStack_isEmpty(this) DO
			MyStack_pop(this);
		END
	END
)