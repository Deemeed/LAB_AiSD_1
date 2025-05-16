#include <vector>
#include <functional>
#include <algorithm>
#include <limits>
#include <stack>
#include <map>

template<typename Vertex, typename Distance = double>
struct Edge {
    Vertex from;
    Vertex to;
    Distance distance;

    Edge() : from{}, to{}, distance{} {}
    Edge(const Vertex& f, const Vertex& t, const Distance& d) : from(f), to(t), distance(d) {}

    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to && distance == other.distance;
    }
};

template<typename Vertex, typename Distance = double>
class Graph {
    HashTable<std::vector<Edge<Vertex, Distance>>, Vertex> _graph;
public:
    Graph(size_t size = 16) : _graph(size) {}

    bool has_vertex(const Vertex& v) const {
        return _graph.search(v) != nullptr;
    }

    bool add_vertex(const Vertex& v) {
        if (has_vertex(v)) return false;
        return _graph.insert(v, std::vector<Edge<Vertex, Distance>>());
    }

    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) return false;

        for (auto& vertex : vertices()) {
            auto* edges = _graph.search(vertex);
            if (edges) {
                for (auto& it = edges->begin(); it != edges.end(); ) {
                    if (it->to == v) {
                        it = edges->erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }

        _graph.erase(v);
        return true;
    }

    std::vector<Vertex> vertices() const {
        return _graph.keys();
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!has_vertex(from)) add_vertex(from);
        if (!has_vertex(to)) add_vertex(to);

        auto* edges = _graph.search(from);
        if (edges) edges->emplace_back(from, to, d);
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to)) return false;

        auto* edges = _graph.search(from);
        if (!edges) return false;

        bool removed = false;
        for (auto it = edges->begin(); it != edges->end(); ) {
            if (it->to == to) {
                it = edges->erase(it);
                removed = true;
            }
            else {
                ++it;
            }
        }

        return removed;
    }

    bool remove_edge(const Edge<Vertex, Distance>& e) {
        if (!has_vertex(e.from) || !has_vertex(e.to)) return false;

        auto* edges = _graph.search(e.from);
        if (!edges) return false;

        for (auto it = edges->begin(); it != edges->end(); ) {
            if (*it == e) {
                it = edges->erase(it);
                return true;
            }
        }

        return false;
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) return false;

        const auto* edges = _graph.search(from);
        if (!edges) return false;

        for (const auto& edge : *edges) {
            if (edge.to == to) return true;
        }

        return false;
    }

    bool has_edge(const Edge<Vertex, Distance>& e) const {
        if (!has_vertex(e.from) || !has_vertex(e.to)) return false;

        const auto* edges = _graph.search(e.from);
        if (!edges) return false;

        for (const auto& edge : *edges) {
            if (edge == e) return true;
        }

        return false;
    }

    std::vector<Edge<Vertex, Distance>> edges(const Vertex& vertex) const {
        const auto* edges = _graph.search(vertex);
        if (edges) return *edges;
        else return std::vector<Edge<Vertex, Distance>>();
    }

    size_t order() const {
        return _graph.get_size();
    }

    size_t degree(const Vertex& v) const {
        if (!has_vertex(v)) return 0;

        size_t cnt = 0;

        const auto* edges = _graph.search(v);
        if (edges) cnt += edges->size();

        for (const auto& vertex : vertices()) {
            const auto* edges = _graph.search(vertex);
            if (edges) {
                for (const auto& edge : *edges) {
                    if (edge.to == v) ++cnt;
                }
            }
        }

        return cnt;
    }

    void walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) const {
        if (!has_vertex(start_vertex)) return;

        HashTable<bool, Vertex> visited(_graph.get_size());
        std::stack<Vertex> stack;
        stack.push(start_vertex);

        while (!stack.empty()) {
            Vertex current = stack.top();
            stack.pop();

            if (visited.search(current) && *visited.search(current)) continue;

            action(current);
            visited.insert(current, true);

            const auto* edges = _graph.search(current);
            if (edges) {
                for (const auto& edge : *edges) {
                    if (!visited.search(edge.to) || !*visited.search(edge.to)) {
                        stack.push(edge.to);
                    }
                }
            }
        }
    }

    std::vector<Edge<Vertex, Distance>> shortest_path(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) return {};

        auto all_vertices = vertices();
        std::map<Vertex, Distance> dist;
        std::map<Vertex, Vertex> prev;
        std::map<Vertex, bool> visited;

        for (const auto& v : all_vertices) {
            dist[v] = std::numeric_limits<Distance>::max();
        }
        dist[from] = 0;

        for (size_t i = 0; i < all_vertices.size(); ++i) {
            Vertex current;
            Distance min_dist = std::numeric_limits<Distance>::max();

            for (const auto& v : all_vertices) {
                if (!visited[v] && dist[v] < min_dist) {
                    min_dist = dist[v];
                    current = v;
                }
            }

            if (min_dist == std::numeric_limits<Distance>::max()) break;

            visited[current] = true;

            const auto* edges = _graph.search(current);
            if (edges) {
                for (const auto& e : *edges) {
                    if (e.distance < 0) throw std::runtime_error("Distances must be positive");
                    Distance alt = dist[current] + e.distance;
                    if (alt < dist[e.to]) {
                        dist[e.to] = alt;
                        prev[e.to] = current;
                    }
                }
            }
        }

        if (dist[to] == std::numeric_limits<Distance>::max()) return {};

        std::vector<Edge<Vertex, Distance>> path;
        Vertex current = to;
        while (current != from) {
            Vertex previous = prev[current];
            const auto* edges = _graph.search(previous);
            if (!edges) return {};

            for (const auto& edge : *edges) {
                if (edge.to == current) {
                    path.push_back(edge);
                    current = previous;
                    break;
                }
            }
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    bool is_connected() const {
        auto all_vertices = vertices();
        if (all_vertices.empty()) return true;

        for (const auto& vertex : all_vertices) {
            size_t cnt = 0;
            HashTable<bool, Vertex> visited(_graph.get_size());

            walk(vertex, [&](const Vertex& v) { 
                if (!visited.search(v) || !*visited.search(v)) { 
                    visited.insert(v, true); 
                    cnt++; 
                }
                });

            if (cnt != all_vertices.size()) return false;
        }

        return true;
    }
};

template<typename Vertex, typename Distance = double>
Vertex find_storage(const Graph<Vertex, Distance>& graph) {
    auto all_vertices = graph.vertices();

    Vertex optimal_vertex = all_vertices.front();
    Distance min_max_distance = std::numeric_limits<Distance>::max();

    for (const auto& candidate : all_vertices) {
        Distance current_max_distance = 0;
        bool flag = true;

        for (const auto& target : all_vertices) {
            if (candidate == target) continue;

            auto path = graph.shortest_path(candidate, target);
            if (path.empty()) {
                flag = false;
                break;
            }

            Distance total_distance = 0;
            for (const auto& edge : path) {
                total_distance += edge.distance;
            }

            if (total_distance > current_max_distance) {
                current_max_distance = total_distance;
            }
        }

        if (flag && current_max_distance < min_max_distance) {
            min_max_distance = current_max_distance;
            optimal_vertex = candidate;
        }
    }

    if (min_max_distance == std::numeric_limits<Distance>::max()) {
        throw std::runtime_error("No condidates found");
    }

    return optimal_vertex;
}