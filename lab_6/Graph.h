#pragma once
#include <string>
#include <list>
#include <optional>
#include <set>
#include <unordered_map>
#include "exception.h"
#include <fstream>



namespace graph_ns {
	enum _color {
		Black, Red, Blue
	};

	template<typename NodeInfo>
	struct node {
	private:
		size_t id;
		NodeInfo info;
		short int color = 0;
	public:
		node() = default;
		node(size_t id) {
			this->id = id;
		}
		node(const node& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
		}
		node(node&& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			other.id = 0;
			other.info = NodeInfo();
			other.color = Black;
		}
		node& operator=(const node& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			return *this;
		}
		node& operator=(node&& other) {
			this->id = other.id;
			this->color = other.color;
			this->info = other.info;
			other.id = 0;
			other.info = NodeInfo();
			other.color = Black;
			return *this;
		}
		node(size_t id, NodeInfo info) : node(id) {
			this->info = info;
		}
		~node() = default;
		size_t get_id() const noexcept {
			return id;
		}
		NodeInfo get_info() const noexcept {
			return info;
		}
		short int get_color() const noexcept {
			return color;
		}
		void set_color(size_t color) noexcept {
			this->color = color;
		}
	};

	template<typename EdgeInfo, typename WType>
	struct edge {
	private:
		size_t from = 0;
		WType weight = WType();
		EdgeInfo info = EdgeInfo();
		size_t color = 0;
		size_t to = 0;
	public:
		edge() = delete;
		edge(size_t id_from, size_t id_to, WType weight) {
			this->from = id_from;
			this->to = id_to;
			this->weight = weight;
		}
		edge(const edge& other) : info(other.info) {
			this->from = other.from;
			this->to = other.to;
			this->weight = other.weight;
			this->color = other.color;
		}
		edge(size_t id_from, size_t id_to, WType weight, EdgeInfo info, size_t color = 0) : edge(id_from, id_to, weight) {
			this->info = info;
			this->color = color;
		}
		~edge() = default;
		size_t get_from() const noexcept {
			return this->from;
		}
		size_t get_to() const noexcept {
			return this->to;
		}
		EdgeInfo get_info() const noexcept {
			return this->info;
		}
		WType get_weight() const noexcept {
			return this->weight;
		}
		size_t get_color() const noexcept {
			return color;
		}
		void set_color(size_t color) noexcept {
			this->color = color;
		}
		bool operator==(const edge& other) const noexcept {
			if (this->from == other.from && this->to == other.to && this->weight == other.weight && this->info == other.info) {
				return true;
			}
			return false;
		}
		bool operator!=(const edge& other) const noexcept {
			if (this->from == other.from && this->to == other.to && this->weight == other.to && this->info == other.info) {
				return false;
			}
			return true;
		}
	};

	template<typename T>
	struct max_value {
		static T get() noexcept {
			return INT32_MAX / 3;
		}
	};
	template<>
	struct max_value<long long> {
		static long long get() noexcept {
			return INT64_MAX / 3;
		}
	};
	template<>
	struct max_value<short int> {
		static short int get() noexcept {
			return INT8_MAX / 3;
		}
	};
	template<>
	struct max_value<double> {
		static double get() noexcept {
			return INT64_MAX / 3;
		}
	};

	template<typename EdgeInfo, typename WType>
	struct path {
	private:
		WType len;
		size_t count;
		std::list<edge<EdgeInfo, WType>> chain;
	public:
		path() = default;
		WType get_len() const noexcept {
			return len;
		}
		size_t get_count() const noexcept {
			return count;
		}
		const std::list<edge<EdgeInfo, WType>>& get_chain() const noexcept {
			return chain;
		}
		void push_back(const edge<EdgeInfo, WType>& ed) {
			if (chain.size() == 0 || (*chain.end()).get_to() == ed.get_from()) {
				chain.push_back(ed);
				count++;
				len += ed.get_weight();
			}
			else throw SetException(IncorrectPath);
		}
		void push_tobegin(const edge<EdgeInfo, WType>& ed) {
			if (chain.size() == 0 || (*chain.begin()).get_from() == ed.get_to()) {
				chain.insert(chain.begin(), ed);
				count++;
				len += ed.get_weight();
			}
			else throw SetException(IncorrectPath);
		}
		std::ostream& print(std::ostream& os) const noexcept {
			for (typename std::list<edge<EdgeInfo, WType>>::const_iterator it = chain.cbegin(); it != chain.cend(); it++) {
				os << "{ " << (*it).get_from() << ", " << (*it).get_to() << ", " << (*it).get_weight() << " }\n";
			}
			return os;
		}
		path& operator+=(const path& other) {
			if ((*other.chain.begin()).get_from() == (*chain.end()).get_to()) {
				for (typename std::list<edge<EdgeInfo, WType>>::const_iterator it = other.chain.cbegin(); it != other.chain.cend(); it++) {
					this->chain.push_back(*it);
				}
			} else throw SetException(IncorrectPath);
		}
	};

