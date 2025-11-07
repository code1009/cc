/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_version.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "cc_export.h"

//===========================================================================
#include "cc_version.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_version_major 1
#define cc_version_minor 5





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static size_t cc_decimal_digits_uint32_t(uint32_t value)
{
	size_t digit_count = 1;
	while (value >= 10)
	{
		value /= 10;
		digit_count++;
	}
	return digit_count;
}

static char* cc_c_string_write_decimal_uint32_t(char* pointer, uint32_t value)
{
	cc_debug_assert(pointer != NULL);


	// uint32_t = 0x FFFF FFFF = 4,294,967,295
	// max 10 digits
	if (value == 0)
	{
		*pointer++ = '0';
	}
	else
	{
		while (value != 0)
		{
			*pointer++ = (char)('0' + (value % 10));
			value /= 10;
		}
	}


	return pointer;
}

static char* cc_c_string_write_char(char* pointer, char value)
{
	cc_debug_assert(pointer != NULL);


	*pointer++ = value;


	return pointer;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_version(cc_version_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->major = cc_version_major;
	ctx->minor = cc_version_minor;
}

cc_api bool cc_version_c_string(char* pointer, size_t max_count)
{
	if (pointer == NULL)
	{
		return false;
	}


	cc_version_t version;
	cc_version(&version);


	size_t major_count = cc_decimal_digits_uint32_t(version.major);
	size_t minor_count = cc_decimal_digits_uint32_t(version.minor);

	size_t required_size = major_count + 1 + minor_count + 1;
	if (max_count < required_size)
	{
		return false;
	}

	char* wpointer = pointer;
	wpointer = cc_c_string_write_decimal_uint32_t(wpointer, version.major);
	wpointer = cc_c_string_write_char(wpointer, '.');
	wpointer = cc_c_string_write_decimal_uint32_t(wpointer, version.minor);
	wpointer = cc_c_string_write_char(wpointer, '\0');

	return true;
}
