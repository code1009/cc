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
#define cc_version_minor 0





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static size_t cc_decimal_digits(uint32_t value)
{
	size_t digit_count = 1;
	while (value >= 10)
	{
		value /= 10;
		digit_count++;
	}
	return digit_count;
}

static size_t cc_c_string_write_uint32_t(char* buffer, uint32_t value)
{
	char c_string_buffer[11]; // uint32_t 최대 10자리 + 안전 여유
	size_t c_string_index = 0;

	if (value == 0)
	{
		c_string_buffer[c_string_index++] = '0';
	}
	else
	{
		while (value != 0)
		{
			c_string_buffer[c_string_index++] = (char)('0' + (value % 10));
			value /= 10;
		}
	}

	for (size_t i = 0; i < c_string_index; i++)
	{
		buffer[i] = c_string_buffer[c_string_index - 1 - i];
	}

	return c_string_index;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_version(cc_version_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->major = cc_version_major;
	ctx->minor = cc_version_minor;
}

cc_api bool cc_version_c_string(char* version_c_string, size_t version_c_string_size)
{
	if (version_c_string == NULL)
	{
		return false;
	}


	cc_version_t version;
	cc_version(&version);


	size_t major_count = cc_decimal_digits(version.major);
	size_t minor_count = cc_decimal_digits(version.minor);

	size_t required_size = major_count + 1 + minor_count + 1;
	if (version_c_string_size < required_size)
	{
		return false;
	}

	char* write_pointer = version_c_string;
	size_t written;

	written = cc_c_string_write_uint32_t(write_pointer, version.major);
	write_pointer += written;

	*write_pointer++ = '.';

	written = cc_c_string_write_uint32_t(write_pointer, version.minor);
	write_pointer += written;

	*write_pointer = '\0';

	return true;
}
