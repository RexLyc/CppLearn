#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<numeric>
#include<queue>
#include<unordered_set>
#include<type_traits>
#include<stack>
//using namespace std;

struct sizeTest {
	char a[20];
	char b[20];
};

void getSize(sizeTest &t) {
	std::cout << sizeof(t.a) << std::endl;
}

#define SORT_EXAMPLE
#ifdef SORT_EXAMPLE
#include"non_comparison_sort.hpp"
#include"heap_sort.hpp"
void sort_example() {
	using namespace lyc_algorithm;
	std::vector<int> input = { 0,1,3,2,4 ,1,0,1,3,2,4 ,-1 };
	auto result = count_sort(input);
	for (auto& t : result) {
		std::cout << t << " ";
	}
	std::cout << std::endl;
	heap_sort(input);
	for (auto& t : input) {
		std::cout << t << " ";
	}
}
#endif // SORT_EXAMPLE

#define GRAPH_EXAMPLE
#ifdef GRAPH_EXAMPLE
#include"graph_algorithm.hpp"
void graph_example() {
	using namespace lyc_algorithm;
	std::vector<std::vector<int>> input = {
		{1,2,4,7,11},
		{3,5,8,12,16},
		{6,9,13,17,20},
		{10,14,18,21,23},
		{15,19,22,24,25}
	};
	
	bfs(input, { 0,0 });
	std::vector<std::vector<int>> maze = {
		{1,0,1,1,1},
		{1,1,1,0,1},
		{0,0,1,0,0},
		{1,1,1,1,1},
		{1,0,1,0,1}
	};
	dfs(maze, { 0,0 }, { maze.size() - 1,maze.size() - 1 });
	std::vector<std::vector<int>> adj = {
		{},{2,7},{3},{4,8},{5},{},{7},{3},{5}
	};
	std::stack<int> output;
	if (topoSortDfs(adj, output)) {
		while (!output.empty()) {
			std::cout << output.top() << " ";
			output.pop();
		}
	}
	else {
		std::cout << "no topo, there has circle" << std::endl;
	}
	adj = {
		{1}
		,{2}
		,{0,3}
		,{4,5}
		,{5}
		,{4}
	};
	std::vector<std::vector<int>> sccGraph;
	sccDFS(adj, sccGraph);
	for (auto& t : sccGraph) {
		for (auto& tt : t) {
			std::cout << tt << " ";
		}
		std::cout << std::endl;
	}
}
#endif // GRAPH_EXAMPLE


int main() {

#ifdef SORT_EXAMPLE
	sort_example();
#endif

#ifdef GRAPH_EXAMPLE
	graph_example();
#endif


	return 0;
}