	template<typename WType = int, bool isDirected = false, class NodeInfo = std::string, class EdgeInfo = std::string>
	class graph {
	public:
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;
	protected:
		std::unordered_map<size_t, std::list<edge>> incidences_list;
		std::unordered_map<size_t, node> nodes;
		struct less_x {
			constexpr bool operator()(const edge& first, const edge& second) const noexcept {
				if (first.get_weight() == second.get_weight()) {
					if (first.get_from() == second.get_from()) {
						return first.get_to() < second.get_to();
					}
					return first.get_from() < second.get_from();
				}
				return first.get_weight() < second.get_weight();
			}
		};
		std::set<edge, less_x> edges;
		void print_node(std::ofstream& ofs, const node& nd) const noexcept {
			ofs << nd.get_id();
			if (nd.get_info() != NodeInfo()) {
				ofs << " [label=\"" << nd.get_info() << "\",";
			}
			else {
				ofs << " [";
			}
			if (nd.get_color() == 1) {
				ofs << "color=\"red\"";
			}
			else if (nd.get_color() == 2) {
				ofs << "color=\"green\"";
			}
			else if (nd.get_color() == 3) {
				ofs << "color=\"blue\"";
			}
			ofs << "];\n";
		}

		virtual void print_edge(std::ofstream& ofs, const edge& ed) const noexcept {
			const node& nd = (*nodes.find(ed.get_from())).second;
			ofs << nd.get_id();
			ofs << "->";
			const node& nd2 = (*nodes.find(ed.get_to())).second;
			ofs << nd2.get_id();
			ofs << " [";
			if (ed.get_color() == 1) {
				ofs << "color=\"red\",";
			}
			else if (ed.get_color() == 2) {
				ofs << "color=\"green\",";
			}
			else if (ed.get_color() == 3) {
				ofs << "color=\"blue\",";
			}
			ofs << "label=\"" << ed.get_info() << " {" << ed.get_weight() << "}" << "\"];\n";
		}
		size_t n;
		size_t m;
	public:
		graph() = default;
		size_t n_count() const noexcept {
			return n;
		}
		size_t m_count() const noexcept {
			return m;
		}
		
