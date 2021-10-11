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
		RED, BLACK
	};

	std::string enum_trans(const NODE_COLOR& e) {
		switch (e) {
			case NODE_COLOR::RED:
				return "RED";
			case NODE_COLOR::BLACK:
				return "BLACK";
			default:
				return "??";
		}
	}

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
		binary_tree_node<T>* right = parent->right;
		binary_tree_node<T>* parent_parent = parent->parent;
		if (!right) // can't left_rotate
			return nullptr;
		parent->right = right->left;
		if (right->left) {
			right->left->parent = parent;
		}
		parent->parent = right;
		right->left = parent;
		right->parent = parent_parent;
		if (parent_parent) {
			if (parent == parent_parent->left) {
				parent_parent->left = right;
			}
			else {
				parent_parent->right = right;
			}
		}
		return right;
	}

	template<typename T>
	binary_tree_node<T>* right_rotate(binary_tree_node<T>* parent) {
		binary_tree_node<T>* left = parent->left;
		binary_tree_node<T>* parent_parent = parent->parent;
		if (!left)
			return nullptr;
		parent->left = left->right;
		if (left->right) {
			left->right->parent = parent;
		}
		parent->parent = left;
		left->right = parent;
		left->parent = parent_parent;
		if (parent_parent) {
			if (parent == parent_parent->left) {
				parent_parent->left = left;
			}
			else {
				parent_parent->right = left;
			}
		}
		return left;
	}

	template<typename T>
	binary_search_tree_node<T>* query(binary_search_tree_node<T>* root, const T& data) {
		binary_search_tree_node<T>* current = root;
		while (current&&current->data!=data) {
			if (current->data > data) {
				current = reinterpret_cast<binary_search_tree_node<T>*>(current->left);
			}
			else {
				current = reinterpret_cast<binary_search_tree_node<T>*>(current->right);
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
	void redblack_tree_insert_fix(redblack_tree_node<T>*  root, redblack_tree_node<T>* new_node) {
		redblack_tree_node<T>* current_node = new_node;
		while (current_node->parent && reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color == NODE_COLOR::RED) {
			if (current_node->parent == current_node->parent->parent->left) {
				if (current_node->parent->parent->right && reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent->right)->color == NODE_COLOR::RED) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent->right)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent)->color = NODE_COLOR::RED;
					current_node = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent);
				}
				else if (current_node == current_node->parent->right) {
					current_node= reinterpret_cast<redblack_tree_node<T>*>(left_rotate(current_node->parent)->left);
				}
				else {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent)->color = NODE_COLOR::RED;
					right_rotate(current_node->parent->parent);
				}
			}
			else { // mirror
				if (current_node->parent->parent->left && reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent->left)->color == NODE_COLOR::RED) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent->left)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent)->color = NODE_COLOR::RED;
					current_node = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent);
				}
				else if (current_node == current_node->parent->left) {
					current_node = reinterpret_cast<redblack_tree_node<T>*>(right_rotate(current_node->parent)->right);
				}
				else {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent)->color = NODE_COLOR::RED;
					left_rotate(current_node->parent->parent);
				}
			}
		}
		root->color = NODE_COLOR::BLACK;
	}

	template<typename T>
	void redblack_tree_insert(redblack_tree_node<T>**root, const T& data) {
		if (!*root) {
			*root = new redblack_tree_node<T>(nullptr, data, NODE_COLOR::BLACK);
			return;
		}
		redblack_tree_node<T>* insert_successor = reinterpret_cast<redblack_tree_node<T>*>(successor(*root, data));
		// to be continue
		if (insert_successor->data == data) {
			insert_successor->times++;
		}
		else {
			redblack_tree_node<T>* new_node = new redblack_tree_node<T>(insert_successor, data);
			if (insert_successor->data < data) {
				insert_successor->right = new_node;
			}
			else {
				insert_successor->left = new_node;
			}
			redblack_tree_insert_fix(*root, new_node);
		}
	}

	template<typename T>
	bool redblack_tree_delete(redblack_tree_node<T>** root, const T& data) {
		auto node=query(*root, data);
		if (!node)
			return false;
	}
}