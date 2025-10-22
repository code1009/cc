#ifndef cc_config_h
#define cc_config_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_config.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_config_debug 1

#define cc_config_element_macro_function 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_MSC_VER)

#define cc_config_compiler_unknown 0
#define cc_config_compiler_msvc 1

#else

#define cc_config_compiler_unknown 1
#define cc_config_compiler_msvc 0

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_WIN64) || defined(_WIN32)

#define cc_config_platform_os_unknown 0
#define cc_config_platform_os_windows 1

#else

#define cc_config_platform_os_unknown 1
#define cc_config_platform_os_windows 0

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1==cc_config_platform_os_windows

#if defined(_WIN64)
#define cc_config_platform_32bit 0
#define cc_config_platform_64bit 1
#else
#define cc_config_platform_32bit 1
#define cc_config_platform_64bit 0
#endif

#endif

//===========================================================================
#if 1==cc_config_platform_os_unknown

#define cc_config_platform_32bit 1
#define cc_config_platform_64bit 0

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_config_h