		path find_min_dist(size_t id_f, size_t id_t) {
			std::unordered_map<size_t, std::optional<WType>> dist;
			std::unordered_map<size_t, const edge*> parents;
			parents.reserve(n);
			dist.reserve(n);
			dist[id_f] = WType();
			for (typename std::unordered_map<size_t, node>::const_iterator it = nodes.cbegin(); it != nodes.cend(); it++) {
				int v = -1;
				for (typename std::unordered_map<size_t, std::optional<WType>>::iterator itd = dist.begin(); itd != dist.end(); itd++) {
					if (nodes[(*itd).first].get_color() == 0) {
						if (v == -1 || (dist[(*itd).first].has_value() && dist[v].has_value() && dist[(*itd).first].value() < dist[v].value())) {
							v = (*itd).first;
						}
						else if (!dist[v].has_value() && dist[(*itd).first].has_value()) {
							v = (*itd).first;
						}
					}
				}
				if (!dist[v].has_value()) break;
				nodes[v].set_color(1);
				for (typename std::list<edge>::const_iterator it = incidences_list[v].cbegin(); it != incidences_list[v].cend(); it++) {
					WType len = (*it).get_weight();
					size_t id_to = (*it).get_to();
					if ((!dist[id_to].has_value()) || dist[id_to].value() > dist[v].value() + len) {
						dist[id_to] = dist[v].value() + len;
						parents[id_to] = &(*it);
					}
				}
			}
			for (typename std::unordered_map<size_t, node>::iterator it = nodes.begin(); it != nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			if (!dist[id_t].has_value()) throw SetException(GraphNotConnective);
			path res;
			for (size_t cur_id = id_t; cur_id != id_f; cur_id = parents[cur_id]->get_from()) {
				res.push_tobegin(*parents[cur_id]);
			}
			return res;
		}
		void add_node(size_t id, NodeInfo inf = NodeInfo()) {
			if (nodes.find(id) == nodes.end()) {
				nodes[id] = node(id, inf);
				n++;
				incidences_list.emplace(id, std::list<edge>());
			}
		}
		void add_edge(size_t id_from, size_t id_to, WType weight = 1, EdgeInfo inf = EdgeInfo()) {
			if (nodes.find(id_from) == nodes.end() || nodes.find(id_to) == nodes.end()) throw SetException(NoSuchElement);
			if (isDirected || edges.find(edge(id_to, id_from, weight, inf)) == edges.end())
				edges.emplace(edge(id_from, id_to, weight, inf));
			if (std::find(incidences_list[id_from].begin(), incidences_list[id_from].end(), edge(id_from, id_to, weight, inf)) == incidences_list[id_from].end())
			{
				if (isDirected || (std::find(incidences_list[id_from].begin(), incidences_list[id_from].end(), edge(id_to, id_from, weight, inf)) == incidences_list[id_from].end()))
				{
					m++;
					incidences_list[id_from].push_back(edge(id_from, id_to, weight, inf));
					if (!isDirected) {
						incidences_list[id_to].push_back(edge(id_to, id_from, weight, inf));
					}
				}
			}
		}
		void remove_node(size_t id) {
			if (nodes.find(id) == nodes.end()) throw SetException(NoSuchElement);
			for (typename std::set<edge>::iterator it = edges.begin(); it != edges.end(); ) {
				if ((*it).get_to() == id) {
					typename std::set<edge>::iterator it_cur = it;
					it++;
					typename std::list<edge>::iterator itx = std::find(incidences_list[(*it_cur).get_from()].begin(), incidences_list[(*it_cur).get_from()].end(), *it_cur);
					if (itx != incidences_list[(*it_cur).get_from()].end())
						incidences_list[(*it_cur).get_from()].erase(itx);
					if (!isDirected) {
						itx = incidences_list[(*it_cur).get_to()].end();
						edge opposite_edge((*it_cur).get_to(), (*it_cur).get_from(), (*it_cur).get_weight(), (*it_cur).get_info());
						itx = std::find(incidences_list[(*it_cur).get_to()].begin(), incidences_list[(*it_cur).get_to()].end(), opposite_edge);
						if (itx != incidences_list[(*it_cur).get_to()].end())
							incidences_list[(*it_cur).get_to()].erase(itx);
					}
					edges.erase(it_cur);
				}
				else if ((*it).get_from() == id) {
					typename std::set<edge>::iterator it_cur = it;
					it++;
					typename std::list<edge>::iterator itx = std::find(incidences_list[(*it_cur).get_from()].begin(), incidences_list[(*it_cur).get_from()].end(), *it_cur);
					if (itx != incidences_list[(*it_cur).get_from()].end())
						incidences_list[id].erase(itx);
					if (!isDirected) {
						itx = incidences_list[(*it_cur).get_to()].end();
						edge opposite_edge((*it_cur).get_to(), (*it_cur).get_from(), (*it_cur).get_weight(), (*it_cur).get_info());
						itx = std::find(incidences_list[(*it_cur).get_to()].begin(), incidences_list[(*it_cur).get_to()].end(), opposite_edge);
						if (itx != incidences_list[(*it_cur).get_to()].end())
							incidences_list[(*it_cur).get_to()].erase(itx);
					}
					edges.erase(it_cur);
				}
				else it++;
			}
			incidences_list.erase(id);
			n--;
			nodes.erase(id);
		}
		void remove_edge(size_t id_from, size_t id_to, std::optional<WType> weight = std::optional<WType>()) {
			for (typename std::list<edge>::iterator it = incidences_list[id_from].begin(); it != incidences_list[id_from].end();) {
				if ((*it).get_from() == id_from && (*it).get_to() == id_to && (!weight.has_value() || (*it).get_weight() == weight.value())) {
					typename std::list<edge>::iterator it_cur = it;
					it++;
					typename std::set<edge>::iterator itx = std::find(edges.begin(), edges.end(), *it_cur);
					if (itx != edges.end()) {
						edges.erase(itx);
						m--;
					}
					incidences_list[id_from].erase(it_cur);
				}
				else it++;
			}
			if (!isDirected) {
				for (typename std::list<edge>::iterator it = incidences_list[id_to].begin(); it != incidences_list[id_to].end();) {
					if ((*it).get_from() == id_to && (*it).get_to() == id_from && (!weight.has_value() || (*it).get_weight() == weight.value())) {
						typename std::list<edge>::iterator it_cur = it;
						it++;
						typename std::set<edge>::iterator itx = std::find(edges.begin(), edges.end(), *it_cur);
						if (itx != edges.end())
							edges.erase(itx);
						incidences_list[id_to].erase(it_cur);
					}
					else it++;
				}
			}
		}
		std::ostream& print(std::ostream& os) const noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = incidences_list.cbegin(); it != incidences_list.cend(); it++) {
				os << (*it).first << " - ";
				for (typename std::list<edge>::const_iterator itl = (*it).second.cbegin(); itl != (*it).second.cend(); itl++) {
					os << "< " << (*itl).get_from() << ", " << (*itl).get_to() << ", " << (*itl).get_weight() << " >";
					if (std::distance(itl, (*it).second.cend()) > 1) os << ", ";
				}
				if ((*it).second.size() == 0) {
					os << "<>";
				}
				os << "\n\n";
			}
			return os;
		}
		std::ostream& print_edges(std::ostream& os) const noexcept {
			os << "\n";
			for (typename std::set<edge>::const_iterator it = edges.cbegin(); it != edges.cend(); it++) {
				os << "< " << (*it).get_from() << " - " << (*it).get_to() << " [" << (*it).get_weight() << "] >";
				os << "\n";
			}
			return os;
		}
		~graph() = default;
	};

	template<typename WType = int, class NodeInfo = std::string, class EdgeInfo = std::string>
	class directed_graph : public graph<WType, true, NodeInfo, EdgeInfo>
	{
	public:
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;
	private:
		using base = graph<WType, true, NodeInfo, EdgeInfo>;

		void csc_order(size_t id, std::vector<size_t>& order) {
			base::nodes[id].set_color(1);
			for (typename std::list<edge>::iterator it = base::incidences_list[id].begin(); it != base::incidences_list[id].end(); it++) {
				if (base::nodes[(*it).get_to()].get_color() == 0) {
					csc_order((*it).get_to(), order);
				}
			}
			order.push_back(id);
		}
		void connect_components(size_t node_id, directed_graph<WType, NodeInfo, EdgeInfo>& trp, directed_graph<WType, NodeInfo, EdgeInfo>& res, size_t clr) {
			trp.nodes[node_id].set_color(clr);
			for (typename std::list<edge>::iterator it = trp.incidences_list[node_id].begin(); it != trp.incidences_list[node_id].end(); it++) {
				if (trp.nodes[(*it).get_to()].get_color() == 0)
				{
					res.add_node((*it).get_to(), trp.nodes[node_id].get_info());
					connect_components((*it).get_to(), trp, res, clr);
					res.add_edge((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				}
				else if (trp.nodes[(*it).get_to()].get_color() == clr) {
					res.add_edge((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				}
			}
		}
	public:
		std::list<directed_graph<WType, NodeInfo, EdgeInfo>> connect_components() {
			std::list<directed_graph<WType, NodeInfo, EdgeInfo>> res;
			std::vector<size_t> order;
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				if ((*it).second.get_color() == 0)
					csc_order((*it).first, order);
			}
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			directed_graph<WType, NodeInfo, EdgeInfo> trp = get_transponated();
			for (int i = order.size() - 1; i >= 0; i--) {
				if (trp.nodes[order[i]].get_color() == 0) {
					directed_graph<WType, NodeInfo, EdgeInfo> cur_gr;
					cur_gr.add_node(base::nodes[order[i]].get_id(), base::nodes[order[i]].get_info());
					connect_components(order[i], trp, cur_gr, i);
					res.push_back(cur_gr);
				}
			}
			return res;
		}
		void visualize() const noexcept {
			std::ofstream ofs("g.dot");
			ofs << "digraph T {\n";
			for (typename std::unordered_map<size_t, node>::const_iterator it = base::nodes.cbegin(); it != base::nodes.cend(); it++) {
				base::print_node(ofs, (*it).second);
			}
			for (typename std::set<edge>::const_iterator it = base::edges.cbegin(); it != base::edges.cend(); it++) {
				base::print_edge(ofs, (*it));
			}
			ofs << "}";
			ofs.close();
			system("cd C:\\Users\\Àðò¸ì\\source\\repos\\lab_6\\lab_6");
			system("dot g.dot -T png -o g.png");
			system("g.png");
		}
		void visualize_path(const path& path, size_t color) noexcept {
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(color);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(color);
					base::nodes[ed.get_to()].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(0);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(0);
					base::nodes[ed.get_to()].set_color(0);
				}
			}

		}
		void visualize_other(const directed_graph& other, size_t color) noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.cend()) {
					base::nodes[(*it).first].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(color);
					base::edges.emplace(ed);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.cend()) {
					base::nodes[(*it).first].set_color(0);
				}
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(0);
					base::edges.emplace(ed);
				}
			}

		}
		void add_graph(const directed_graph<WType, NodeInfo, EdgeInfo>& other) {
			for (typename std::unordered_map<size_t, node>::const_iterator it = other.nodes.cbegin(); it != other.nodes.cend(); it++) {
				this->add_node((*it).first, (*it).second.get_info());
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				this->add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
		}

		directed_graph<WType, NodeInfo, EdgeInfo> get_transponated() const noexcept {
			directed_graph<WType, NodeInfo, EdgeInfo> res;
			for (auto i : base::edges) {
				res.add_node(i.get_from());
				res.add_node(i.get_to());
				res.add_edge(i.get_to(), i.get_from(), i.get_weight(), i.get_info());
			}
			return res;
		}
	};

	template<typename WType = int, class NodeInfo = std::string, class EdgeInfo = std::string>
	class undirected_graph : public graph<WType, false, NodeInfo, EdgeInfo>
	{
	public:
		using node = node<NodeInfo>;
		using edge = edge<EdgeInfo, WType>;
		using path = path<EdgeInfo, WType>;
	private:
		using base = graph<WType, false, NodeInfo, EdgeInfo>;
		void connect_components(size_t node_id, undirected_graph<WType, NodeInfo, EdgeInfo>& res) {
			for (typename std::list<edge>::iterator it = base::incidences_list[node_id].begin(); it != base::incidences_list[node_id].end(); it++) {
				if (res.nodes.find((*it).get_to()) == res.nodes.end())
				{
					base::nodes[(*it).get_to()].set_color(1);
					res.add_node((*it).get_to());
					connect_components((*it).get_to(), res);
				}
				res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
			
		}
		void print_edge(std::ofstream& ofs, const edge& ed) const noexcept override {
			const node& nd = (*base::nodes.find(ed.get_from())).second;
			ofs << nd.get_id();
			ofs << "--";
			const node& nd2 = (*base::nodes.find(ed.get_to())).second;
			ofs << nd2.get_id();
			ofs << " [";
			if (ed.get_color() == 1) {
				ofs << "color=\"red\",";
			}
			else if (ed.get_color() == 2) {
				ofs << "color=\"green\",";
			}
			else if (ed.get_color() == 3) {
				ofs << "color=\"blue\",";
			}
			ofs << "label=\"" << ed.get_info() << " {" << ed.get_weight() << "}" << "\"];\n";
		}
	public:
		std::list<undirected_graph<WType, NodeInfo, EdgeInfo>> skeleton() {
			std::list<undirected_graph<WType, NodeInfo, EdgeInfo>> ans;
			std::list<undirected_graph<WType, NodeInfo, EdgeInfo>> cc = connect_components();
			for (typename std::list<undirected_graph<WType, NodeInfo, EdgeInfo>>::const_iterator it_cc = cc.cbegin(); it_cc != cc.cend(); it_cc++) {
				undirected_graph<WType, NodeInfo, EdgeInfo> res;
				const std::unordered_map<size_t, node>& nodes = (*it_cc).nodes;
				res.nodes.emplace((*(*it_cc).edges.cbegin()).get_from(), (*nodes.find((*(*it_cc).edges.cbegin()).get_from())).second);
				for (typename std::set<edge, typename base::less_x>::const_iterator it = (*it_cc).edges.cbegin(); it != (*it_cc).edges.cend(); it++) {
					if ((res.nodes.find((*it).get_from()) == res.nodes.cend()) ^ ((res.nodes.find((*it).get_to()) == res.nodes.cend()))) {
						res.nodes[(*it).get_from()] = (*nodes.find((*it).get_from())).second;
						res.nodes[(*it).get_to()] = (*nodes.find((*it).get_to())).second;
						res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					}
					else if (res.nodes.find((*it).get_from()) == res.nodes.cend()) {
						res.nodes[(*it).get_from()] = (*nodes.find((*it).get_from())).second;
						res.nodes[(*it).get_to()] = (*nodes.find((*it).get_to())).second;
						res.add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
					}
				}
				ans.push_back(res);
			}
			return ans;
		}
		std::list<undirected_graph<WType, NodeInfo, EdgeInfo>> connect_components() {
			std::list<undirected_graph<WType, NodeInfo, EdgeInfo>> res;
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				if ((*it).second.get_color() == 0) {
					(*it).second.set_color(1);
					undirected_graph<WType, NodeInfo, EdgeInfo> new_cc;
					new_cc.add_node((*it).second.get_id(), (*it).second.get_info());
					connect_components((*it).second.get_id(), new_cc);
					res.push_back(new_cc);
				}
			}
			for (typename std::unordered_map<size_t, node>::iterator it = base::nodes.begin(); it != base::nodes.end(); it++) {
				(*it).second.set_color(0);
			}
			return res;
		}
		void visualize() const noexcept {
			std::ofstream ofs("g.dot");
			ofs << "graph T {\n";
			for (typename std::unordered_map<size_t, node>::const_iterator it = base::nodes.cbegin(); it != base::nodes.cend(); it++) {
				base::print_node(ofs, (*it).second);
			}
			for (typename std::set<edge>::const_iterator it = base::edges.cbegin(); it != base::edges.cend(); it++) {
				print_edge(ofs, (*it));
			}
			ofs << "}";
			ofs.close();
			system("cd C:\\Users\\Àðò¸ì\\source\\repos\\lab_6\\lab_6");
			system("dot g.dot -T png -o g.png");
			system("g.png");
		}
		void visualize_path(const path& path, size_t color) noexcept {
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info(), (*it).get_color());
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(color);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(color);
					base::nodes[ed.get_to()].set_color(color);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					r_ed.set_color(color);
					base::edges.emplace(r_ed);
					base::nodes[r_ed.get_from()].set_color(color);
					base::nodes[r_ed.get_to()].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::list<edge>::const_iterator it = path.get_chain().cbegin(); it != path.get_chain().cend(); it++) {
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info(), (*it).get_color());
				if (base::edges.find(*it) != base::edges.end()) {
					base::edges.erase((*it));
					edge ed((*it));
					ed.set_color(0);
					base::edges.emplace(ed);
					base::nodes[ed.get_from()].set_color(0);
					base::nodes[ed.get_to()].set_color(0);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					r_ed.set_color(0);
					base::edges.emplace(r_ed);
					base::nodes[r_ed.get_from()].set_color(0);
					base::nodes[r_ed.get_to()].set_color(0);
				}
			}
		}
		void visualize_other(const undirected_graph& other, size_t color) noexcept {
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.end()) {
					base::nodes[(*it).first].set_color(color);
				}
				else throw SetException(NoSuchElement);
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.end()) {
					base::edges.erase(ed);
					ed.set_color(color);
					base::edges.emplace(ed);
				}
				else if (base::edges.find(r_ed) != base::edges.end()) {
					base::edges.erase(r_ed);
					ed.set_color(color);
					base::edges.emplace(r_ed);
				}
				else throw SetException(NoSuchElement);
			}
			visualize();
			for (typename std::unordered_map<size_t, std::list<edge>>::const_iterator it = other.incidences_list.cbegin(); it != other.incidences_list.cend(); it++) {
				if (base::nodes.find((*it).first) != base::nodes.end()) {
					base::nodes[(*it).first].set_color(0);
				}
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				edge ed((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
				edge r_ed((*it).get_to(), (*it).get_from(), (*it).get_weight(), (*it).get_info());
				if (base::edges.find(ed) != base::edges.cend()) {
					base::edges.erase(ed);
					ed.set_color(0);
					base::edges.emplace(ed);
				}
				else if (base::edges.find(r_ed) != base::edges.cend()) {
					base::edges.erase(r_ed);
					ed.set_color(0);
					base::edges.emplace(r_ed);
				}
			}
		}
		void add_graph(const undirected_graph& other) {
			for (typename std::unordered_map<size_t, node>::const_iterator it = other.nodes.cbegin(); it != other.nodes.cend(); it++) {
				this->add_node((*it).first, (*it).second.get_info());
			}
			for (typename std::set<edge>::const_iterator it = other.edges.cbegin(); it != other.edges.cend(); it++) {
				this->add_edge((*it).get_from(), (*it).get_to(), (*it).get_weight(), (*it).get_info());
			}
		}
	};
}

