#pragma once
#include<vector>
namespace lyc_algorithm {

	template<typename T>
	struct binary_tree_node {
		binary_tree_node* parent;
		binary_tree_node* left;
		binary_tree_node* right;
		T data;
		std::size_t times;
		binary_tree_node(binary_tree_node* parent, binary_tree_node* left, binary_tree_node* right, const T& data)
			:parent(parent), left(left), right(right), data(data),times(1) {}
		binary_tree_node()
			:parent(nullptr), left(nullptr), right(nullptr),times(0) {}
	};

	enum class NODE_COLOR {
		RED,BLACK
	};

	template<typename T>
	struct binary_search_tree_node : public binary_tree_node<T> {
		binary_search_tree_node(binary_search_tree_node* parent, binary_search_tree_node* left, binary_search_tree_node* right, const T& data)
			:binary_tree_node<T>(parent, left, right, data) {}
		binary_search_tree_node()
			:binary_tree_node<T>() {}
	};

	template<typename T>
	struct redblack_tree_node: public binary_search_tree_node<T> {
		redblack_tree_node(redblack_tree_node* parent, const T& data,NODE_COLOR color=NODE_COLOR::RED)
			:binary_search_tree_node<T>(parent, nullptr, nullptr, data), color(color) {}
		NODE_COLOR color;
	};

	template<typename T>
	binary_tree_node<T>* left_rotate(binary_tree_node<T>* parent) {
		binary_tree_node<T> right = parent->right;
		if (!right) // can't left_rotate
			return nullptr;
		if (right.left) {
			parent->right = right.left;
			right.left.parent = parent;
		}
		parent->parent = right;
		right.left = parent;
		return right;
	}

	template<typename T>
	binary_tree_node<T>* right_rotate(binary_tree_node<T>* parent) {
		binary_tree_node<T>* left = parent->left;
		if (!left)
			return nullptr;
		if (left->right) {
			parent->left = left->right;
			left->right->parent = parent;
		}
		parent->parent = left;
		left->right = parent;
		return parent;
	}

	template<typename T>
	binary_search_tree_node<T>* query(binary_search_tree_node<T>* root, const T& data) {
		binary_search_tree_node<T>* current = root;
		while (current&&current->data!=data) {
			if (current->data > data) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return current;
	}

	template<typename T>
	void inorder(binary_tree_node<T>* root, std::vector<binary_tree_node<T>*>& in_order_list) {
		if (!root)
			return;
		inorder(root->left, in_order_list);
		in_order_list.push_back(root);
		inorder(root->right, in_order_list);
	}

	template<typename T>
	binary_search_tree_node<T>* successor(binary_search_tree_node<T>* root, const T& data) {
		// if data not in the tree : find the successor of the last value that is smaller than data
		// else : find the data node
		binary_tree_node<T>* current = root;
		binary_tree_node<T>* memory = nullptr;
		while (current&&current->data!=data) {
			memory = current;
			if (current->data > data) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		if (current && current->data == data)
			return reinterpret_cast<binary_search_tree_node<T>*>(current);
		else
			return reinterpret_cast<binary_search_tree_node<T>*>(memory);
	}

	template<typename T>
	void insert(redblack_tree_node<T>**root, const T& data) {
		if (!*root) {
			*root = new redblack_tree_node<T>(nullptr, data, NODE_COLOR::BLACK);
		}
		redblack_tree_node<T>* insert_successor = reinterpret_cast<redblack_tree_node<T>*>(successor(*root, data));
		// to be continue
		
	}
}