#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/array.hpp>
#include <array>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;


struct GraphData { std::string Name; };
struct VertexProperty { std::string Name; };
struct EdgeProperty { std::string Name; };

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty, GraphData>;

Graph ReadIFGraph(std::ifstream& is) {
    Graph graph;
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("Name", boost::get(&VertexProperty::Name, graph));

    boost::read_graphml(is, graph, dp);

    return graph;
}


int main() {
    std::ifstream inFile("test.graphml");
    if (! inFile.is_open()){
        std::cout<<"nope"<<std::endl;
    }
    Graph g = ReadIFGraph(inFile);
    print_graph(g, get(&VertexProperty::Name, g));



}

