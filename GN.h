//
// Created by AZ on 11/10/2021.
//
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
#ifndef INC_3353_PA_03_GN_H
#define INC_3353_PA_03_GN_H
using namespace std;

struct GraphData { std::string Name; };
struct VertexProperty { std::string Name; };
struct EdgeProperty { std::string Name; };
struct node{
    int node_id;
    string node_name;
    vector<pair<int,int>> edges;
//    bool operator () (const node &lhs, const node &rhs) const{
//        return lhs.connections.size() > rhs.connections.size();
//    }
    int edge_count(){
        return edges.size();
    }
};

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty, GraphData>;

class GN{
public:

    void gn_run(string filename){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        Graph g = ReadIFGraph(inFile);
        print_graph(g, get(&VertexProperty::Name, g));

//        get a list of all the connections
        vector<node> adj;
        for (int i=0; i< g.m_vertices.size(); i++){
            node temp_node;
            temp_node.node_name = g.m_vertices[i].m_property.Name;
            temp_node.node_id = i;
            auto *ptr = &g.m_vertices[i].m_out_edges;
            for (auto p: g.m_vertices[i].m_out_edges){
                int l= p.m_target;
                temp_node.edges.push_back(make_pair(i, l));
            }
            adj.push_back(temp_node);
        }
        cout<<"adj vector created"<<endl;
    }


    Graph ReadIFGraph(ifstream& is) {
        Graph graph;
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("Name", boost::get(&VertexProperty::Name, graph));

        boost::read_graphml(is, graph, dp);

        return graph;
    }

    void calc_edge_betweeness(){

    }




};



#endif //INC_3353_PA_03_GN_H
