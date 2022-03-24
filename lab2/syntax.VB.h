//
// Created by Bartol on 10.11.2021..
//

#ifndef LAB_2_SYNTAX_H
#define LAB_2_SYNTAX_H

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "lexres.VB.h"

VB_C_CPP_FILE(
	FUNCTION syntax_init OF () AS void END
	FUNCTION syntax_end  OF () AS void END

	FUNCTION syntax_analyze OF () AS void END
)


#endif //LAB_2_SYNTAX_H
