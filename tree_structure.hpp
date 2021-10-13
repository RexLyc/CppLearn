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
		binary_tree_node(binary_tree_node* parent)
			:parent(parent), left(nullptr), right(nullptr), times(0) {}
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
		binary_search_tree_node(binary_search_tree_node* parent)
			:binary_tree_node<T>(parent) {}
	};

	template<typename T>
	struct redblack_tree_node: public binary_search_tree_node<T> {
		// create an inner node
		redblack_tree_node(redblack_tree_node* parent, const T& data,  NODE_COLOR color = NODE_COLOR::RED)
			:binary_search_tree_node<T>(parent, new redblack_tree_node(this), new redblack_tree_node(this), data), color(color) ,is_nil(false){}

		// create a nil node
		redblack_tree_node(redblack_tree_node* parent)
			:binary_search_tree_node<T>(parent), color(NODE_COLOR::BLACK) ,is_nil(true) {}

		NODE_COLOR color;
		bool is_nil;

		void to_nil() {
			if (is_nil) {
				throw std::exception("this node is already a nil");
			}
			else if (!(reinterpret_cast<redblack_tree_node<T>*>(binary_tree_node<T>::left)->is_nil)
				|| !(reinterpret_cast<redblack_tree_node<T>*>(binary_tree_node<T>::right)->is_nil)) {
				throw std::exception("to_nil should only apply to the node which has two nil childs");
			}
			else {
				delete binary_tree_node<T>::left;
				delete binary_tree_node<T>::right;
				binary_tree_node<T>::times = 0;
				is_nil = true;
				color = NODE_COLOR::BLACK;
			}
		}

		void to_inner(const T& data) {
			if (is_nil) {
				binary_tree_node<T>::data = data;
				binary_tree_node<T>::times = 1;
				is_nil = false;
				this->color = NODE_COLOR::RED;
				binary_tree_node<T>::left = new redblack_tree_node<T>(this);
				binary_tree_node<T>::right = new redblack_tree_node<T>(this);
			}
			else {
				throw std::exception("this node is already a inner node");
			}
		}

		~redblack_tree_node() {
			if (binary_tree_node<T>::left && reinterpret_cast<redblack_tree_node<T>*>(binary_tree_node<T>::left)->is_nil) {
				delete binary_tree_node<T>::left;
			}
			if (binary_tree_node<T>::right && reinterpret_cast<redblack_tree_node<T>*>(binary_tree_node<T>::right)->is_nil) {
				delete binary_tree_node<T>::right;
			}
		}
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

	/*
	* @return the node with data, or return a node that data times=0, or return nullptr
	*/
	template<typename T>
	binary_search_tree_node<T>* query(binary_search_tree_node<T>* root, const T& data) {
		binary_search_tree_node<T>* current = root;
		while (current&&current->times&&current->data!=data) {
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
		if (!root || !root->times)
			return;
		inorder(root->left, in_order_list);
		in_order_list.push_back(root);
		inorder(root->right, in_order_list);
	}

	template<typename T>
	binary_search_tree_node<T>* tree_minimum(binary_search_tree_node<T>* root) {
		binary_tree_node<T>* current = root;
		binary_tree_node<T>* memory = nullptr;
		while (current && current->times) {
			memory = current;
			current = current->left;
		}
		return reinterpret_cast<binary_search_tree_node<T>*>(memory);
	}

	/*
	* return the last data node that is after data in the inorder
	*/
	template<typename T>
	binary_search_tree_node<T>* successor(binary_search_tree_node<T>* root, const T& data) {
		// if data not in the tree : find the successor of the last value that is smaller than data
		// else : find the data node
		binary_tree_node<T>* current = query(root,data);
		// has right subtree
		if (current && current->right && current->right->times)
			return tree_minimum(reinterpret_cast<binary_search_tree_node<T>*>(current->right));
		binary_tree_node<T>* memory = current->parent;
		while (memory && memory->right==current) {
			current = memory;
			memory = memory->parent;
		}
		return reinterpret_cast<binary_search_tree_node<T>*>(memory);
	}

	template<typename T>
	void redblack_tree_insert_fix(redblack_tree_node<T>* root, redblack_tree_node<T>* new_node) {
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
					current_node = reinterpret_cast<redblack_tree_node<T>*>(left_rotate(current_node->parent)->left);
				}
				else {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent)->color = NODE_COLOR::RED;
					right_rotate(current_node->parent->parent);
				}
			}
			else { // mirror
				std::cout << current_node->data << std::endl;
				std::cout << enum_trans(reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->parent->left)->color) << std::endl;
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
	void redblack_tree_insert(redblack_tree_node<T>** root, const T& data) {
		if (!*root) {
			*root = new redblack_tree_node<T>(nullptr, data, NODE_COLOR::BLACK);
			return;
		}
		redblack_tree_node<T>* query_node = reinterpret_cast<redblack_tree_node<T>*>(query(*root, data));
		if (!query_node->is_nil) {
			query_node->times++;
			return;
		}
		redblack_tree_node<T>* insert_node = query_node;
		insert_node->to_inner(data);
		redblack_tree_insert_fix(*root, insert_node);
	}

	template<typename T>
	void redblack_tree_delete_fix(redblack_tree_node<T>* root, redblack_tree_node<T>* current_node) {
		while (current_node != root && current_node->color == NODE_COLOR::BLACK) {
			if (current_node == current_node->parent->left) {
				if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color == NODE_COLOR::RED
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->left)->color == NODE_COLOR::BLACK
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::RED;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color = NODE_COLOR::BLACK;
					//problems here !
					left_rotate(current_node->parent);
				}
				else if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->is_nil
					|| (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color == NODE_COLOR::BLACK
						&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->left)->color == NODE_COLOR::BLACK
						&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->right)->color == NODE_COLOR::BLACK)) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color = NODE_COLOR::RED;
					current_node = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent);
				}
				else if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color == NODE_COLOR::BLACK
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right->left)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color = NODE_COLOR::RED;
					right_rotate(current_node->parent->right);
				}
				else {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->right)->color = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					left_rotate(current_node->parent);
				}
			}
			else {
				if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color == NODE_COLOR::RED
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->right)->color == NODE_COLOR::BLACK
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->left)->color == NODE_COLOR::BLACK) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::RED;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color = NODE_COLOR::BLACK;
					left_rotate(current_node->parent);
				}
				else if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->is_nil
					|| (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color == NODE_COLOR::BLACK
						&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->right)->color == NODE_COLOR::BLACK
						&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->left)->color == NODE_COLOR::BLACK)) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color = NODE_COLOR::RED;
					current_node = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent);
				}
				else if (reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color == NODE_COLOR::BLACK
					&& reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->left)->color == NODE_COLOR::BLACK) {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left->right)->color = NODE_COLOR::BLACK;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color = NODE_COLOR::RED;
					right_rotate(current_node->parent->left);
				}
				else {
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent->left)->color = reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color;
					reinterpret_cast<redblack_tree_node<T>*>(current_node->parent)->color = NODE_COLOR::BLACK;
					left_rotate(current_node->parent);
				}
			}
		}
		current_node->color = NODE_COLOR::BLACK;
		//root->parent = nullptr;
	}

	template<typename T>
	bool redblack_tree_delete(redblack_tree_node<T>** root, const T& data) {
		redblack_tree_node<T>* node=reinterpret_cast<redblack_tree_node<T>*>(query(*root, data));
		if (node->is_nil)
			return false;
		if (node->times > 1) {
			node->times--;
			return true;
		}
		// calculate remove node
		redblack_tree_node<T>* remove_node=nullptr;
		if (reinterpret_cast<redblack_tree_node<T>*>(node->left)->is_nil || reinterpret_cast<redblack_tree_node<T>*>(node->right)->is_nil) {
			remove_node = node;
		}
		else {
			remove_node = reinterpret_cast<redblack_tree_node<T>*>(successor(*root, node->data));
		}
		// calculate save node
		redblack_tree_node<T>* save_node = nullptr;
		if (reinterpret_cast<redblack_tree_node<T>*>(remove_node->left)->is_nil) {
			save_node = reinterpret_cast<redblack_tree_node<T>*>(remove_node->right);
			remove_node->right = nullptr;
		}
		else {
			save_node = reinterpret_cast<redblack_tree_node<T>*>(remove_node->left);
			remove_node->left = nullptr;
		}
		// data copy
		if (remove_node != node) {
			node->data = remove_node->data;
			node->times = remove_node->times;
		}
		// parent reset
		save_node->parent = remove_node->parent;
		if (remove_node->parent) {
			// remove_node is NOT root
			if (remove_node->parent->left == remove_node) {
				remove_node->parent->left = save_node;
			}
			else {
				remove_node->parent->right = save_node;
			}
		}
		else {
			// remove_node is root
			*root = save_node;
		}
		if(remove_node->color==NODE_COLOR::BLACK)
				redblack_tree_delete_fix(*root, save_node);
		// remove the remove_node
		delete remove_node;
		return true;
	}

	template<typename T>
	void rbtree_print(redblack_tree_node<T>** root) {
		if (*root==nullptr) {
			std::cout << "rbtree empty" << std::endl;
			return;
		}
		std::cout << "====rbtree start====" << std::endl;
		std::vector<binary_tree_node<T>*> order_list;
		inorder<T>(*root, order_list);
		for (auto& t : order_list) {
			std::cout << t->data << " " << enum_trans(reinterpret_cast<redblack_tree_node<T>*>(t)->color) << "  <<  ";
			if (t->parent)
				std::cout << t->parent->data << std::endl;
			else
				std::cout << "root" << std::endl;
		}
		std::cout << "====rbtree  end ====" << std::endl << std::endl;
	}

	// TODO : 关于叶子的处理，没有那么简单，需要完成叶子<->内部节点的双向变换
}