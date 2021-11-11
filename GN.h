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
    Graph g;
    vector<node> adj;

    void gn_run(string filename){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        g = ReadIFGraph(inFile);
        print_graph(g, get(&VertexProperty::Name, g));

//        get a list of all the connections

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
        cout<<"adj vector created:"<<endl;
        for (int i=0; i<adj.size(); i++){
            cout<<adj[i].node_name<<" "<<adj[i].node_id<<" -> ";
            for (int j=0; j<adj[i].edge_count(); j++){
                cout<< adj[i].edges[j].second <<" ";
            }
            cout<<endl;
        }
    }


    Graph ReadIFGraph(ifstream& is) {
        Graph graph;
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("Name", boost::get(&VertexProperty::Name, graph));

        boost::read_graphml(is, graph, dp);

        return graph;
    }

    void calc_Q(vector<vector<int>>all_vertices){
        long Q_sum=0;
        int all_links = g.m_edges.size();
        int double_all_links = all_links *2;
        for (int i=0; i<all_vertices.size(); i++){
            int links_between_nodes = calc_links_between_nodes(all_vertices[i]);
            int sum_of_degrees = calc_sum_of_degrees(all_vertices[i]);
            long temp_rhs = sum_of_degrees / double_all_links;
            temp_rhs = temp_rhs * temp_rhs;
            long temp_lhs = links_between_nodes / all_links;
            long temp_long = temp_lhs - temp_rhs;
            Q_sum = Q_sum + temp_long;
        }


    }


    int calc_links_between_nodes(vector<int> community){
        int links_between_nodes =0;
        for (int i=0; i<community.size()-1; i++){
            int a = community[i];
            for (int j=i+1; j<community.size();j++){
                int b = community[i+j];
//                if link builds between vertex a and vertex b, increment links_between_nodes
                if(edge(vertex(a,g),vertex(b,g),g).second)
                    links_between_nodes++;
            }
        }
        return links_between_nodes;
    }

    int calc_sum_of_degrees(vector<int> community){
        int sum_of_degrees =0;
        for (int i=0; i<community.size(); i++){
            sum_of_degrees = sum_of_degrees + adj[community[i]].edge_count();
        }
        return sum_of_degrees;
    }




};



#endif //INC_3353_PA_03_GN_H
