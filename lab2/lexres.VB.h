//
// Created by Bartol on 10.11.2021..
//

#ifndef LAB_2_LEXRES_H
#define LAB_2_LEXRES_H

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

VB_C_CPP_FILE(
	
	ENUM lexres_ID
	START
		lexres_ID_IDN,
		lexres_ID_BROJ,
		lexres_ID_OP_PRIDRUZI,
		lexres_ID_OP_PLUS,
		lexres_ID_OP_MINUS,
		lexres_ID_OP_PUTA,
		lexres_ID_OP_DIJELI,
		lexres_ID_L_ZAGRADA,
		lexres_ID_D_ZAGRADA,
		lexres_ID_KR_ZA,
		lexres_ID_KR_OD,
		lexres_ID_KR_DO,
		lexres_ID_KR_AZ,
	END
	
	STRUCT lexres_Entity
	START
		DECL source AS char* END;
		DECL id AS lexres_ID END;
	END
	
	DEFINE_LATER DECL lexres_list_length AS uint32_t       END;
	DEFINE_LATER DECL lexres_list        AS lexres_Entity* END;
	
	FUNCTION lexres_init OF () AS void END
	FUNCTION lexres_end  OF () AS void END
)

#endif //LAB_2_LEXRES_H
