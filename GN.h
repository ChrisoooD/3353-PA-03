//
// Created by AZ on 11/10/2021.
//
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/config.hpp>
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
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/graph_traits.hpp>
#ifndef INC_3353_PA_03_GN_H
#define INC_3353_PA_03_GN_H
using namespace std;
using namespace boost;

struct GraphData { std::string Name; };
struct VertexProperty { std::string Name; };
struct EdgeProperty { std::string Name; };

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty, GraphData>;

class GN{
public:

    void gn_run(string filename){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        Graph g = ReadIFGraph(inFile);
        //print_graph(g, get(&VertexProperty::Name, g));


//        if(edge(vertex(0,g),vertex(1,g),g).second)
//            cout<<"there is a edge"<<endl;
//        else
//            cout<<"fuck you"<<endl;







        ///Calculate Edge Betweeness
//        for(int i =0;i<5;i++){
            boost::shared_array_property_map<double, boost::property_map<Graph, boost::vertex_index_t>::const_type>
            centrality_map(num_vertices(g), get(boost::vertex_index, g));
            boost::brandes_betweenness_centrality(g,centrality_map);

            int highest1 = 0;
            int highest2 = 0;
            int index1 = 0;
            int index2 = 0;
            bool removed=false;

            for(int i =0;i<g.m_vertices.size();i++)
            {
                //cout<<centrality_map[i]<<endl;
                if(centrality_map[i]>highest1)
                {
                    highest1=centrality_map[i];
                    index1=i;
                }
                else if(centrality_map[i]>highest2)
                {
                    highest2=centrality_map[i];
                    index2=i;
                }
            }

            while(removed==false){
                if(edge(vertex(index1,g),vertex(index2,g),g).second)
                {
                    remove_out_edge_if(vertex(index1,g), incident_to(vertex(index2,g), g), g);
                    cout<<"Removed vertex "<<index1<<" "<<index2<<endl;
                    removed=true;
                }
                else
                {
                    cout<<"Attempting to remove vertex "<<index1<<" "<<index2<<", remove fail"<<endl;
                    centrality_map[index2]=0;
                    highest2=0;
                }
                for(int i =0;i<g.m_vertices.size();i++)
                {
                    //cout<<centrality_map[i]<<endl;
                    if(centrality_map[i]>highest2)
                    {
                        highest2=centrality_map[i];
                        index2=i;
                    }
                }
            }



//        }


        //print_graph(g, get(&VertexProperty::Name, g));

//        boost::brandes_betweenness_centrality(g,centrality_map);
//
//        for(int i =0;i<g.m_vertices.size();i++)
//            cout<<centrality_map[i]<<endl;

        vector <int> community;



    }


    Graph ReadIFGraph(ifstream& is) {
        Graph graph;
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("Name", boost::get(&VertexProperty::Name, graph));

        boost::read_graphml(is, graph, dp);

        return graph;
    }





};



#endif //INC_3353_PA_03_GN_H
