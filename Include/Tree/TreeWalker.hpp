#pragma once
#ifndef _TinyChillParser_Tree_TreeWalker_Header_
#define _TinyChillParser_Tree_TreeWalker_Header_

#include "base.hpp"
ChillParser_Begin

template <class TreeClass>
class TreeWalker
{
public:
	using Tree = TreeClass;
	using NodePtr = typename TreeClass::NodePtr;
	using ElementNode = typename TreeClass::ElementNode;
	using TreeNode = typename TreeClass::TreeNode;

public:
	TreeWalker() = default;
	virtual ~TreeWalker() = default;

	void traverseNode(NodePtr node) {
		assert(!node.isNull());
		if (node.isElementNode()) {
			auto& element_node = node.template getAs<ElementNode>();
			visitElementNode(element_node);
		}
		else if (node.isTreeNode()) {
			auto& tree_node = node.template getAs<TreeNode>();
			visitTreeNode(tree_node);
		}
	}

	virtual void visitElementNode(ElementNode& node) {

	}

	virtual void visitTreeNode(TreeNode& node) {
		traverseTreeNode(node);
	}

	virtual void traverseTreeNode(TreeNode& node) {
		for (const auto& sub_node : node) {
			traverseNode(sub_node);
		}
	}
};

ChillParser_End

#endif
