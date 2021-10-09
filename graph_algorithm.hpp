#pragma once
#include<vector>
#include<queue>
#include<unordered_set>
#include<iostream>
#include<stack>
namespace lyc_algorithm {

	std::pair<std::size_t, std::size_t> operator+(const std::pair<std::size_t, std::size_t>& a, const std::pair<std::size_t, std::size_t>& b) {
		std::pair<std::size_t, std::size_t> ret = a;
		ret.first += b.first;
		ret.second += b.second;
		return ret;
	}

	template<typename T>
	struct posHasher {
		std::size_t operator()(const std::pair<T, T>& value) const {
			return std::hash<T>()(value.first) ^ std::hash<T>()(value.second);
		}
	};

	void bfs(const std::vector<std::vector<int>>& input, std::pair<std::size_t, std::size_t> start) {
		std::queue<std::pair<std::size_t, std::size_t>> bfsQueue;
		std::unordered_set<std::pair<std::size_t, std::size_t>, posHasher<std::size_t>> flag;
		flag.insert(start);
		bfsQueue.push(start);
		std::vector<std::pair<std::size_t, std::size_t>> pace = { {0,1},{1,0} }; // 模拟二维地图上只允许向右向下运动
		while (!bfsQueue.empty()) { //仍有节点可访问
			auto point = bfsQueue.front();
			std::cout << input[point.first][point.second] << std::endl;
			bfsQueue.pop();
			for (auto& p : pace) { //尝试扩展邻接节点
				auto nextPoint = point + p;
				if (nextPoint.first < input.size() && nextPoint.second < input[0].size() // 地图边界
					&& flag.find(nextPoint) == flag.end()) { //未被访问
					flag.insert(nextPoint);
					bfsQueue.push(nextPoint);
				}
			}
		}
	}


	std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b) {
		std::pair<int, int> ret = a;
		ret.first += b.first;
		ret.second += b.second;
		return ret;
	}

	void dfs(const std::vector<std::vector<int>>& input, std::pair<int, int> currentPos, std::pair<int, int > targetPos) {
		std::stack<std::pair<int, int>> pathStack;
		std::unordered_set<std::pair<int, int>, posHasher<int>> flag;
		flag.insert(currentPos);
		pathStack.push(currentPos);
		std::vector<std::pair<int, int>> pace = { {0,1},{-1,0},{0,-1},{1,0} };
		bool find = false;
		bool newPos;
		while (!pathStack.empty()) {
			auto pos = pathStack.top();
			//
			newPos = false;
			for (auto& p : pace) {
				auto nextPoint = pos + p;
				if (nextPoint == targetPos) {
					pathStack.push(nextPoint);
					find = true;
					break;
				}
				else if (nextPoint.first >= 0 && nextPoint.first < input.size()
					&& nextPoint.second >= 0 && nextPoint.second < input[0].size()
					&& flag.find(nextPoint) == flag.end() && input[nextPoint.first][nextPoint.second]) {
					pathStack.push(nextPoint);
					flag.insert(nextPoint);
					newPos = true;
					break;
				}
			}
			if (find) {
				break;
			}
			else if (!newPos) {
				pathStack.pop();
			}
		}
		if (find) {
			// 输出路径
			std::stack<std::pair<int, int>> result;
			while (!pathStack.empty()) {
				result.push(pathStack.top());
				pathStack.pop();
			}
			while (!result.empty()) {
				std::cout << result.top().first << "," << result.top().second << std::endl;
				result.pop();
			}
		}
		else {
			std::cout << "cant move to target" << std::endl;
		}
	}

	bool topoDfsVisit(const std::vector<std::vector<int>>& adj, std::stack<int>& output
		, std::unordered_set<int>& visited, std::unordered_set<int>& ancient, int current) {
		visited.insert(current);
		ancient.insert(current);//记录祖先
		for (auto& t : adj[current]) {
			if (ancient.find(t) != ancient.end()) {
				return false;
			}
			else if (visited.find(t) == visited.end()) {
				if (!topoDfsVisit(adj, output, visited, ancient, t)) {
					return false;
				}
			}
		}
		output.push(current);
		ancient.erase(current);//递归返回，从祖先列表中删除自己
		return true;
	}

	bool topoSortDfs(const std::vector<std::vector<int>>& adj, std::stack<int>& output) {
		std::unordered_set<int> visited;
		std::unordered_set<int> ancient;
		for (int i = 0; i != adj.size(); ++i) {
			if (visited.find(i) == visited.end()) {
				if (!topoDfsVisit(adj, output, visited, ancient, i)) {
					return false;
				}
			}
		}
		return true;
	}

	void visitDFS(const std::vector<std::vector<int>>& adj
		, std::vector<int>& finishOrder, std::unordered_set<int>& visited, int current) {
		for (auto& t : adj[current]) {
			if (visited.find(t) == visited.end()) {
				visited.insert(t);
				visitDFS(adj, finishOrder, visited, t);
			}
		}
		std::cout << "push_back << " << current << std::endl;
		finishOrder.push_back(current);
	}

	void finishDFS(const std::vector<std::vector<int>>& adj, std::vector<int>& finishOrder) {
		std::unordered_set<int> visited;
		for (int i = 0; i != adj.size(); ++i) {
			if (visited.find(i) == visited.end()) {
				visited.insert(i);
				visitDFS(adj, finishOrder, visited, i);
			}
		}
	}

	std::vector<std::vector<int>> reverseGraph(const std::vector<std::vector<int>>& adj) {
		std::vector<std::vector<int>> rAdj(adj.size());
		for (int i = 0; i != adj.size(); ++i) {
			for (auto& t : adj[i]) {
				rAdj[t].push_back(i);
			}
		}
		return rAdj;
	}

	void visitSCC(const std::vector<std::vector<int>>& adj
		, std::vector<std::vector<int>>& sccGraph, std::unordered_set<int>& visited, int current) {

		for (auto& t : adj[current]) {
			if (visited.find(t) == visited.end()) {
				visited.insert(t);
				sccGraph.back().push_back(t);
				visitSCC(adj, sccGraph, visited, t);
			}
		}
	}

	void getSCC(const std::vector<std::vector<int>>& adj
		, std::vector<std::vector<int>>& sccGraph, std::vector<int> finishOrder) {
		std::unordered_set<int> visited;
		for (auto& t : finishOrder) {
			if (visited.find(t) == visited.end()) {
				sccGraph.push_back(std::vector<int>());
				sccGraph.back().push_back(t);
				visited.insert(t);
				visitSCC(adj, sccGraph, visited, t);
			}
		}
	}

	void sccDFS(const std::vector<std::vector<int>>& adj, std::vector<std::vector<int>>& sccGraph) {
		std::vector<int> finishOrder;
		finishDFS(adj, finishOrder);
		for (auto& t : finishOrder) {
			std::cout << t << " ";
		}
		std::cout << std::endl << "========================" << std::endl;
		std::reverse(finishOrder.begin(), finishOrder.end());
		for (auto& t : finishOrder) {
			std::cout << t << " ";
		}
		std::cout << std::endl << "========================" << std::endl;
		auto rAdj = reverseGraph(adj);
		getSCC(rAdj, sccGraph, finishOrder);
	}
}