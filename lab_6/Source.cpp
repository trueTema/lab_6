#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "Graph.h"
#include <optional>
#include <map>
#include <list>
#include "menu.h"
#include "tests.h"

using namespace std;
using namespace graph_ns;



int main() {
	setlocale(LC_ALL, "Russian");
	base_test_dir();
	base_test_undir();
	algo_test_dijkstra_dir();
	algo_test_dijkstra_undir();
	algo_test_skeleton();
	algo_test_connectivity();
	algo_test_strong_connectivity();
	main_listener();
	/*directed_graph<double> gr;
	graph<double, true>* gra = new directed_graph<double>;
	gra = &gr;*/
	//ifstream ifs("test.txt");
	//gr.read_fstream(ifs, true);
	//ifs.close();
	//gr.visualize();
	
	//gr.add_node(1);
	//gr.add_node(2);
	//gr.add_node(3);
	//gr.add_node(4);
	//gr.add_node(5);
	//gr.add_node(6);
	//gr.add_node(7);
	//gr.add_edge(1, 2);
	//gr.add_edge(1, 3);
	//gr.add_edge(1, 4);
	//gr.add_edge(2, 7);
	//gr.add_edge(3, 6);
	//gr.add_edge(4, 6);
	//gr.add_edge(6, 7);
	//ofstream ofs("test.txt");
	//gr.write_fstream(ofs);
	//ofs.close();
	//undirected_graph<int>::path p = gr.find_min_dist(1, 2);
	//gr.visualize(p, 1);
	return 0;
}