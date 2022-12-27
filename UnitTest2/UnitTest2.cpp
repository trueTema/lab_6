#include "pch.h"
#include "CppUnitTest.h"
#include "..\lab_6\Graph.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace graph_ns;
using namespace std;

namespace UnitTest2
{
	TEST_CLASS(UnitTest2)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			directed_graph<int> d_gr;
			ifstream ifs("C:\\Users\\Артём\\source\\repos\\lab_6\\lab_6\\preset2.txt");
			d_gr.read_fstream(ifs, true);
			d_gr.add_edge(1, 5, 1);
			d_gr.add_edge(1, 6, 1);
			d_gr.add_edge(1, 7, 1);
			d_gr.remove_edge(1, 3);
			d_gr.remove_node(4);
			set<graph<int, true>::edge, graph<int, true>::less_x> ans;
			ans.emplace(graph<int, true>::edge(2, 1, 5));
			ans.emplace(graph<int, true>::edge(3, 2, 2));
			ans.emplace(graph<int, true>::edge(1, 4, 1));
			ans.emplace(graph<int, true>::edge(2, 7, 2));
			ans.emplace(graph<int, true>::edge(1, 5, 1));
			ans.emplace(graph<int, true>::edge(1, 6, 1));
			ans.emplace(graph<int, true>::edge(1, 7, 1));
			ans.emplace(graph<int, true>::edge(3, 6, 3));
			ans.emplace(graph<int, true>::edge(6, 7, 1));
			set<graph<int, true>::edge, graph<int, true>::less_x> st;
			st = d_gr.get_edges();
			Assert::IsTrue(ans == st);
		}
	};
}
