#include "Lexer/Lexer.hpp"
#include "Tree/TreeCreator.hpp"

#include <iostream>
#include <functional>
#include <ctime>
#include <memory>

ChillParser_Begin

static bool handle(int c, SourceOffset offset, std::unique_ptr<Lexer::Handler> &handler, bool is_new_line) {
	//std::cerr << offset << std::endl;
	Lexer::Handler *new_handler = nullptr;
	switch (handler->handle(c, offset, new_handler)) {
	case Lexer::HandleResult::Next:
		break;
	case Lexer::HandleResult::Change:
		handler.reset(new_handler);
		return true;
	case Lexer::HandleResult::ChangeNext:
		handler.reset(new_handler);
		break;
	case Lexer::HandleResult::ErrorOccur:
		// TODO
		std::cerr << "Error Occur" << std::endl;
		exit(-1);
		break;
	}
	return false;
}

TokenTree Lexer::parse(std::istream &input, SourceData &sourceData, TreeAllocator<TokenTree> &allocator, SourceOffset &offset,
	const HandlerCreator &start_handler_creator) {
	TreeCreator<TokenTree> creator(allocator);
	bool is_over = false;
	const SubmitFunc &func = [&](const Token &token) {
		//std::cout << to_string(token.type) << " "
		//		  << "(" << token.begin << ", " << token.end << ") "
		//		  << "'" << sourceData.data.substr(token.begin, token.end - token.begin) << "'" << std::endl;
		switch (token.type) {
		case TokenType::LeftSmallBracket:
			creator.addTreeNode(allocator.allocTreeNode(BracketTypeInfo{ BracketType::Small }));
			break;
		case TokenType::LeftSquareBracket:
			creator.addTreeNode(allocator.allocTreeNode(BracketTypeInfo{ BracketType::Square }));
			break;
		case TokenType::LeftBigBracket:
			creator.addTreeNode(allocator.allocTreeNode(BracketTypeInfo{ BracketType::Big }));
			break;
		case TokenType::RightSmallBracket:
		case TokenType::RightSquareBracket:
		case TokenType::RightBigBracket:
			creator.retTreeNode();
			if (creator.isDone()) {
				is_over = true;
			}
			break;
		default:
			creator.addElementNode(allocator.allocElementNode(token));
			if (creator.isDone()) {
				is_over = true;
			}
		}
	};

	SourceOffset token_start = offset;
	sourceData.linedata.push_back(offset);
	bool is_new_line = true;

	int c = 0;

	std::unique_ptr<Handler> handler(start_handler_creator(func, token_start));

	const auto next = [&]() {
		auto _c = input.get();
		sourceData.data.push_back(_c);  // TODO

		size_t max_count = 0x100000;
		size_t count = 0;

		while (handle(_c, offset, handler, is_new_line)) {
			if (++count == max_count) {
				assert(false && "Too many loops");
			}
		}

		offset += 1;
		return _c;
	};
	const auto handleLine = [&]() {
		sourceData.linedata.push_back(offset);
		is_new_line = true;
	};
	
	while (!input.eof()) {
		c = next();
		if (is_over || (c == -1)) {
			break;
		}
		switch (c) {
		case '\r':
			if (input.peek() == '\n') {
				next();
				if (is_over) {
					break;
				}
			}
		case '\n':
			handleLine();
			break;
		default:
			;
		}

		is_new_line = false;
	}

	if (creator.isDone()) {
		return creator.getResult();
	} else {
		return TokenTree(TokenTree::NodePtr());
	}
}

ChillParser_End
