#pragma once
#ifndef _TinyChillParser_Lexer_TokenTree_Header_
#define _TinyChillParser_Lexer_TokenTree_Header_
#include "base.hpp"
#include "Token.hpp"
#include "Tree/Tree.hpp"

ChillParser_Begin

struct BracketTypeInfo
{
	BracketType brackettype = BracketType::None;
};

using TokenTree = Tree<Token, BracketTypeInfo>;

ChillParser_End

#endif
