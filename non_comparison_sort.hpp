#pragma once
#include<vector>
#include<algorithm>
namespace lyc_algorithm {
	std::vector<int> count_sort(std::vector<int>& input) {
		std::vector<int> ret(input.size(), 0);
		auto minmaxElement = std::minmax_element(input.begin(), input.end());
		std::vector<unsigned int> count(*minmaxElement.second - *minmaxElement.first + 1, 0);
		for (auto& t : input) {
			count[t - *minmaxElement.first]++;
		}
		for (std::size_t i = 1; i != count.size(); ++i) {
			count[i] += count[i - 1];
		}
		for (auto& t : input) {
			//std::cout << count[t - *minmaxElement.first] - 1 << std::endl;
			ret[count[t - *minmaxElement.first] - 1] = t;
			count[t - *minmaxElement.first]--;
		}
		return ret;
	}
}