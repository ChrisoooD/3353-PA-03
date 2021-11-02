import networkx as nx

G = nx.read_gml('F:/CLionProjects/3353-PA-03/football.gml', label='id')

# Adding node_id as a node value to make it easier for the Boost Library to read it
for node_id in G.nodes:
    G.nodes[node_id]['node_id'] = node_id

nx.write_graphml(G, 'football.graphml', named_key_ids=True)