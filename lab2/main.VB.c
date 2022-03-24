//
// Created by Bartol on 10.11.2021..
//

#include "VB.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "syntax.VB.h"

VB_C_CPP_FILE(
	FUNCTION main OF (DECL argc AS int END, DECL argv AS char** END) AS int
	START
		syntax_init();

		syntax_analyze();

		syntax_end();
	END
)