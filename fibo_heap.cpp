#pragma once
#include<exception>
#include<vector>
#include<cstdlib>
namespace lyc_algorithm {
	template<typename T>
	struct fibo_heap_node {
		fibo_heap_node* left, * right, * parent, * child;
		size_t degree;
		bool mark;
		T value;
		
		fibo_heap_node()
			:left(nullptr), right(nullptr)
			, parent(nullptr), child(nullptr)
			, mark(false), degree(0) {}

		fibo_heap_node(const T& value, fibo_heap_node* parent=nullptr)
			:left(this), right(this), parent(parent)
			, child(nullptr), value(value)
			, mark(false), degree(0) {

		}
	};

	template<typename T>
	class fibo_heap {
	private:
		fibo_heap_node<T>* min_root;
		size_t n;

		void consolidate() {
			std::vector<fibo_heap_node<T>*> root_degree_list(log2(n)+1,nullptr);
			std::vector<fibo_heap_node<T>*> root_list(n,nullptr);
			root_list.push_back(min_root);
			for (auto root_node = min_root->right
				; root_node != min_root
				; root_node == root_node->right) {
				root_list.push_back(root_node);
			}
			for (auto& root_node : root_list) {
				auto current_root = root_node;
				auto degree = current_root->degree;
				// merge all the save degree root with current_root
				for (; root_degree_list[degree];) {
					auto another_root = root_degree_list[degree];
					if (current_root->value>another_root->value) {
						// exchange value
						std::swap(current_root->value, another_root->value);
					}
					link(current_root, another_root);
					root_degree_list[degree] = nullptr;
					degree++;
				}
				root_degree_list[degree] = current_root;
			}
			min_root = nullptr;
			for (size_t i = 0; i != root_degree_list.size(); ++i) {
				if (root_degree_list[i]){
					// rebuild root list
					if (!min_root) {
						min_root = root_degree_list[i];
					}
					else {
						auto left = min_root->left;
						left->right = root_degree_list[i];
						root_degree_list[i]->left = left;
						min_root->left = root_degree_list[i];
						root_degree_list[i]->right = min_root;
					}
					if(root_degree_list[i]->value < min_root->value) {
						min_root = root_degree_list[i];
					}
				}
			}
		}

		void link(fibo_heap_node<T>* root, fibo_heap_node<T>* child) {
 			// make child to be the child of root
			child->left->right = child->right;
			child->right->left = child->left;
			if (root->child) {
				auto left = root->child->left;
				left->right = child;
				child->left = left;
				child->right = root->child;
				root->child->left = child;
			}
			else {
				root->child = child;
				child->left = child;
				child->right = child;
			}
			child->mark = false;
			root->degree++;
		}

		void cut(fibo_heap_node<T>* child, fibo_heap_node<T>* parent) {
			if (parent->child == child) {
				parent->child = child->right;
			}
			if (parent->child == child) {
				parent->child = nullptr;
			}
			child->parent = nullptr;
			child->mark = false;
			auto left = min_root->left;
			left->right = child;
			child->left = left;
			child->right = min_root;
			min_root->left = child;
		}

		void cascade_cut(fibo_heap_node<T>* node) {
			auto parent = node->parent;
			if (parent) {
				if (!parent->mark) {
					parent->mark = true;
				}
				else {
					cut(node, parent);
					cascade_cut(parent);
				}
			}
		}

	public:
		fibo_heap()
			:min_root(nullptr), n(0) {}

		void insert(const T& key) {
			auto node = new fibo_heap_node<T>(key);
			node->left = node;
			node->right = node;
			if (min_root) {
				auto left = min_root->left;
				left->right = node;
				node->left = left;
				node->right = min_root;
				min_root->left = node;
				if (min_root->value > node->value) {
					min_root = node;
				}
			}
			else {
				min_root = node;
			}
			n++;
		}

		T get_minimum() {
			if (!min_root)
				throw std::exception("empty fibo heap");
			return min_root->value;
		}

		bool empty() {
			return min_root == nullptr;
		}

		void merge(fibo_heap& other) {
			if (other.empty()) {
				return;
			}
			if (!min_root) {
				min_root = other->min_root;
			}
			else {
				auto left = min_root->left;
				auto other_left = other.min_root->left;
				left->right = other.min_root;
				other.min_root->left = left;
				min_root->left = other_left;
				other_left->right = min_root;
				if (min_root->value > other.min_root->value) {
					min_root = other.min_root;
				}
			}
			// release 
			other->min_root = nullptr;
			other->n = 0;
		}

		fibo_heap_node<T>* extract_minimun() {
			auto ret = min_root;
			if (!ret) {
				// child of min_root, move to root list
				auto child = ret->child;
				if (child) {
					auto left = ret->left;
					auto child_left = child->left;
					left->right = child;
					child->left = left;
					ret->left = child_left;
					child_left->right = ret;
				}
				// delete min_root from root list
				ret->left->right = ret->right;
				ret->right->left = ret->left;
				// adjust
				if (n == 1) {
					min_root = nullptr;
				}
				else {
					min_root = ret->left;
					consolidate();
				}
				ret->left = nullptr;
				ret->right = nullptr;
				ret->child = nullptr;
				n--;
			}
			return ret;
		}

		void decrease_value(fibo_heap_node<T>* node, const T& new_value) {
			if (node->value < new_value) {
				throw std::exception("new value should smaller than old value");
			}
			node->value = new_value;
			auto parent = node->parent;
			if (parent && parent->value > node->value) {
				cut(node, parent);
				cascade_cut(parent);
			}
			if (node->value < min_root->value) {
				min_root = node;
			}
		}

		void delete_node(fibo_heap_node<T>* node) {
			decrease_value(node, min_root->value);
			extract_minimun();
		}
	};
}