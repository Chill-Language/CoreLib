#pragma once
#ifndef _TinyChillParser_Tree_Node_Header_
#define _TinyChillParser_Tree_Node_Header_
#include "base.hpp"
#include "Utils/BindPtr.hpp"
#include <cstddef>
#include <utility>

ChillParser_Begin

namespace NodeStructure
{
	enum class NodeType {
		Unknown = 0,
		Element,
		Tree
	};

	template <typename T>
	bool operator<=(const NodeType &lhs, const T &rhs) {
		return static_cast<uint64_t>(lhs) <= rhs;
	}

	class BlankNodeInfoType {};
	class BlankTreeNodeInfoType {};

	template <typename NodeInfoType = BlankNodeInfoType>
	struct Node;
	template <typename ElementType, typename NodeInfoType = BlankNodeInfoType>
	struct ElementNode;
	template <typename TreeNodeInfoType = BlankTreeNodeInfoType, typename NodeInfoType = BlankNodeInfoType>
	struct TreeNode;
	template <typename NodeInfoType = BlankNodeInfoType>
	class NodePtr;


	template <typename NodeInfoType>
	struct Node : NodeInfoType {

	};

	template <typename ElementType, typename NodeInfoType>
	struct ElementNode : Node<NodeInfoType> {
		template <typename... Args>
		explicit ElementNode(Args&&... args) : data(std::forward<Args>(args)...) {}

		ElementType data;
	};

	template <typename TreeNodeInfoType, typename NodeInfoType>
	struct TreeNode : Node<NodeInfoType>, TreeNodeInfoType {
		template <typename... Args>
		explicit TreeNode(Args&&... args) : TreeNodeInfoType(std::forward<Args>(args)...) {}

		void setList(NodePtr<NodeInfoType>* data, std::size_t count) {
			this->data = data;
			this->count = count;
		}

		TreeNode(const TreeNode&) = delete;
		TreeNode& operator=(const TreeNode&) = delete;
		const NodePtr<NodeInfoType>* begin() const { return data; }
		const NodePtr<NodeInfoType>* end() const { return data + count; }

		std::size_t count = 0;
		NodePtr<NodeInfoType>* data = nullptr;
	};

	template <typename NodeInfoType>
	class NodePtr : public BindPtr<Node<NodeInfoType>, NodeType>
	{
	public:
		using Node = NodeStructure::Node<NodeInfoType>;
	public:
		NodePtr() = default;
		NodePtr(const Node *data, NodeType property) : BindPtr<Node, NodeType>(data, property) {}
		
		bool isElementNode() { return this->property() == NodeType::Element; }
		bool isTreeNode() { return this->property() == NodeType::Tree; }

		template <typename T> T* getAsPtr() { return &getAs<T>(); }

		template <typename T> T& getAs() { return *static_cast<T*>(this->get()); }
		template <typename T> const T& getAs() const { return const_cast<NodePtr*>(this)->getAs<T>(); }
	};
}

ChillParser_End

#endif
