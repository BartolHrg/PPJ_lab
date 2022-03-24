//
// Created by Bartol on 10.11.2021..
//

#ifndef LAB_2_MYSTACK_H
#define LAB_2_MYSTACK_H

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

VB_C_CPP_FILE(
	
	STRUCT MyStack_Node
	START
		DECL data AS void* END;
		DECL prev AS MyStack_Node* END;
	END
	
	STRUCT MyStack
	START
		DECL current AS MyStack_Node* END;
	END
	
	FUNCTION MyStack_init    OF (DECL this AS MyStack* END, DECL first AS void* END) AS void END
	
	FUNCTION MyStack_push    OF (DECL this AS MyStack* END, DECL item  AS void* END) AS void END
	FUNCTION MyStack_pop     OF (DECL this AS MyStack* END) AS void* END
	FUNCTION MyStack_peek    OF (DECL this AS MyStack* END) AS void* END
	FUNCTION MyStack_isEmpty OF (DECL this AS MyStack* END) AS bool  END
	FUNCTION MyStack_clear   OF (DECL this AS MyStack* END) AS void  END
)

#endif //LAB_2_MYSTACK_H
