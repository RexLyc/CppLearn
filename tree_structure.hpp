#pragma once
#include<vector>
namespace lyc_algorithm {

	template<typename T>
	struct binary_tree_node {
		binary_tree_node* parent;
		binary_tree_node* left;
		binary_tree_node* right;
		T data;
		// ���ݴ洢����
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
		// ����һ���ڲ��ڵ�
		redblack_tree_node(redblack_tree_node* parent
			, const T& data,  NODE_COLOR color = NODE_COLOR::RED)
			:binary_search_tree_node<T>(parent
				, new redblack_tree_node(this), new redblack_tree_node(this), data)
			, color(color) ,is_nil(false){}

		// ����һ�������ݵ�Ҷ�ӽڵ�
		redblack_tree_node(redblack_tree_node* parent)
			:binary_search_tree_node<T>(parent), color(NODE_COLOR::BLACK) ,is_nil(true) {}

		NODE_COLOR color;
		bool is_nil;

		// ��������ɾ���ڵ�Ӧ��ֱ�ӱ�ɾ����������תΪҶ�ӽڵ�
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

		// ��Ҷ�ӽڵ�תΪ�ڲ��ڵ㣬���Զ������µ�Ҷ�ӽڵ�
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

		// ����ʱɾ�����ڵ�Ҷ�Ӻ��ӽڵ�
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

