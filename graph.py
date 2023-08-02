import networkx as nx
import random

# Create a random graph
graph = nx.gnp_random_graph(100, 0.5)
# Assign random weights to edges
#for edge in graph.edges():
#    graph[edge[0]][edge[1]]['capacity'] = random.randint(1, 100)

# Get the list of nodes
nodes = graph.nodes()

# Get the list of edges
edges = graph.edges()

# Print the nodes and their connections

# Print the edges
with open("graph_info.txt", "w") as file:
    for edge in edges:
        file.write(f"{edge[0]} {edge[1]}\n")

#graph = graph.to_directed()
#source = 0
#target = 1
#
#max_flow_value = nx.maximum_flow_value(graph, source, target)
#print("Maximum flow value:", max_flow_value)

