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
#include"comparison_sort.hpp"
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

	// spanning tree
	minimum_spanning_tree* myTree = new kruskal;
	std::vector<edge> graphEdges;
	graphEdges.push_back(edge(0, 1, 1));
	graphEdges.push_back(edge(1, 4, 1));
	graphEdges.push_back(edge(1, 3, 2));
	graphEdges.push_back(edge(3, 5, 1));
	graphEdges.push_back(edge(2, 3, 1));
	graphEdges.push_back(edge(0, 2, 2));
	graphEdges.push_back(edge(2, 4, 2));
	graphEdges.push_back(edge(4, 5, 3));
	std::cout << myTree->get_minimum_spanning_tree(graphEdges) << std::endl;
	delete myTree;
	myTree = new prim;
	std::cout << myTree->get_minimum_spanning_tree(graphEdges) << std::endl;

	// shortest path
	graph_adjacent_map graph_map;
	graph_map.nodes[0] = { {1,1} ,{2,2},{3,3} };
	graph_map.nodes[1] = { {2,1} ,{3,4},{4,4} };
	graph_map.nodes[2] = { {3,1} ,{4,2},{4,13} };
	graph_map.nodes[3] = { {4,1} ,{2,2},{1,3} };
	graph_map.nodes[4] = { {0,1} ,{2,2},{3,3} };
	try
	{
		auto result = dijkstra(graph_map, 4);
		for (auto& t : result) {
			std::cout << "distance to " << t.first << " = " << t.second << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	graph_adjacent graph;
	graph.nodes.push_back(node_adjacent(0, { {1,1} ,{2,2},{3,3} }));
	graph.nodes.push_back(node_adjacent(1, { {2,1} ,{3,4},{4,4} }));
	graph.nodes.push_back(node_adjacent(2, { {3,1} ,{4,2},{4,13} }));
	graph.nodes.push_back(node_adjacent(3, { {4,1} ,{2,2},{1,3} }));
	graph.nodes.push_back(node_adjacent(4, { {0,1} ,{2,2},{3,3} }));
	try
	{
		auto result = bellman_ford_shortest_path(graph, 4);
		for (auto& t : result) {
			std::cout << "distance to " << t.first << " = " << t.second << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	// floyd
	{
		std::vector<std::vector<int>> graph = { {0,1,4,3},{1,0,4,5},{-1,2,0,3},{2,4,3,0} };
		std::vector<std::vector<int>> dst = graph;
		for (int k = 0; k != graph.size(); ++k) {
			for (int i = 0; i != graph.size(); ++i) {
				if (i == k)
					continue;
				for (int j = 0; j != graph.size(); ++j) {
					if (i == j || k == j)
						continue;
					dst[i][j] = std::min(dst[i][j], dst[i][k] + dst[k][j]);
				}
			}
		}
		for (int i = 0; i != dst.size(); ++i) {
			for (int j = 0; j != dst.size(); ++j) {
				std::cout << dst[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	// jonhson
	{
		graph_adjacent_map graph_map;
		graph_map.nodes[0] = { {1,1} ,{2,2},{3,3} };
		graph_map.nodes[1] = { {2,1} ,{3,4},{4,4} };
		graph_map.nodes[2] = { {3,1} ,{4,2},{0,-2} };
		graph_map.nodes[3] = { {4,1} ,{2,2},{1,3} };
		graph_map.nodes[4] = { {0,1} ,{2,2},{3,3} };
		auto result = johnson(graph_map);
		for (auto& node : result) {
			std::cout << "from node: " << node.first << std::endl;
			for (auto& end : node.second) {
				std::cout << "    to node: " << end.first << " -> " << end.second << std::endl;
			}
		}
	}
}
#endif // GRAPH_EXAMPLE

#define ORDER_STATISTIC
#ifdef ORDER_STATISTIC
#include"order_statistic.hpp"

#endif // ORDER_STATISTIC

#define TREE_EXAMPLE
#ifdef TREE_EXAMPLE
#include"tree_structure.hpp"
template<typename T>
void rbtree_delete_test(lyc_algorithm::redblack_tree_node<T>**root, const T&data) {
	using namespace lyc_algorithm;
	if (redblack_tree_delete<T>(root, data)) {
		rbtree_print(root);
	}
	else {
		std::cout << data << " not exist in rbtree" << std::endl;
	}
}

template<typename T>
void rbtree_insert_test(lyc_algorithm::redblack_tree_node<T>** root, const T& data) {
	using namespace lyc_algorithm;
	redblack_tree_insert<T>(root, data);
	rbtree_print(root);
}

void tree_examples() {
	std::cout << "======tree_examples======" << std::endl;
	using namespace lyc_algorithm;
	redblack_tree_node<int>* root = nullptr;
	rbtree_insert_test(&root, 10);
	rbtree_insert_test(&root, 11);
	rbtree_insert_test(&root, 1);
	rbtree_insert_test(&root, 5);
	rbtree_insert_test(&root, -3);
	rbtree_insert_test(&root, 0);
	rbtree_insert_test(&root, 17);
	rbtree_insert_test(&root, 14);
	rbtree_insert_test(&root, 13);
	rbtree_insert_test(&root, 12);

	//std::cout << "====rbtree_delete====" << std::endl;
	rbtree_delete_test(&root, 233);
	rbtree_delete_test(&root, -3);
	rbtree_delete_test(&root, 10);
	rbtree_delete_test(&root, 11);
	rbtree_delete_test(&root, 1);
	rbtree_delete_test(&root, 5);
	rbtree_delete_test(&root, -3);
	rbtree_delete_test(&root, 0);
	rbtree_delete_test(&root, 17);
	rbtree_delete_test(&root, 14);
	rbtree_delete_test(&root, 13);
	rbtree_delete_test(&root, 12);
}
#endif // TREE_EXAMPLE


int main() {

#ifdef SORT_EXAMPLE
	sort_example();
#endif

#ifdef GRAPH_EXAMPLE
	graph_example();
#endif

#ifdef TREE_EXAMPLE
	tree_examples();
#endif // TREE_EXAMPLE

	return 0;
}