//
// Created by Bartol on 11.11.2021..
//

#ifndef LAB_2_OUTPUT_H
#define LAB_2_OUTPUT_H

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

VB_C_CPP_FILE(
	FUNCTION output_output OF (uint32_t , char*) AS void END
	FUNCTION output_clear  OF ()                 AS void END
	FUNCTION output_error  OF ()                 AS void END
	
	FUNCTION output_init OF () AS void END
	FUNCTION output_end  OF () AS void END
)

#endif //LAB_2_OUTPUT_H
