#pragma once
#ifndef _TinyChillParser_Tree_TreeCreator_Header_
#define _TinyChillParser_Tree_TreeCreator_Header_
#include "base.hpp"
#include "Tree.hpp"
#include "TreeAllocator.hpp"
#include <stack>
#include <vector>

ChillParser_Begin

template <class TreeClass>
class TreeCreator
{
public:
	using Tree = TreeClass;
	using TreeAllocatorClass = TreeAllocator<Tree>;
	using NodePtr = typename Tree::NodePtr;
	using ElementNode = typename Tree::ElementNode;
	using TreeNode = typename Tree::TreeNode;
public:
	explicit TreeCreator(TreeAllocatorClass& allocator) : _allocator(allocator) {}

	bool addElementNode(NodePtr node);
	bool addTreeNode(NodePtr node);
	bool retTreeNode();

	bool isDone() {
		return _curr_stack.empty() && !root.isNull();
	}

	Tree getResult() {
		assert(isDone());
		return Tree(root);
	}

private:
	NodePtr root;
	TreeAllocatorClass& _allocator;
	std::stack<std::pair<NodePtr, std::vector<NodePtr>>> _curr_stack;
};


ChillParser_End

#endif

#ifdef NDEBUG
static void print_test(const char *msg, bool newline = true) {}
#else
#include <iostream>
static void print_test(const char* msg, bool newline = true) {
	if (!true) {
		std::cout << msg;
		if (newline) {
			std::cout << std::endl;
		}
	}
}

#endif

ChillParser_Begin

template <class TreeClass>
bool TreeCreator<TreeClass>::addElementNode(NodePtr node) {
	assert(node.isElementNode());

	print_test("addElementNode : ", false);
	if (root.isNull()) {
		print_test("root");
		root = node;
		return true;
	}
	else {
		if (!_curr_stack.empty()) {
			print_test("in node");
			_curr_stack.top().second.push_back(node);
			return true;
		}
		else {
			print_test("No add");
			return false;
		}
	}
}

template <class TreeClass>
bool TreeCreator<TreeClass>::addTreeNode(NodePtr node) {
	assert(node.isTreeNode());

	print_test("addNode : ", true);
	if (isDone()) {
		print_test("No add");
		return false;
	}
	if (root.isNull()) {
		print_test("root");
		root = node;
		_curr_stack.push(std::make_pair(node, std::vector<NodePtr>()));
		return true;
	}
	else {
		if (!_curr_stack.empty()) {
			print_test("in node");
			_curr_stack.top().second.push_back(node);
			_curr_stack.push(std::make_pair(node, std::vector<NodePtr>()));
			return true;
		}
		else {
			print_test("No add");
			return false;
		}
	}
}

template <class TreeClass>
bool TreeCreator<TreeClass>::retTreeNode() {
	print_test("retNode : ", true);
	if (_curr_stack.empty()) {
		print_test("No ret");
		return false;
	}
	print_test("to parent");
	auto length = _curr_stack.top().second.size();
	auto list = _allocator.allocNodeList(length);
	_allocator.copyTo(list, _curr_stack.top().second.data(), length);
	_curr_stack.top().first.template getAs<TreeNode>().setList(list, length);
	_curr_stack.pop();
	return true;
}

ChillParser_End
