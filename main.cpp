#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <fstream>
using namespace std;

int main() {
    std::ifstream inFile;
    inFile.open("football.graphml", std::ifstream::in);

//    typedef boost::adjacency_list<> Graph;
//    Graph g;
//
//    boost::dynamic_properties dp;
//    boost::read_graphml(inFile, g, dp);
    cout<<"end"<<endl;
}

