#pragma once
#include<chrono>
#include<random>
#include<vector>
namespace lyc_algorithm {

	template<typename T>
	struct skip_node {
		skip_node* left, * right;
		skip_node* upper, * lower;
		T value;
		std::size_t times;

		skip_node(const T& data) 
			:value(data), left(nullptr), right(nullptr)
			, upper(nullptr), lower(nullptr),times(1)
		{}

		skip_node()
			:left(nullptr),right(nullptr)
			,upper(nullptr),lower(nullptr),times(0)
		{}
	};

	template<typename T>
	struct skip_list {
		unsigned int max_level;
		std::vector<skip_node<T>*> level;

		skip_list(unsigned int max_level = 32)
			:max_level(max_level)
		{
			if (max_level < 1) {
				throw std::exception("max_level must >= 1");
			}
			make_new_level();
		}

		void make_new_level() {
			level.push_back(new skip_node<T>());
			auto tempnode = new skip_node<T>();
			level.back()->right = tempnode;
			tempnode->left = level.back();
		}

		void make_level_node(skip_node<T>* corner, const T& data, skip_node<T>** lower_node, bool& new_level) {
			static std::uniform_real_distribution<> dis(0, 1);
			auto seed = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
			auto old_right = corner->right;
			corner->right = new skip_node<T>(data);
			old_right->left = corner->right;
			corner->right->right = old_right;
			corner->right->left = corner;
			if (*lower_node) {
				corner->right->lower = *lower_node;
				(*lower_node)->upper = corner->right;
			}
			*lower_node = corner->right;
			new_level= dis(seed) < 0.5;
		}

		void insert(const T& data) {
			std::vector<skip_node<T>*> level_corner;
			auto pos = lower_bound(data, level_corner);
			if (pos->times&&pos->value == data) {
				pos->times++;
			}
			else {
				skip_node<T>* lower_node = nullptr;
				bool new_level = false;
				for (auto& corner : level_corner) {
					make_level_node(corner, data, &lower_node, new_level);
					if (!new_level)
						break;
				}
				while (new_level && level.size() < max_level) {
					make_new_level();
					make_level_node(level.back(), data, &lower_node, new_level);
				}
			}
		}

		/*
		* 不存在返回false
		* 否则删除并返回 true
		*/
		bool remove(const T& data) {
			auto node = contain(data);
			if (node) {
				while (node) {
					auto lower_node = node->lower;
					node->left->right = node->right;
					node->right->left = node->left;
					delete node;
					node = lower_node;
				}
				return true;
			}
			else {
				return false;
			}
		}

		/*
		* 存在返回对应值
		* 否则返回nullptr
		*/
		skip_node<T>* contain(const T& data) {
			auto node = lower_bound(data);
			if (node->times && node->value == data) {
				return node;
			}
			else {
				return nullptr;
			}
		}

		/*
		* 返回第一个大于等于data的值
		* 不存在则返回最底层的哨兵
		*/
		skip_node<T>* lower_bound(const T& data) {
			std::vector<skip_node<T>*> level_corner;
			return lower_bound(data, level_corner);
		}

		/*
		* 返回第一个大于等于data的值
		* 同时返回查询时每一层转角值
		* 不存在则返回最底层的哨兵
		*/
		skip_node<T>* lower_bound(const T& data, std::vector<skip_node<T>*>& level_corner) {
			skip_node<T>* current_node = level.back();
			while (current_node) {
				current_node = current_node->right;
				if (current_node->times) {
					while (current_node->times && current_node->value < data) {
						current_node = current_node->right;
					}
					if (current_node->times && current_node->value == data) {
						while (current_node->upper) {
							current_node = current_node->upper;
						}
						return current_node;
					}
					else {
						if (current_node->upper) {
							level_corner.push_back(current_node->left);
							current_node = current_node->left->upper;
						}
						else {
							level_corner.push_back(current_node->left);
							return current_node;
						}
					}
				}
				else {
					level_corner.push_back(current_node->left);
					if (current_node->upper)
						current_node = current_node->upper;
					else
						return current_node;
				}
			}
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& o, const skip_list<T>& list) {
		for (auto& level : list.level) {
			o << "^ <-> ";
			while (level) {
				if (level->times) {
					o << " <-> " << level->value;
				}
				else {
					o << "<-> $";
				}
			}
			o << std::endl;
		}
		return o;
	}
}