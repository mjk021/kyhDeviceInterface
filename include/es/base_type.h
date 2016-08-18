#ifndef __ES_BASE_TYPE_H__
#define __ES_BASE_TYPE_H__


#include <string>
#include <iostream>
#include <sstream>

#if defined(stdString)
#error You must include 'base_type.h' at first.
#endif

#if !defined(stdString)
#	if defined(UNICODE) || defined(_UNICODE)
#		define stdString			std::wstring
#	else
#		define stdString			std::string
#	endif
#endif

#if !defined(stdOStrStream)
#	if defined(UNICODE) || defined(_UNICODE)
#		define stdOStrStream			std::wostringstream
#	else
#		define stdOStrStream			std::ostringstream
#	endif
#endif



#endif // __ES_BASE_TYPE_H__