#pragma once
#ifndef _TinyChillParser_Lexer_Token_Header_
#define _TinyChillParser_Lexer_Token_Header_
#include "base.hpp"
#include <cstdint>

ChillParser_Begin


enum class TokenType
{
	None = 0,           //
#define TokenType(Type) Type,
#include "TokenType.inc"
#undef TokenType
};

enum class BracketType
{
	None = 0,           //
	Small,              // ()
	Big,                // {}
	Square,             // []
};

constexpr const char * to_string(TokenType type) {
	switch (type) {
	case TokenType::None: return "None";
#define TokenType(Type) case TokenType::Type: return #Type;
#include "TokenType.inc"
#undef TokenType
	default: return "Unknown";
	}
}

using SourceOffset = std::uint32_t;

struct Token
{
	explicit Token() = default;

	explicit Token(TokenType type, SourceOffset begin, SourceOffset end)
		: type(type), begin(begin), end(end) {}

	TokenType type = TokenType::None;
	SourceOffset begin = 0;
	SourceOffset end = 0;
};

ChillParser_End

#endif
