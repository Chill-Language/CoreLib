#pragma once
#ifndef _TinyChillParser_Lexer_Lexer_Header_
#define _TinyChillParser_Lexer_Lexer_Header_
#include "base.hpp"
#include "SourceData.hpp"
#include "TokenTree.hpp"
#include "Tree/TreeAllocator.hpp"
#include <istream>
#include <functional>

ChillParser_Begin

class Lexer
{
public:
	enum class HandleResult {
		ErrorOccur,
		Next,
		Change,
		ChangeNext
	};
	using SubmitFunc = std::function<void(const Token &)>;
	class Handler {
	public:
		explicit Handler(const SubmitFunc &submit, SourceOffset &token_start, bool start_new = true)
			: submit(submit), token_start(token_start), start_new(start_new) {}
		virtual ~Handler() = default;

		virtual HandleResult handle(int c, SourceOffset offset, Handler *&changed) = 0;

		const SubmitFunc &submit;
		SourceOffset &token_start;
		bool start_new;
	};
	using HandlerCreator = std::function<Handler * (const SubmitFunc &, SourceOffset &)>;

	static TokenTree parse(std::istream &input, SourceData &sourceData, TreeAllocator<TokenTree> &allocator, SourceOffset &offset, const HandlerCreator &start_handler_creator);
};

ChillParser_End

#endif
