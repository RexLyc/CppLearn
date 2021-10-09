#pragma once
#include<vector>
#include<xutility>
namespace lyc_algorithm {
	void heapify(std::vector<int>& input, std::size_t i, std::size_t count) {
		while (i < count)
		{
			if (i * 2 + 2 < count && input[i * 2 + 2] == std::max(std::max(input[i * 2 + 2], input[i * 2 + 1]), input[i])) { // right child bigger
				std::swap(input[i * 2 + 2], input[i]);
				i = i * 2 + 2;
			}
			else if (i * 2 + 1 < count && input[i * 2 + 1] == std::max(input[i * 2 + 1], input[i])) { // left child bigger
				std::swap(input[i * 2 + 1], input[i]);
				i = i * 2 + 1;
			}
			else {
				break;
			}
		}
	}

	void heap_sort(std::vector<int>& input) {
		for (std::size_t i = (input.size() - 1) / 2; i > 0; i--)
			heapify(input, i, input.size());
		heapify(input, 0, input.size());
		for (std::size_t i = 1; i != input.size(); ++i) {
			std::swap(input[0], input[input.size() - i]);
			heapify(input, 0, input.size() - i);
		}
	}
}