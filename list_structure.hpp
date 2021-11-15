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

		// 值节点
		skip_node(const T& data) 
			:value(data), left(nullptr), right(nullptr)
			, upper(nullptr), lower(nullptr),times(1)
		{}

		// 哨兵节点
		skip_node()
			:left(nullptr),right(nullptr)
			,upper(nullptr),lower(nullptr),times(0)
		{}
	};

	template<typename T>
	struct skip_list {
		unsigned int max_level;
		std::vector<skip_node<T>*> level;
		skip_node<T>* tail_pointer;

		skip_list(unsigned int max_level = 32)
			:max_level(max_level),tail_pointer(nullptr)
		{
			if (max_level < 1) {
				throw std::exception("max_level must >= 1");
			}
			make_new_level();
		}
		
		~skip_list() {
			for (auto& node : level) {
				auto save_node = node;
				while (node) {
					save_node = node->right;
					delete node;
					node = save_node;
				}
			}
		}

		// 创建一个新的空层
		void make_new_level() {
			level.push_back(new skip_node<T>());
			auto tempnode = new skip_node<T>();
			level.back()->right = tempnode;
			tempnode->left = level.back();
			if (level.size() > 1) {
				level.back()->upper = level.at(level.size() - 2);
				level.back()->upper->lower = level.back();
				tempnode->upper = tail_pointer;
				tail_pointer->lower = tempnode;
			}
			tail_pointer = tempnode;
		}

		// 在corner右侧，创建一个新的值节点，其上层节点为*upper_node，产生随机值new_level指示是否继续提升
		void make_level_node(skip_node<T>* corner, const T& data, skip_node<T>** upper_node, bool& new_level) {
			static std::uniform_real_distribution<> dis(0, 1);
			auto seed = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
			auto old_right = corner->right;
			corner->right = new skip_node<T>(data);
			old_right->left = corner->right;
			corner->right->right = old_right;
			corner->right->left = corner;
			if (*upper_node) {
				corner->right->upper = *upper_node;
				(*upper_node)->lower = corner->right;
			}
			*upper_node = corner->right;
			new_level= dis(seed) < 0.5;
		}

		void insert(const T& data) {
			std::vector<skip_node<T>*> level_corner;
			auto pos = lower_bound(data, level_corner);
			if (pos->times&&pos->value == data) {
				pos->times++;
			}
			else {
				skip_node<T>* upper_node = nullptr;
				bool new_level = false;
				// corner列表是自底向上的，提升是自顶向下的，需要反转一次
				std::reverse(level_corner.begin(), level_corner.end());
				for (auto& corner : level_corner) {
					make_level_node(corner, data, &upper_node, new_level);
					if (!new_level)
						break;
				}
				// 从此层开始是全新的层
				while (new_level && level.size() < max_level) {
					make_new_level();
					make_level_node(level.back(), data, &upper_node, new_level);
				}
			}
		}

		/*
		* 不存在返回false
		* 否则删除并返回 true
		*/
		bool remove(const T& data) {
			auto node = contain(data);
			if (node && node->times > 1) {
				node->times--;
				return true;
			}
			else if (node) {
				while (node) {
					auto lower_node = node->lower;
					node->left->right = node->right;
					node->right->left = node->left;
					delete node;
					node = lower_node;
				}
				// 检查是否有多余的空层
				if (level.size() > 1) {
					// 注意这里不能用迭代器哦，因为涉及到删除操作，迭代器会失效的
					for (size_t i = level.size()-1; i != 0 && level.at(i)->right->times == 0; --i) {
						tail_pointer = level.at(i)->right->upper;
						delete level.at(i)->right;
						delete level.at(i);
						level.pop_back();
					}
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
		* 同时返回查询时自底到上每一层转角值
		* 不存在则返回最底层的哨兵
		*/
		skip_node<T>* lower_bound(const T& data, std::vector<skip_node<T>*>& level_corner) {
			skip_node<T>* current_node = level.back();
			while (current_node) {
				// 每一次循环开始时current_node一定位于不可能满足大于等于data的位置
				current_node = current_node->right;
				if (current_node->times) {
					//循环向右找到第一个大于等于的情况
					while (current_node->times && current_node->value < data) {
						current_node = current_node->right;
					}
					//存在等于
					if (current_node->times && current_node->value == data) {
						while (current_node->upper) {
							current_node = current_node->upper;
						}
						return current_node;
					}
					else {//大于或不存在
						//存在上层，继续查询
						if (current_node->upper) {
							level_corner.push_back(current_node->left);
							current_node = current_node->left->upper;
						}
						else {
							//不存在上层了，确实没有该值，直接返回
							level_corner.push_back(current_node->left);
							return current_node;
						}
					}
				}
				else {
					// 该层到哨兵了，该层不存在大于等于
					level_corner.push_back(current_node->left);
					// 可以向上
					if (current_node->upper)
						current_node = current_node->left->upper;
					else
						return current_node;
				}
			}
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& o, const skip_list<T>& list) {
		for (auto& level : list.level) {
			o << "^ ";
			auto node = level->right;
			while (node) {
				if (node->times) {
					o << " <-> " << node->value;
				}
				else {
					o << " <-> $";
				}
				node = node->right;
			}
			o << std::endl;
		}
		return o;
	}
}