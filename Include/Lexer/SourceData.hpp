#pragma once
#ifndef _TinyChillParser_Lexer_SourceData_Header_
#define _TinyChillParser_Lexer_SourceData_Header_
#include "base.hpp"
#include <cstdint>
#include <string>
#include <vector>

ChillParser_Begin


using SourceOffset = std::uint32_t;  // TODO


struct SourceData
{
	std::string data;
	std::vector<SourceOffset> linedata;
};

ChillParser_End

#endif
