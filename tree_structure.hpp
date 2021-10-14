#pragma once
#include<vector>
namespace lyc_algorithm {

	template<typename T>
	struct binary_tree_node {
		binary_tree_node* parent;
		binary_tree_node* left;
		binary_tree_node* right;
		T data;
		// 数据存储次数
		std::size_t times;
		binary_tree_node(binary_tree_node* parent
			, binary_tree_node* left, binary_tree_node* right, const T& data)
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
		binary_search_tree_node(binary_search_tree_node* parent
			, binary_search_tree_node* left, binary_search_tree_node* right, const T& data)
			:binary_tree_node<T>(parent, left, right, data) {}
		binary_search_tree_node()
			:binary_tree_node<T>() {}
		binary_search_tree_node(binary_search_tree_node* parent)
			:binary_tree_node<T>(parent) {}
	};

	template<typename T>
	struct redblack_tree_node: public binary_search_tree_node<T> {
		// 创建一个内部节点
		redblack_tree_node(redblack_tree_node* parent
			, const T& data,  NODE_COLOR color = NODE_COLOR::RED)
			:binary_search_tree_node<T>(parent
				, new redblack_tree_node(this), new redblack_tree_node(this), data)
			, color(color) ,is_nil(false){}

		// 创建一个无数据的叶子节点
		redblack_tree_node(redblack_tree_node* parent)
			:binary_search_tree_node<T>(parent), color(NODE_COLOR::BLACK) ,is_nil(true) {}

		NODE_COLOR color;
		bool is_nil;

		// 废弃：待删除节点应当直接被删除，而不是转为叶子节点
		void to_nil() {
			if (is_nil) {
				throw std::exception("this node is already a nil");
			}
			else if (!(to_rbnode(binary_tree_node<T>::left)->is_nil)
				|| !(to_rbnode(binary_tree_node<T>::right)->is_nil)) {
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

		// 将叶子节点转为内部节点，并自动创建新的叶子节点
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

		// 析构时删除存在的叶子孩子节点
		~redblack_tree_node() {
			if (binary_tree_node<T>::left && to_rbnode(binary_tree_node<T>::left)->is_nil) {
				delete binary_tree_node<T>::left;
			}
			if (binary_tree_node<T>::right && to_rbnode(binary_tree_node<T>::right)->is_nil) {
				delete binary_tree_node<T>::right;
			}
		}
	};

	template<typename T>
	constexpr redblack_tree_node<T>* to_rbnode(binary_tree_node<T>* node) {
		return reinterpret_cast<redblack_tree_node<T>*>(node);
	}

	// 左旋：返回左旋后新的根
	template<typename T>
	binary_tree_node<T>* left_rotate(binary_tree_node<T>* parent) {
		binary_tree_node<T>* right = parent->right;
		binary_tree_node<T>* parent_parent = parent->parent;
		if (!right) // 不存在右孩子，结构不支持左旋
			return nullptr;
		// 新根和老根相关孩子节点关系变更
		parent->right = right->left;
		if (right->left) {
			right->left->parent = parent;
		}
		parent->parent = right;
		right->left = parent;
		right->parent = parent_parent;
		// 建立新根到原根的父节点之间的关系
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

	// 右旋，返回右旋后新的根
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

	// 在二叉搜索树中查询指定数据，返回nullptr，或空数据节点，或包含当前data的节点
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

	// 返回中序遍历下的节点指针列表
	template<typename T>
	void inorder(binary_tree_node<T>* root, std::vector<binary_tree_node<T>*>& in_order_list) {
		if (!root || !root->times)
			return;
		inorder(root->left, in_order_list);
		in_order_list.push_back(root);
		inorder(root->right, in_order_list);
	}

	// 获取当前根所带子树下的最小值节点，不存在（空树）返回nullptr，存在则返回对应节点
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
	// 返回中序遍历下data的后继，要求data必须存在于树中
	template<typename T>
	binary_search_tree_node<T>* successor(binary_search_tree_node<T>* root, const T& data) {
		binary_tree_node<T>* current = query(root,data);
		if (!current || !current->times)
			throw std::exception("data not in tree, can't search successor");
		// 有右子树，返回右子树中最小值
		if (current->right && current->right->times)
			return tree_minimum(reinterpret_cast<binary_search_tree_node<T>*>(current->right));
		// 否则需要找到父结点中第一个比自己大的
		binary_tree_node<T>* memory = current->parent;
		while (memory && memory->right==current) {
			current = memory;
			memory = memory->parent;
		}
		return reinterpret_cast<binary_search_tree_node<T>*>(memory);
	}

	// 插入后恢复红黑树
	template<typename T>
	void redblack_tree_insert_fix(redblack_tree_node<T>** root, redblack_tree_node<T>* new_node) {
		redblack_tree_node<T>* current_node = new_node;
		// current始终指向一个红节点，因此退出条件为：抵达根节点，或者父节点不红
		while (current_node->parent && to_rbnode(current_node->parent)->color == NODE_COLOR::RED) {
			if (current_node->parent == current_node->parent->parent->left) { // 父节点为左孩子
				if (current_node->parent->parent->right 
					&& to_rbnode(current_node->parent->parent->right)->color == NODE_COLOR::RED) {
					// 情形一
					to_rbnode(current_node->parent->parent->right)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent->parent);
				}
				else if (current_node == current_node->parent->right) {
					// 情形二
					current_node = to_rbnode(left_rotate(current_node->parent)->left);
				}
				else {
					// 情形三
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					// 若对根旋转，需要更新根结点的值
					if (current_node->parent->parent == *root)
						*root = to_rbnode(right_rotate(current_node->parent->parent));
					else
						right_rotate(current_node->parent->parent);
				};
			}
			else { // 镜像情况，将所有左、左旋，替换为右、右旋，反之亦然
				if (current_node->parent->parent->left 
					&& to_rbnode(current_node->parent->parent->left)->color == NODE_COLOR::RED) {
					to_rbnode(current_node->parent->parent->left)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent->parent);
				}
				else if (current_node == current_node->parent->left) {
					current_node = to_rbnode(right_rotate(current_node->parent)->right);
				}
				else {
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					if (current_node->parent->parent == *root)
						*root = to_rbnode(left_rotate(current_node->parent->parent));
					else
						left_rotate(current_node->parent->parent);
				}
			}
		}
		// 针对回到根的情况，需要保证根是黑色
		(*root)->color = NODE_COLOR::BLACK;
	}

