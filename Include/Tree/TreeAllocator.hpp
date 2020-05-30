#pragma once
#ifndef _TinyChillParser_Tree_TreeAllocator_Header_
#define _TinyChillParser_Tree_TreeAllocator_Header_
#include "base.hpp"
#include "Tree.hpp"
#include <cstring>
#include <list>
#include <cstdlib>

ChillParser_Begin

template <class TreeClass>
class TreeAllocator
{
public:
	using Tree = TreeClass;
	using NodePtr = typename TreeClass::NodePtr;
	using ElementNode = typename TreeClass::ElementNode;
	using TreeNode = typename TreeClass::TreeNode;

public:
	virtual ~TreeAllocator() = default;

	template <typename... Args>
	NodePtr allocElementNode(Args&&... args) {
		NodePtr node = _allocElementNode();
		new (node.template getAsPtr<ElementNode>()) ElementNode(std::forward<Args>(args)...);
		return node;
	}

	template <typename... Args>
	NodePtr allocTreeNode(Args&&... args) {
		NodePtr node = _allocTreeNode();
		new (node.template getAsPtr<TreeNode>()) TreeNode(std::forward<Args>(args)...);
		return node;
	}

	NodePtr* allocNodeList(std::size_t length) {
		return _allocNodeList(length);
	}

	virtual void copyTo(NodePtr* dst, const NodePtr* src, std::size_t length) {
		std::memcpy(dst, src, length * sizeof(NodePtr));
	}

protected:
	virtual NodePtr _allocElementNode() = 0;
	virtual NodePtr _allocTreeNode() = 0;
	virtual NodePtr* _allocNodeList(std::size_t length) = 0;
};


template <class TreeClass>
class MemoryListTreeAllocator : public TreeAllocator<TreeClass>
{
public:
	using TreeClass_ = TreeClass;
	using NodePtr = typename TreeClass::NodePtr;
	using ElementNode = typename TreeClass::ElementNode;
	using TreeNode = typename TreeClass::TreeNode;

public:
	MemoryListTreeAllocator() = default;
	MemoryListTreeAllocator(const MemoryListTreeAllocator &) = delete;
	MemoryListTreeAllocator& operator=(const MemoryListTreeAllocator &) = delete;
	virtual ~MemoryListTreeAllocator() {
		for (auto node : _element_node_pool) {
			node.template getAs<ElementNode>().~ElementNode();
			std::free(node.get());
		}
		for (auto node : _tree_node_pool) {
			node.template getAs<TreeNode>().~TreeNode();
			std::free(node.get());
		}
		for (auto nodelist : _nodelist_pool) {
			std::free(nodelist);
		}
	}

protected:
	NodePtr _allocElementNode() override {
		NodePtr node(static_cast<ElementNode*>(std::malloc(sizeof(ElementNode))), ChillParser::NodeStructure::NodeType::Element);
		_element_node_pool.push_back(node);
		return node;
	}
	NodePtr _allocTreeNode() override {
		NodePtr node(static_cast<TreeNode*>(std::malloc(sizeof(TreeNode))), ChillParser::NodeStructure::NodeType::Tree);
		_tree_node_pool.push_back(node);
		return node;
	}
	NodePtr* _allocNodeList(std::size_t length) override {
		NodePtr* nodelist(static_cast<NodePtr*>(std::malloc(sizeof(NodePtr) * length)));
		_nodelist_pool.push_back(nodelist);
		return nodelist;
	}

private:
	std::list<NodePtr> _element_node_pool;
	std::list<NodePtr> _tree_node_pool;
	std::list<NodePtr*> _nodelist_pool;
};

ChillParser_End

#endif
