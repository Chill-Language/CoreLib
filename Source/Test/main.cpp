#include "Tree/TreeCreator.hpp"
#include "Tree/TreeWalker.hpp"
#include "Lexer/Lexer.hpp"
#include "Lexer/SourceData.hpp"
#include "Lexer/Handler.hpp"

#include <iostream>
#include <fstream>
#include <string>

using Tree = ChillParser::Tree<std::string>;

// TODO

ChillParser_Begin
SourceData initSourceData(std::istream &input) {
	SourceData result;
	SourceOffset offset = 0;
	while (input) {
		std::string line;
		std::getline(input, line);
		result.data += line;


		std::cerr << "\"";
		for (auto c : line) {
			switch (c) {
			case '\n': std::cerr << "\\n"; break;
			case '\r': std::cerr << "\\r"; break;
			case '\t': std::cerr << "\\t"; break;
			case '\0': std::cerr << "\\0"; break;
			default: std::cerr << c;
			}
		}
		std::cerr << "\"" << std::endl;
	}
	return result;
}
ChillParser_End

static ChillParser::SourceData source_data; // TODO
static ChillParser::SourceOffset offset; // TODO
static ChillParser::MemoryListTreeAllocator<ChillParser::TokenTree> allocator; // TODO

auto test_create(const char *filename) -> ChillParser::TokenTree {
	if (filename == nullptr) {
		return ChillParser::TokenTree(ChillParser::TokenTree::NodePtr());
	}

	std::ifstream input(filename, std::ios::binary);

	ChillParser::SourceData source_data; // TODO

	return ChillParser::Lexer::parse(input, source_data, allocator, offset, [](const ChillParser::Lexer::SubmitFunc &submit, ChillParser::SourceOffset &offset) {
		return new ChillParser::TriggerHandler(submit, offset);
	});
}

auto main(int argc, const char *argv[]) -> int {
	//

	class _ : public ChillParser::TreeWalker<ChillParser::TokenTree> {
	public:
		void visitElementNode(ElementNode& node) override {
			std::cout << source_data.data.substr(node.data.begin, node.data.end - node.data.begin);
		}

		void visitTreeNode(TreeNode& node) override {
			auto btype = node.brackettype;
			std::pair<char, char> bracket;
			switch (btype) {
			case ChillParser::BracketType::Small:
				bracket = std::make_pair<char, char>('(', ')');
				break;
			case ChillParser::BracketType::Square:
				bracket = std::make_pair<char, char>('[', ']');
				break;
			case ChillParser::BracketType::Big:
				bracket = std::make_pair<char, char>('{', '}');
				break;
			}
			std::cout << bracket.first;
			for (auto i = 0; i != node.count; ++i) {
				traverseNode(node.begin()[i]);
				if (i != node.count - 1) {
					std::cout << " ";
				}
			}
			std::cout << bracket.second;
		}
	} Walker;


	std::ifstream input(argv[1], std::ios::binary);

	if (argc > 1) {
		std::cout << argv[1] << std::endl;
	}

	while (true) {
		auto tree = ChillParser::Lexer::parse(input, source_data, allocator, offset,
			[](const ChillParser::Lexer::SubmitFunc &submit, ChillParser::SourceOffset &offset) {
				return new ChillParser::TriggerHandler(submit, offset);
		});
		if (tree.root().isNull()) {
			break;
		}
		Walker.traverseNode(tree.root());
		std::cout << "\n";
	}

	return 0;
}
