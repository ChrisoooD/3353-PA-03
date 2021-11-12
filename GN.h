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
struct node{
    int node_id;
    string node_name;
    vector<pair<int,int>> edges;
    int edge_count(){
        return edges.size();
    }
};

using Graph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty, GraphData>;
using ECMap = std::map<Graph::edge_descriptor, double>;
using ECEntry = ECMap::value_type;
using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
using Edge   = std::pair<Vertex, Vertex>;

class GN{
public:
    Graph g;
    Graph gcopy;
    vector<node> adj;
    vector<node> adjcopy;

    void gn_run(string filename){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        g = ReadIFGraph(inFile);
        gcopy = g;

        print_graph(g, get(&VertexProperty::Name, g));


        for (int i=0; i< g.m_vertices.size(); i++){
            node temp_node;
            temp_node.node_name = g.m_vertices[i].m_property.Name;
            temp_node.node_id = i;
            auto *ptr = &g.m_vertices[i].m_out_edges;
            for (auto p: g.m_vertices[i].m_out_edges){
                int l= p.m_target;
                temp_node.edges.push_back(make_pair(i, l));
            }
            adjcopy.push_back(temp_node);
        }
//        cout<<"adjcopy vector created:"<<endl;
//        for (int i=0; i<adjcopy.size(); i++){
//            cout<<adjcopy[i].node_name<<" "<<adjcopy[i].node_id<<" -> ";
//            for (int j=0; j<adjcopy[i].edge_count(); j++){
//                cout<< adjcopy[i].edges[j].second <<" ";
//            }
//            cout<<endl;
//        }







        ///Calculate Edge Betweeness
        int communityFound = 0;
        double maxQ=-1;
        double calculatedQ=-1;
        int bestRun=0;
        int runs=0;

        while(communityFound<12){
            if((calculatedQ>maxQ)&(calculatedQ>0))
            {
                maxQ=calculatedQ;
                bestRun = runs;
            }


            ECMap ecm;
            boost::brandes_betweenness_centrality(g,boost::edge_centrality_map(boost::make_assoc_property_map(ecm)));

            std::vector<std::reference_wrapper<ECEntry>> ranking(ecm.begin(), ecm.end());

            {
                // top-n
                auto n = std::min(1ul, ranking.size());
                auto first = ranking.begin(), middle = first + n, last = ranking.end();
                std::partial_sort(
                        first, middle, last,
                        [](ECEntry const& a, ECEntry const& b) { return a.second > b.second; });

                ranking.erase(middle, last);
            }


            ///Removing the Edge
            int index1=0;
            int index2=0;
            for (ECEntry const& entry : ranking) {
                auto [edge, centrality] = entry;
                int index1 = edge.m_source;
                int index2 = edge.m_target;
                std::cout << "Edge "<<edge.m_source <<" to "<<edge.m_target << " ";
                std::cout << " centrality " << centrality << "\n";
                remove_out_edge_if(vertex(index1,g), incident_to(vertex(index2,g), g), g);
            }
//            ECEntry const& entry = ranking.at(0);
//            auto [edge, centrality] = entry;
//            std::cout << "Edge "<<edge.m_source <<" to "<<edge.m_target << " ";
//            std::cout << " centrality " << centrality << "\n";
//            int index1 = edge.m_source;
//            int index2 = edge.m_target;
//            remove_out_edge_if(vertex(index1,g), incident_to(vertex(index2,g), g), g);






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
//        cout<<"adj vector created:"<<endl;
//        for (int i=0; i<adj.size(); i++){
//            cout<<adj[i].node_name<<" "<<adj[i].node_id<<" -> ";
//            for (int j=0; j<adj[i].edge_count(); j++){
//                cout<< adj[i].edges[j].second <<" ";
//            }
//            cout<<endl;
//        }


            ///Creating the communities by conducting a BFS on all nodes
            vector<vector<int>> communities;
            stack <int> vertexes;
            vector <bool> visited(adj.size());
            int vertexVisited = 0;

            int startVertex=-1;
            int tempVertex =-1;

            while(vertexVisited !=adj.size())
            {
                ///Find node that has not been visited
                for(int i =0;i<adj.size();i++)
                {
                    if(visited[i]==false)
                    {
                        //cout<<"Vertex: " << i <<endl;
                        vertexes.push(i);
                        break;
                    }
                }

                vector<int> singleCommunity;
                while(!vertexes.empty())
                {
                    startVertex=vertexes.top();
                    vertexes.pop();
                    if (!visited[startVertex]) {
                        //mark t
                        visited[startVertex] = true;
                        vertexVisited++;
                        singleCommunity.push_back(startVertex);
                        for (int i = 0; i < adj[startVertex].edge_count(); i++) {
                            tempVertex = adj[startVertex].edges[i].second;
                            if (visited[tempVertex] == false) {
                                vertexes.push(tempVertex);
                            }
                        }
                    }
                }
                communities.push_back(singleCommunity);
            }
            communityFound=communities.size();
            cout<<"Communities found "<<communities.size()<<endl;
            for (int i = 0; i < communities.size(); ++i) {
                cout << "community " << i << endl;
                for (int j = 0;j<communities[i].size();j++) {
                    cout << communities[i][j] << " ";
                }
                cout << endl;
            }

            calculatedQ= calc_Q(communities);
            cout<<"MaxQ "<<maxQ<<endl;
            cout<<"CalculatedQ "<<calculatedQ<<endl;
            cout<<endl;
            adj.clear();
            runs++;
        }

        cout<<"Best runs happen at "<<bestRun<<" iterations"<<endl;

    }


    Graph ReadIFGraph(ifstream& is) {
        Graph graph;
        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("label", boost::get(&VertexProperty::Name, graph));

        boost::read_graphml(is, graph, dp);

        return graph;
    }

    double calc_Q(vector<vector<int>>all_vertices){
        double Q_sum=0;
        double all_links = gcopy.m_edges.size();
        double double_all_links = all_links *2;
        for (int i=0; i<all_vertices.size(); i++){
            double links_between_nodes = calc_links_between_nodes(all_vertices[i]);
            double sum_of_degrees = calc_sum_of_degrees(all_vertices[i]);
            double temp_rhs = sum_of_degrees / double_all_links;
            temp_rhs = temp_rhs * temp_rhs;
            double temp_lhs = links_between_nodes / all_links;
            double temp_long = temp_lhs - temp_rhs;
            Q_sum = Q_sum + temp_long;
        }

        return Q_sum;
    }


    int calc_links_between_nodes(vector<int> community){
        int links_between_nodes =0;
        int sz = community.size();
        for (int i=0; i<(sz-1); i++){
            int a = community[i];
            for (int j=i+1; j<sz;j++){
                int b = community[j];
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
            sum_of_degrees = sum_of_degrees + adjcopy[community[i]].edge_count();
        }
        return sum_of_degrees;
    }





};



#endif //INC_3353_PA_03_GN_H
