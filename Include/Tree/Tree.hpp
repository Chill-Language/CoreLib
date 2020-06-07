#pragma once
#ifndef _TinyChillParser_Tree_Tree_Header_
#define _TinyChillParser_Tree_Tree_Header_
#include "base.hpp"
#include "Node.hpp"

ChillParser_Begin

using BlankNodeInfoType = NodeStructure::BlankNodeInfoType;
using BlankTreeNodeInfoType = NodeStructure::BlankTreeNodeInfoType;

template <typename ElementType, typename TreeNodeInfoType = BlankTreeNodeInfoType, typename NodeInfoType = BlankNodeInfoType>
class Tree;

template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
class Tree
{
public:
	using NodePtr = NodeStructure::NodePtr<ElementType, TreeNodeInfoType, NodeInfoType>;
	using ElementNode = NodeStructure::ElementNode<ElementType, TreeNodeInfoType, NodeInfoType>;
	using TreeNode = NodeStructure::TreeNode<ElementType, TreeNodeInfoType, NodeInfoType>;

public:
	explicit Tree(NodePtr root)
		: _root(root) {}

	NodePtr root() const {
		return _root;
	}

private:
	NodePtr _root;
};

ChillParser_End

#endif
