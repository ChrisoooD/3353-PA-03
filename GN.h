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
using ECMap = std::map<Graph::edge_descriptor, double>;
using ECEntry = ECMap::value_type;
using Vertex = typename boost::graph_traits<Graph>::vertex_descriptor;
using Edge   = std::pair<Vertex, Vertex>;

struct node{
    int node_id;
    string node_name;
    vector<pair<int,int>> edges;
    int edge_count(){
        return edges.size();
    }
};
struct my_visitor: boost::default_bfs_visitor{};



class GN{
public:
    Graph g;
    Graph gcopy;
    vector<node> adj;
    vector<node> adjcopy;
    int communityFound = 0;
    double maxQ=-1;
    double calculatedQ=-1;
    int bestRun=0;
    int runs=0;
    vector<vector<int>> communities;


    void gn_run(string filename, int num){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        g = ReadIFGraph(inFile);
        gcopy = g;

        print_graph(g, get(&VertexProperty::Name, g));

        generateOriginalAdj();


        while(communityFound<num){
            if((calculatedQ>maxQ)&(calculatedQ>0))
            {
                maxQ=calculatedQ;
                bestRun = runs;
            }

            Girven_Newman();

            generateAdj();

            communities = foundCommunities();

            runs++;

            qCalculation();
        }

        printCommunities(communities);

    }

    void find_betweeness(int num){
        bfs(0, 3);
        cout<<"vis with path"<<endl;
    }

    void bfs(int s, int e){
//        vector <int> prev = solve(s, e);
    }

    vector<int> reconstructPath(int s, int e, vector<int> &prev){
        vector<int> path;
        int prev_index =0;
        while (prev[prev_index] != e && prev_index < prev.size()){
            path[prev_index] = prev[prev_index];
        }
        if (path[path.size()] == e){
            return path;
        }
    }

    vector<vector<int>> solve(int s, int e){
        vector<vector<int>> all_paths;
        stack<int> curr;
        curr.push(s);
//      whenever a node is visited, mark it as true
        vector<bool> visited;
        for (int i=0; i<adj.size(); i++) {
            visited[i] = false;
        }
//      mark starting node as visited
        visited[s]=true;
//      prev helps reconstructing path
        vector<int> curr_path;
        while (!curr.empty()){
            int curr_top = curr.top();
            if(curr_top == e){
                curr.pop();
            }
            else{
//                todo: need to update visited whenever popping: make end e always false,
                vector<pair<int,int>> curr_adj = adj[curr_top].edges;
                bool has_viable_adj = false;
                int temp_curr;
//                find a viable "go next" for current top
                for (int top_adj=0; top_adj<curr_adj.size();top_adj++){
                    temp_curr = curr_adj[top_adj].second;
                    if (visited[temp_curr]){
                        has_viable_adj = true;
                    }
                }
//                if there is one viable "go next", make that the new top. else, pop top
                if (has_viable_adj){
                    curr.push(temp_curr);
                }
                else {
                    curr.pop();
                }
            }


        }

//        find shortest path
        vector<int> sizes;
        for (int i=0; i<all_paths.size(); i++){
            sizes[i]=all_paths[i].size();
        }
        int min_size = *min_element(sizes.begin(), sizes.end());
        vector<vector<int>> shortest_paths;
        for (int i=0; i<all_paths.size(); i++){
            if (all_paths[i].size()==min_size){
                shortest_paths.push_back(all_paths[i]);
            }
        }
//        return the shortest path(s)
        return shortest_paths;
    }





    void gn_run(string filename){
        std::ifstream inFile(filename);
        if (! inFile.is_open()){
            std::cout<<"nope"<<std::endl;
        }
        g = ReadIFGraph(inFile);
        gcopy = g;

        find_betweeness(0);

//        print_graph(g, get(&VertexProperty::Name, g));
//
//        generateOriginalAdj();
//
//
//        while(communityFound<g.m_vertices.size()){
//            if((calculatedQ>maxQ)&(calculatedQ>0))
//            {
//                maxQ=calculatedQ;
//                bestRun = runs;
//            }
//            Girven_Newman();
//
//            generateAdj();
//
//            communities = foundCommunities();
//
//            runs++;
//
//            qCalculation();
//            cout<<"Best runs happen at "<<bestRun<<" iterations"<<endl;
//            cout<<endl;
//        }
//
//        g = gcopy;
//        for(int i =0;i<bestRun;i++)
//        {
//            Girven_Newman();
//        }
//        generateAdj();
//        communities = foundCommunities();
//        qCalculation();
//        printCommunities(communities);


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

    void generateOriginalAdj(){
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
    }


    void Girven_Newman(){ ///Reference: https://stackoverflow.com/questions/67066766/how-to-calculate-edge-betweenness-with-bgl

        ///Calculate Edge Betweeness
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


        ///Than remove the edge with highest centrality
        int index1=0;
        int index2=0;
        for (ECEntry const& entry : ranking) {
            auto [edge, centrality] = entry;
            int index1 = edge.m_source;
            int index2 = edge.m_target;
//            std::cout << "Edge "<<edge.m_source <<" to "<<edge.m_target << " ";
//            std::cout << " centrality " << centrality << "\n";
            remove_out_edge_if(vertex(index1,g), incident_to(vertex(index2,g), g), g);
        }

    }
    void generateAdj(){
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
    }

    vector<vector<int>> foundCommunities(){ ///Reference: https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/
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
        return communities;
    }

    void printCommunities(vector<vector<int>> communities){
        cout<<"Communities found "<<communities.size()<<endl;
        for (int i = 0; i < communities.size(); ++i) {
            cout << "community " << i+1 << endl;
            for (int j = 0;j<communities[i].size();j++) {
                cout << communities[i][j] << " ";
            }
            cout << endl;
        }
    }
    void qCalculation(){
        calculatedQ= calc_Q(communities);
        cout<<"MaxQ "<<maxQ<<endl;
        cout<<"CalculatedQ "<<calculatedQ<<endl;
        cout<<endl;
        adj.clear();
    }




};



#endif //INC_3353_PA_03_GN_H
