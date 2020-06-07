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

	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct Node;
	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct ElementNode;
	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct TreeNode;
	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	class NodePtr;


	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct Node : NodeInfoType {

	};

	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct ElementNode : Node<ElementType, TreeNodeInfoType, NodeInfoType> {
		template <typename... Args>
		explicit ElementNode(Args&&... args) : data(std::forward<Args>(args)...) {}

		ElementType data;
	};

	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	struct TreeNode : Node<ElementType, TreeNodeInfoType, NodeInfoType>, TreeNodeInfoType {
		template <typename... Args>
		explicit TreeNode(Args&&... args) : TreeNodeInfoType(std::forward<Args>(args)...) {}

		void setList(NodePtr<ElementType, TreeNodeInfoType, NodeInfoType>* data, std::size_t count) {
			this->data = data;
			this->count = count;
		}

		TreeNode(const TreeNode&) = delete;
		TreeNode& operator=(const TreeNode&) = delete;
		const NodePtr<ElementType, TreeNodeInfoType, NodeInfoType>* begin() const { return data; }
		const NodePtr<ElementType, TreeNodeInfoType, NodeInfoType>* end() const { return data + count; }

		std::size_t count = 0;
		NodePtr<ElementType, TreeNodeInfoType, NodeInfoType>* data = nullptr;
	};

	template <typename ElementType, typename TreeNodeInfoType, typename NodeInfoType>
	class NodePtr : public BindPtr<Node<ElementType, TreeNodeInfoType, NodeInfoType>, NodeType>
	{
	public:
		using Node = NodeStructure::Node<ElementType, TreeNodeInfoType, NodeInfoType>;
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
