#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Graph.h"
#include <optional>
#include <map>
#include <list>

using namespace std;
using namespace graph_ns;

struct test {
	int a;
	int b;
	int c;
	test() = default;
	test(int a, int b, int c) {
		this->a = a;
		this->b = b;
		this->c = c;
	}
};

ostream& operator<<(ostream& os, const test& obj) {
	os << obj.a << " " << obj.b << " " << obj.c;
	return os;
}

struct less_x {
	_NODISCARD constexpr bool operator()(const test& first, const test& second) const noexcept {
		if (first.c == second.c) {
			if (first.a == second.a) {
				return first.b < second.b;
			}
			return first.a < second.a;
		}
		return first.c < second.c;
	}
};

int main() {
	directed_graph<int> gr;
	gr.add_node(1);
	gr.add_node(2);
	gr.add_node(3);
	gr.add_node(4);
	gr.add_node(5);
	gr.add_node(6);
	gr.add_node(7);
	gr.add_edge(1, 2);
	gr.add_edge(2, 3);
	gr.add_edge(6, 7);
	gr.add_edge(3, 6);
	gr.add_edge(4, 5);
	gr.add_edge(4, 7);
	directed_graph<int>::path p = gr.find_min_dist(3, 7);
	p.print(cout);
	gr.visualize_path(p, 1);
	return 0;
}