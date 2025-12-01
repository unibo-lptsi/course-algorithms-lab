import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import random
import networkx as nx
from collections import deque

# Some constants
L_STATUS = 'status'
V_STATUS_UNSEEN = 'unseen'
V_STATUS_DISCOVERED = 'discovered'
V_STATUS_VISITED = 'visited'
L_VISITED = 'visited'
L_VISIT_ORDER = 'visit_order'
L_ROOT = 'root'
L_COLOR = 'color'
L_PARENT = 'parent'
L_DIST = 'distance'

def char_range(start='a', end='z'):
    return map(chr,list(range(ord(start),ord(end)+1)))

def plot_basic_graph(G, pos = None, layout = nx.random_layout, seed = None):
    # Define positions for nodes
    if pos is None:
        pos = layout(G, seed = seed)
    # Create matplotlib figure
    fig, ax = plt.subplots(1, 1, figsize=(8,5))
    # Draw nodes and edges
    node_colors = [node[1]['color'] for node in G.nodes(data=True)]
    nx.draw(G, pos, ax = ax, with_labels=True, node_color=node_colors, edgecolors='#000', 
            linewidths=1, font_size=16, font_weight='bold', node_size=600)
    # Draw labels nearby nodes
    for n in G.nodes:
        x, y = pos[n]
        ax.text(x-0.2, y-0.1, f"{G.nodes[n]['label']}",ha='right', va='bottom', color='#00f', fontsize=14)
    # Draw edge labels
    edge_labels = nx.get_edge_attributes(G, 'weight')
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    # Show figure
    fig.set_facecolor("#ededed")
    plt.show()

# TODO: implement this
def bfv(g, root, onVisit = print, onDiscovery = lambda n: None):
    '''
    Breadth-First Visit of graph g from root node.
    
    onVisit: function to call when visiting a node
    onDiscovery: function to call when discovering a node (when first seen and added to the queue)

    Annotations. Each node n in g should be annotated with:
    - g.nodes[n][L_STATUS]: status of n during the BFS (unseen, discovered, visited)
    - g.nodes[n][L_PARENT]: parent of n in the BFS tree
    - g.nodes[n][L_DIST]: distance from root to n in the BFS tree
    '''
    # Initialization
    for n in g.nodes:
        g.nodes[n][L_VISITED] = False    
    pass

# TODO: implement this
def dfv(g, root, onVisit = print, onFinish = lambda n: None):
    '''
    Depth-First Visit of graph g from root node.

    onVisit: function to call when visiting a node
    onFinish: function to call when finishing a node (after all its neighbors have been visited

    Annotations. Each node n in g should be annotated with:
    - g.nodes[n][L_STATUS]: status of n during the DFS (unseen, discovered, visited)
    - g.nodes[n][L_PARENT]: parent of n in the DFS tree
    - g.nodes[n][L_DIST]: distance from root to n in the DFS tree
    '''
    # Initialization
    for n in g.nodes:
        g.nodes[n][L_VISITED] = False 
    return dfv_rec(g, root, onVisit, onFinish)

def dfv_rec(g, root, onVisit, onFinish):
    pass

# TODO: implement this
# The function should annotate the nodes with their parent and distance from source
def dijkstra(g, src):
    '''
    Dijkstra's algorithm to compute shortest paths from src to all other nodes in g.

    Annotations. The function should annotate each node n in g with:
    - g.nodes[n][L_DIST]: the distance from src to n
    - g.nodes[n][L_PARENT]: the parent of n in the shortest path tree
    - g.nodes[n][L_VISITED]: boolean indicating whether the node has been visited
    '''
    # initialization
    for n in g.nodes:
        g.nodes[n][L_VISITED] = False
        g.nodes[n][L_DIST] = float("inf")
        g.nodes[n][L_PARENT] = ''
    pass

def shortest_path(g, src, dest):
    '''
    Reconstruct the shortest path from src to dest in graph g annotated by running Dijkstra's algorithm.
    '''
    pass

if __name__ == "__main__":
    g = nx.Graph()
    default_node_attributes = {'visited': False, 'parent': None, 'distance': float("inf"), 'label': '', 'color': '#ededed'}
    g.add_nodes_from(char_range('A','F'), **default_node_attributes)
    g.add_weighted_edges_from([('A','B',1), ('A','F',3), ('B','C',3), ('B','E',5), ('B','F',1), 
                            ('C','D',2), ('D','E',1), ('D','F',6), ('E','F',2)], )
    pos = { 'A': (0,0.5), 'B': (1,1), 'C': (2,1), 'D':(3,0.5), 'E':(2,0), 'F':(1,0) }

    print("*** BFV")
    bfv(g, 'A', print) # expected print order: ABFCED
    print("\n*** DFV")
    dfv(g, 'A', print) # expected print order: ABCDEF
    print("\n*** Dijkstra")
    dijkstra(g, 'A') # should annotate the nodes in the graph
    print(['A', 'B', 'F', 'E', 'D'] == shortest_path(g, 'A', 'D')) # should be True

    plot_basic_graph(g, pos)