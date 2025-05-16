#include <iostream>
#include "HashTable.h"
#include "Graph.h"    

int main() {
    Graph<std::string, double> graph;

    graph.add_vertex("A");
    graph.add_vertex("B");
    graph.add_vertex("C");
    graph.add_vertex("D");
    graph.add_vertex("E");

    graph.add_edge("A", "B", 5.0);
    graph.add_edge("B", "C", 3.0);
    graph.add_edge("C", "D", 2.0);
    graph.add_edge("D", "E", 4.0);
    graph.add_edge("A", "C", 10.0);
    graph.add_edge("B", "D", 1.0);
    graph.add_edge("E", "A", 7.0);

    std::cout << "Order: " << graph.order() << std::endl;
    std::cout << "Degree of B: " << graph.degree("B") << std::endl;

    std::cout << "\nWalk from A:\n";
    graph.walk("A", [](const std::string& v) { std::cout << v << " "; });
    std::cout << std::endl;

    std::cout << "\nIs connected: " << (graph.is_connected() ? "Yes" : "No") << std::endl;

    auto path = graph.shortest_path("A", "E");
    std::cout << "\nShortest path from A to E:\n";
    for (const auto& e : path) {
        std::cout << e.from << " -> " << e.to << " (" << e.distance << ")\n";
    }

    auto storage = find_storage(graph);
    std::cout << "Optimal storage location: " << storage << std::endl;

    return 0;
}