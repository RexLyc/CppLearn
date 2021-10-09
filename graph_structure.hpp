#pragma once
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<queue>
#include<unordered_set>
namespace lyc_algorithm {
	typedef unsigned int node_id;
	typedef int edge_weight;
	struct edge {
		node_id begin;
		node_id end;
		int weight;

		edge(node_id begin, node_id end, int weight) {
			this->begin = begin;
			this->end = end;
			this->weight = weight;
		}

		bool operator<(const edge& b) {
			return this->weight < b.weight;
		}
	};

	struct edge_adjacent {
		node_id end;
		edge_weight weight;

		edge_adjacent(node_id end, edge_weight weight) {
			this->end = end;
			this->weight = weight;
		}

		bool operator<(const edge_adjacent& b) const {
			return this->weight < b.weight;
		}
	};

	typedef edge_adjacent distance_single_source;

	struct node_adjacent {
		node_id begin;
		std::vector<edge_adjacent> edges;
		node_adjacent(node_id begin, std::vector<edge_adjacent>&& edges) {
			this->begin = begin;
			this->edges = edges;
		}
	};

	struct graph_adjacent {
		std::vector<node_adjacent> nodes;
	};

	struct graph_adjacent_map {
		std::unordered_map<node_id, std::vector<edge_adjacent>> nodes;
	};

	template<typename T>
	struct min_heap {
		bool operator()(const T& a, const T& b) {
			if (!(a < b) && !(b < a)) // strict weak order
				return false;
			else
				return !(a < b);
		}
	};

	class union_set {
	private:
		std::unordered_map<node_id, node_id> set_;
	public:
		void set_parent(node_id child, node_id parent) {
			set_[child] = parent;
		}

		node_id get_parent(node_id child) {
			std::vector<node_id> path;
			node_id current = child;
			while (set_[current] != current) {
				path.push_back(current);
				current = set_[current];
			}
			for (auto& t : path) {
				set_[t] = current;
			}
			return current;
		}

		void make_union(node_id a, node_id b) {
			set_parent(get_parent(a), get_parent(b));
		}

		bool union_check(node_id a, node_id b) {
			return get_parent(a) == get_parent(b);
		}

		std::size_t size() {
			return set_.size();
		}
	};

	

	struct distance_to_node {
		int weight;
		node_id node;

		distance_to_node(int weight, node_id node) {
			this->weight = weight;
			this->node = node;
		}

		bool operator<(const distance_to_node& a) const {
			return this->weight < a.weight;
		}
	};

}