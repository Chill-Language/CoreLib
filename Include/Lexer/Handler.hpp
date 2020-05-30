#pragma once
#ifndef _TinyChillParser_Lexer_Handler_Header_
#define _TinyChillParser_Lexer_Handler_Header_
#include "base.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include <cstring>

ChillParser_Begin

const bool parse_single_quote_as_string = false; // TODO

inline TokenType get_bracket(int c) {
	switch (c) {
	case '(': return TokenType::LeftSmallBracket;
	case ')': return TokenType::RightSmallBracket;
	case '[': return TokenType::LeftSquareBracket;
	case ']': return TokenType::RightSquareBracket;
	case '{': return TokenType::LeftBigBracket;
	case '}': return TokenType::RightBigBracket;
	default:  return TokenType::None;
	}
}

inline bool is_comment_prefix(int c) {
	return c == ';';
}

inline bool is_end(int c) {
	return (get_bracket(c) != TokenType::None) || is_comment_prefix(c) || std::isspace(c);
}

class TriggerHandler : public Lexer::Handler
{
public:
	using Lexer::Handler::Handler;

	Lexer::HandleResult handle(int c, SourceOffset offset, Lexer::Handler *&changed) override;
};

ChillParser_End

#endif
