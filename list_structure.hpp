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
		double threshold;
		int max_level;
		std::vector<skip_node<T>> level;

		skip_list(double threshold = 0.5f, int max_level = 32)
			:threshold(threshold), max_level(max_level)
		{
			level.push_back(new skip_node<T>());
		}

		void insert(const T& data) {
			static std::uniform_real_distribution<> dis(0, 1);
			auto seed = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
			auto node = first_le(data);
			if (node && node->times > 0 && node->data == data) {
				node->times++;
			}
			else {

			}
		}

		bool remove(const T& data) {

		}

		skip_node<T>* contain(const T& data) {

		}

		skip_node<T>* first_le(const T& data) {

		}
	};
}