#include "Lexer/Handler.hpp"
#include "Lexer/SourceData.hpp"

ChillParser_Begin

using Handler = Lexer::Handler;
using HandleResult = Lexer::HandleResult;

class NormalHandler : public Handler
{
public:
	using Handler::Handler;

	HandleResult handle(int c, SourceOffset offset, Handler *&changed) override {
		if (std::isspace(c) || (get_bracket(c) != TokenType::None)) {
			submit(Token(TokenType::Normal, token_start, offset));
			changed = new TriggerHandler(submit, token_start);
			return HandleResult::Change;
		}
		else if (c == '"' || c == '\'' || c == ';') {
			changed = new TriggerHandler(submit, token_start, false);
			return HandleResult::Change;
		}
		else {
			return HandleResult::Next;
		}
	}
};

class MatchHandler : public Handler
{
public:
	using Handler::Handler;

	HandleResult handle(int c, SourceOffset offset, Handler *&changed) override {
		if (last_c == 0) {
			match_char = c;
		}
		else {
			if (matched) {
				if (is_end(c)) {
					start_new = true;
				}
				if (start_new) {
					submit(Token(TokenType::String, token_start, offset));
					token_start = offset;
				}

				changed = new TriggerHandler(submit, token_start, start_new);
				return HandleResult::Change;
			}
			else {
				if (c == match_char && !(last_c == '\\' && last_last_c != '\\')) {
					matched = true;
					return HandleResult::Next;
				}
			}
		}

		last_last_c = last_c;
		last_c = c;

		return HandleResult::Next;
	}

	int last_c = 0;
	int last_last_c = 0;
	int match_char = 0;
	int matched = false;
};

class CommentHandler : public Handler
{
public:
	using Handler::Handler;

	HandleResult handle(int c, SourceOffset offset, Handler *&changed) override {
		if (c == '\n' || c == '\r') {
			changed = new TriggerHandler(submit, token_start);
			return HandleResult::Change;
		}
		return HandleResult::Next;
	}
};

class BracketHandler : public Handler
{
public:
	using Handler::Handler;

	HandleResult handle(int c, SourceOffset offset, Handler *&changed) override {
		assert(token_start == offset);
		submit(Token(get_bracket(c), token_start, token_start + 1));
		token_start = offset + 1;
		changed = new TriggerHandler(submit, token_start);
		return HandleResult::ChangeNext;
	}
};

HandleResult TriggerHandler::handle(int c, SourceOffset offset, Handler *&changed) {
	if (start_new) {
		token_start = offset;
	}
	if (std::isspace(c)) {
		start_new = true;
		return HandleResult::Next;
	}
	else if (get_bracket(c) != TokenType::None) {
		changed = new BracketHandler(submit, token_start);
		return HandleResult::Change;
	}
	else if ((c == '"') || (c == '\'' && parse_single_quote_as_string)) {
		changed = new MatchHandler(submit, token_start, start_new);
		return HandleResult::Change;
	}
	else if (c == '\'' && !parse_single_quote_as_string) {
		changed = new NormalHandler(submit, token_start, start_new);
		return HandleResult::ChangeNext;
	}
	else if (c == ';') {
		changed = new CommentHandler(submit, token_start);
		return HandleResult::Change;
	}
	else {
		changed = new NormalHandler(submit, token_start);
		return HandleResult::Change;
	}
}

ChillParser_End
