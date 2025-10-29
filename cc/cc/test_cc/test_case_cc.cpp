/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

//===========================================================================
#include "test_cc.hpp"

#include "../cc/cc_code_template.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_version()
{
	char version_c_string[64];
	cc_version_c_string(version_c_string, sizeof(version_c_string) / sizeof(char));
	test_out << "version = " << version_c_string << test_tendl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_code_template()
{
	cc_code_template_t code_template;
	code_template.field = cc_code_template_macro;
	cc_code_template_function(&code_template);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_assert_1()
{
	cc_assert(0 != 0);
}

void test_case_cc_assert_2()
{
	cc_debug_assert(0 != 0);
}