	// �����������������µĸ�
	template<typename T>
	binary_tree_node<T>* left_rotate(binary_tree_node<T>* parent) {
		binary_tree_node<T>* right = parent->right;
		binary_tree_node<T>* parent_parent = parent->parent;
		if (!right) // �������Һ��ӣ��ṹ��֧������
			return nullptr;
		// �¸����ϸ���غ��ӽڵ��ϵ���
		parent->right = right->left;
		if (right->left) {
			right->left->parent = parent;
		}
		parent->parent = right;
		right->left = parent;
		right->parent = parent_parent;
		// �����¸���ԭ���ĸ��ڵ�֮��Ĺ�ϵ
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

	// �����������������µĸ�
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

	// �ڶ����������в�ѯָ�����ݣ�����nullptr��������ݽڵ㣬�������ǰdata�Ľڵ�
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

	// ������������µĽڵ�ָ���б�
	template<typename T>
	void inorder(binary_tree_node<T>* root, std::vector<binary_tree_node<T>*>& in_order_list) {
		if (!root || !root->times)
			return;
		inorder(root->left, in_order_list);
		in_order_list.push_back(root);
		inorder(root->right, in_order_list);
	}

	// ��ȡ��ǰ�����������µ���Сֵ�ڵ㣬�����ڣ�����������nullptr�������򷵻ض�Ӧ�ڵ�
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
	// �������������data�ĺ�̣�Ҫ��data�������������
	template<typename T>
	binary_search_tree_node<T>* successor(binary_search_tree_node<T>* root, const T& data) {
		binary_tree_node<T>* current = query(root,data);
		if (!current || !current->times)
			throw std::exception("data not in tree, can't search successor");
		// ������������������������Сֵ
		if (current->right && current->right->times)
			return tree_minimum(reinterpret_cast<binary_search_tree_node<T>*>(current->right));
		// ������Ҫ�ҵ�������е�һ�����Լ����
		binary_tree_node<T>* memory = current->parent;
		while (memory && memory->right==current) {
			current = memory;
			memory = memory->parent;
		}
		return reinterpret_cast<binary_search_tree_node<T>*>(memory);
	}

	// �����ָ������
	template<typename T>
	void redblack_tree_insert_fix(redblack_tree_node<T>** root, redblack_tree_node<T>* new_node) {
		redblack_tree_node<T>* current_node = new_node;
		// currentʼ��ָ��һ����ڵ㣬����˳�����Ϊ���ִ���ڵ㣬���߸��ڵ㲻��
		while (current_node->parent && to_rbnode(current_node->parent)->color == NODE_COLOR::RED) {
			if (current_node->parent == current_node->parent->parent->left) { // ���ڵ�Ϊ����
				if (current_node->parent->parent->right 
					&& to_rbnode(current_node->parent->parent->right)->color == NODE_COLOR::RED) {
					// ����һ
					to_rbnode(current_node->parent->parent->right)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent->parent);
				}
				else if (current_node == current_node->parent->right) {
					// ���ζ�
					current_node = to_rbnode(left_rotate(current_node->parent)->left);
				}
				else {
					// ������
					to_rbnode(current_node->parent)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->parent)->color = NODE_COLOR::RED;
					// ���Ը���ת����Ҫ���¸�����ֵ
					if (current_node->parent->parent == *root)
						*root = to_rbnode(right_rotate(current_node->parent->parent));
					else
						right_rotate(current_node->parent->parent);
				};
			}
			else { // ������������������������滻Ϊ�ҡ���������֮��Ȼ
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
		// ��Իص������������Ҫ��֤���Ǻ�ɫ
		(*root)->color = NODE_COLOR::BLACK;
	}

	// ��������������
	template<typename T>
	void redblack_tree_insert(redblack_tree_node<T>** root, const T& data) {
		// ����ֱ�Ӳ���
		if (!*root) {
			*root = new redblack_tree_node<T>(nullptr, data, NODE_COLOR::BLACK);
			return;
		}
		// ��ѯ�Ƿ��Ѿ�����
		redblack_tree_node<T>* query_node = to_rbnode(query(*root, data));
		if (!query_node->is_nil) {
			query_node->times++;
			return;
		}
		// �����ڵ�����£�query���ص���ǡ�ÿ����ڲ���Ŀ����ݽڵ㣬����Ϊ�ڲ��ڵ�
		redblack_tree_node<T>* insert_node = query_node;
		insert_node->to_inner(data);
		// �ָ����������
		redblack_tree_insert_fix(root, insert_node);
	}

	// ɾ��������ڵ������ʻָ�
	template<typename T>
	void redblack_tree_delete_fix(redblack_tree_node<T>** root, redblack_tree_node<T>* current_node) {
		// �˳�����Ϊ�ִ���ڵ㣬��ǰ�ڵ㲻Ϊ��ɫ
		while (current_node != *root && current_node->color == NODE_COLOR::BLACK) {
			if (current_node == current_node->parent->left) { // ��ǰ�ڵ�������
				if (to_rbnode(current_node->parent->right)->color == NODE_COLOR::RED
					&& to_rbnode(current_node->parent->right->left)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					// ����һ
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
					// ���ζ�
					to_rbnode(current_node->parent->right)->color = NODE_COLOR::RED;
					current_node = to_rbnode(current_node->parent);
				}
				else if (to_rbnode(current_node->parent->right)->color == NODE_COLOR::BLACK
					&& to_rbnode(current_node->parent->right->right)->color == NODE_COLOR::BLACK) {
					// ������
					to_rbnode(current_node->parent->right->left)->color = NODE_COLOR::BLACK;
					to_rbnode(current_node->parent->right)->color = NODE_COLOR::RED;
					if (current_node->parent->right == *root)
						*root = to_rbnode(right_rotate(current_node->parent->right));
					else
						right_rotate(current_node->parent->right);
				}
				else {
					// ������
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
		// ���ָ�Ϊ�ڣ����߶Ե�ǰ��ڵ���Ӻ�ɫ����ɶ�ȱʧ�ĺ�ɫ�Ĳ��䣩
		current_node->color = NODE_COLOR::BLACK;
	}

	// �Ӻ������ɾ��ָ������
	template<typename T>
	bool redblack_tree_delete(redblack_tree_node<T>** root, const T& data) {
		redblack_tree_node<T>* node=to_rbnode(query(*root, data));
		// ���ݲ�����
		if (node->is_nil)
			return false;
		// �����ж�ݣ���ʱ����ɾ���ڵ�
		if (node->times > 1) {
			node->times--;
			return true;
		}
		// Ѱ��ɾ���ڵ�
		redblack_tree_node<T>* remove_node=nullptr;
		if (to_rbnode(node->left)->is_nil 
			|| to_rbnode(node->right)->is_nil) {
			remove_node = node;
		}
		else {
			// ����ڵ���˫���ӣ��򲻻ᱻֱ��ɾ�������ᱻ���̽ڵ������������̽ڵ�ᱻɾ��
			// ��̽ڵ���Ա�ɾ����ԭ���ǣ��ڵ�ǰ�ڵ�˫���ӵ�ǰ���£���̽ڵ�������һ���Һ���
			remove_node = to_rbnode(successor(*root, node->data));
		}
		// Ѱ�ұ����ڵ㣨��̽ڵ�����ӻ��Һ��ӣ�
		redblack_tree_node<T>* save_node = nullptr;
		if (to_rbnode(remove_node->left)->is_nil) {
			save_node = to_rbnode(remove_node->right);
			remove_node->right = nullptr;
		}
		else {
			save_node = to_rbnode(remove_node->left);
			remove_node->left = nullptr;
		}
		// �ɺ�̽ڵ����ʱ����Ҫ��������
		if (remove_node != node) {
			node->data = remove_node->data;
			node->times = remove_node->times;
		}
		// �͸��ڵ�Ĺ�ϵ����
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
			// ɾ�����ڵ�ʱ���¸�
			*root = save_node;
		}
		// ɾ����һ���ڽڵ㣬����·���ڸ߶ȼ�һ����Ҫ�ָ�����
		if(remove_node->color==NODE_COLOR::BLACK)
			redblack_tree_delete_fix(root, save_node);
		// �Ƴ���ɾ���ڵ�
		delete remove_node;
		return true;
	}

	// ���ߺ�������ӡ�ڲ��ڵ�ֵ����ɫ�����ڵ�ֵ
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