	// 向红黑树插入数据
	template<typename T>
	void redblack_tree_insert(redblack_tree_node<T>** root, const T& data) {
		// 空树直接插入
		if (!*root) {
			*root = new redblack_tree_node<T>(nullptr, data, NODE_COLOR::BLACK);
			return;
		}
		// 查询是否已经存在
		redblack_tree_node<T>* query_node = to_rbnode(query(*root, data));
		if (!query_node->is_nil) {
			query_node->times++;
			return;
		}
		// 不存在的情况下，query返回的是恰好可用于插入的空数据节点，提升为内部节点
		redblack_tree_node<T>* insert_node = query_node;
		insert_node->to_inner(data);
		// 恢复红黑树性质
		redblack_tree_insert_fix(root, insert_node);
	}

	// 删除红黑树节点后的性质恢复
	template<typename T>
	void redblack_tree_delete_fix(redblack_tree_node<T>** root, redblack_tree_node<T>* current_node) {
		// 退出条件为抵达根节点，或当前节点不为黑色
		while (current_node != *root && current_node->color == NODE_COLOR::BLACK) {
			if (current_node == current_node->parent->left) { // 当前节点是左孩子
				if (to_rbnode(current_node->parent->right)->color == NODE_COLOR::RED
					&& to_rbnode(current_node->parent->right->left)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					// 情形一
					to_rbnode(current_node->parent)->color = NODE_COLOR::RED;
					to_rbnode(current_node->parent->right)->color = NODE_COLOR::BLACK;
					if (current_node->parent == *root)
						*root = to_rbnode(left_rotate(current_node->parent));
					else
						left_rotate(current_node->parent);
				}
				else if (to_rbnode(current_node->parent->right)->is_nil
					|| (to_rbnode(current_node->parent->right)->color == NODE_COLOR::BLACK
						&& to_rbnode(current_node->parent->right->left)->color == NODE_COLOR::BLACK
						&& to_rbnode(current_node->parent->right->right)->color == NODE_COLOR::BLACK)) {
					// 情形二
					to_rbnode(current_node->parent->right)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent);
				}
				else if (to_rbnode(current_node->parent->right)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					// 情形三
					to_rbnode(current_node->parent->right->left)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->right)->color = NODE_COLOR::RED;
					if (current_node->parent->right == *root)
						*root = to_rbnode(right_rotate(current_node->parent->right));
					else
						right_rotate(current_node->parent->right);
				}
				else {
					// 情形四
					to_rbnode(current_node->parent->right)->color 
						= to_rbnode(current_node->parent)->color;
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->right->right)->color = NODE_COLOR::BLACK;
					if (current_node->parent == *root)
						*root = to_rbnode(left_rotate(current_node->parent));
					else
						left_rotate(current_node->parent);
				}
			}
			else {
				if (to_rbnode(current_node->parent->left)->color == NODE_COLOR::RED
					&& to_rbnode(current_node->parent->left->right)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->left->left)->color == NODE_COLOR::BLACK) {
					to_rbnode(current_node->parent)->color = NODE_COLOR::RED;
					to_rbnode(current_node->parent->left)->color = NODE_COLOR::BLACK;
					if (current_node->parent == *root)
						*root = to_rbnode(right_rotate(current_node->parent));
					else
						right_rotate(current_node->parent);
				}
				else if (to_rbnode(current_node->parent->left)->is_nil
					|| (to_rbnode(current_node->parent->left)->color == NODE_COLOR::BLACK
						&& to_rbnode(current_node->parent->left->right)->color == NODE_COLOR::BLACK
						&& to_rbnode(current_node->parent->left->left)->color == NODE_COLOR::BLACK)) {
					to_rbnode(current_node->parent->left)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent);
				}
				else if (to_rbnode(current_node->parent->left)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->left->left)->color == NODE_COLOR::BLACK) {
					to_rbnode(current_node->parent->left->right)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->left)->color = NODE_COLOR::RED;
					if (current_node->parent->left == *root)
						*root = to_rbnode(left_rotate(current_node->parent->left));
					else
						left_rotate(current_node->parent->left);
				}
				else {
					to_rbnode(current_node->parent->left)->color 
						= to_rbnode(current_node->parent)->color;
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->left->left)->color = NODE_COLOR::BLACK;
					if (current_node->parent == *root)
						*root = to_rbnode(right_rotate(current_node->parent));
					else
						right_rotate(current_node->parent);
					current_node = *root;
				}
			}
		}
		// 保持根为黑，或者对当前红节点添加黑色（完成对缺失的黑色的补充）
		current_node->color = NODE_COLOR::BLACK;
	}

	// 从红黑树中删除指定数据
	template<typename T>
	bool redblack_tree_delete(redblack_tree_node<T>** root, const T& data) {
		redblack_tree_node<T>* node=to_rbnode(query(*root, data));
		// 数据不存在
		if (node->is_nil)
			return false;
		// 数据有多份，暂时不用删除节点
		if (node->times > 1) {
			node->times--;
			return true;
		}
		// 寻找删除节点
		redblack_tree_node<T>* remove_node=nullptr;
		if (to_rbnode(node->left)->is_nil 
			|| to_rbnode(node->right)->is_nil) {
			remove_node = node;
		}
		else {
			// 如果节点有双孩子，则不会被直接删除，而会被其后继节点数据替代，后继节点会被删除
			// 后继节点可以被删除的原因是，在当前节点双孩子的前提下，后继节点至多有一个右孩子
			remove_node = to_rbnode(successor(*root, node->data));
		}
		// 寻找保留节点（后继节点的左孩子或右孩子）
		redblack_tree_node<T>* save_node = nullptr;
		if (to_rbnode(remove_node->left)->is_nil) {
			save_node = to_rbnode(remove_node->right);
			remove_node->right = nullptr;
		}
		else {
			save_node = to_rbnode(remove_node->left);
			remove_node->left = nullptr;
		}
		// 由后继节点替代时，需要复制数据
		if (remove_node != node) {
			node->data = remove_node->data;
			node->times = remove_node->times;
		}
		// 和父节点的关系重设
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
			// 删除根节点时更新根
			*root = save_node;
		}
		// 删除了一个黑节点，部分路径黑高度减一，需要恢复性质
		if(remove_node->color==NODE_COLOR::BLACK)
			redblack_tree_delete_fix(root, save_node);
		// 移除待删除节点
		delete remove_node;
		return true;
	}

	// 工具函数，打印内部节点值、颜色、父节点值
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
			std::cout << t->data << " " << enum_trans(to_rbnode(t)->color) << "  <<  ";
			if (t->parent)
				std::cout << t->parent->data << std::endl;
			else
				std::cout << "root" << std::endl;
		}
		std::cout << "====rbtree  end ====" << std::endl << std::endl;
	}
}