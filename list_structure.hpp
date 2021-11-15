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

		// ֵ�ڵ�
		skip_node(const T& data) 
			:value(data), left(nullptr), right(nullptr)
			, upper(nullptr), lower(nullptr),times(1)
		{}

		// �ڱ��ڵ�
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

		// ����һ���µĿղ�
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

		// ��corner�Ҳ࣬����һ���µ�ֵ�ڵ㣬���ϲ�ڵ�Ϊ*upper_node���������ֵnew_levelָʾ�Ƿ��������
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
				// corner�б����Ե����ϵģ��������Զ����µģ���Ҫ��תһ��
				std::reverse(level_corner.begin(), level_corner.end());
				for (auto& corner : level_corner) {
					make_level_node(corner, data, &upper_node, new_level);
					if (!new_level)
						break;
				}
				// �Ӵ˲㿪ʼ��ȫ�µĲ�
				while (new_level && level.size() < max_level) {
					make_new_level();
					make_level_node(level.back(), data, &upper_node, new_level);
				}
			}
		}

		/*
		* �����ڷ���false
		* ����ɾ�������� true
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
				// ����Ƿ��ж���Ŀղ�
				if (level.size() > 1) {
					// ע�����ﲻ���õ�����Ŷ����Ϊ�漰��ɾ����������������ʧЧ��
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
		* ���ڷ��ض�Ӧֵ
		* ���򷵻�nullptr
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
		* ���ص�һ�����ڵ���data��ֵ
		* �������򷵻���ײ���ڱ�
		*/
		skip_node<T>* lower_bound(const T& data) {
			std::vector<skip_node<T>*> level_corner;
			return lower_bound(data, level_corner);
		}

		/*
		* ���ص�һ�����ڵ���data��ֵ
		* ͬʱ���ز�ѯʱ�Ե׵���ÿһ��ת��ֵ
		* �������򷵻���ײ���ڱ�
		*/
		skip_node<T>* lower_bound(const T& data, std::vector<skip_node<T>*>& level_corner) {
			skip_node<T>* current_node = level.back();
			while (current_node) {
				// ÿһ��ѭ����ʼʱcurrent_nodeһ��λ�ڲ�����������ڵ���data��λ��
				current_node = current_node->right;
				if (current_node->times) {
					//ѭ�������ҵ���һ�����ڵ��ڵ����
					while (current_node->times && current_node->value < data) {
						current_node = current_node->right;
					}
					//���ڵ���
					if (current_node->times && current_node->value == data) {
						while (current_node->upper) {
							current_node = current_node->upper;
						}
						return current_node;
					}
					else {//���ڻ򲻴���
						//�����ϲ㣬������ѯ
						if (current_node->upper) {
							level_corner.push_back(current_node->left);
							current_node = current_node->left->upper;
						}
						else {
							//�������ϲ��ˣ�ȷʵû�и�ֵ��ֱ�ӷ���
							level_corner.push_back(current_node->left);
							return current_node;
						}
					}
				}
				else {
					// �ò㵽�ڱ��ˣ��ò㲻���ڴ��ڵ���
					level_corner.push_back(current_node->left);
					// ��������
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