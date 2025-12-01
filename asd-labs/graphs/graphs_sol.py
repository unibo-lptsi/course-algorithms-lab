import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl
import random
import networkx as nx
from collections import deque

# Some constants
L_STATUS = 'status'
(V_STATUS_UNSEEN, V_STATUS_DISCOVERED, V_STATUS_VISITED) = range(3)
L_VISITED = 'visited'
L_DISCOVERED = 'discovered'
L_VISIT_ORDER = 'visit_order'
L_ROOT = 'root'
L_COLOR = 'color'
L_PARENT = 'parent'
L_DIST = 'distance'
L_LABEL = 'label'

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
def bfv(g, root, onVisit, onDiscover=lambda x: None):
    # Initialization
    for n in g.nodes:
        g.nodes[n][L_STATUS] = V_STATUS_UNSEEN
        g.nodes[n][L_PARENT] = None
        g.nodes[n][L_DIST] = float("inf") 
    queue = deque()
    queue.append(root)
    onDiscover(root)
    g.nodes[root][L_STATUS] = V_STATUS_DISCOVERED
    g.nodes[root][L_DIST] = 0
    # Main loop: explore next in queue
    while len(queue) > 0 :
        current = queue.popleft()
        for neighbor in g.neighbors(current):
            if g.nodes[neighbor][L_STATUS] == V_STATUS_UNSEEN:
                g.nodes[neighbor][L_PARENT] = current
                g.nodes[neighbor][L_DIST] = g.nodes[current][L_DIST] + 1
                g.nodes[neighbor][L_STATUS] = V_STATUS_DISCOVERED
                onDiscover(neighbor)
                queue.append(neighbor)
        onVisit(current)
        g.nodes[current][L_STATUS] = V_STATUS_VISITED

# TODO: implement this
def dfv(g, root, onVisit, onDiscover=lambda x: None):
    # Initialization
    for n in g.nodes:
        g.nodes[n][L_STATUS] = V_STATUS_UNSEEN
        g.nodes[n][L_PARENT] = None
        g.nodes[n][L_DIST] = float("inf") 
    g.nodes[root][L_DIST] = 0
    return dfv_rec(g, root, onVisit, onDiscover)

def dfv_rec(g, root, onFinish, onVisit):
    g.nodes[root][L_STATUS] = V_STATUS_DISCOVERED
    onVisit(root)
    for neighbor in g.neighbors(root):
        if g.nodes[neighbor][L_STATUS] == V_STATUS_UNSEEN:
            g.nodes[neighbor][L_PARENT] = root
            g.nodes[neighbor][L_DIST] = g.nodes[root][L_DIST] + 1
            dfv_rec(g, neighbor, onFinish, onVisit)
    onFinish(root)
    g.nodes[root][L_STATUS] = V_STATUS_VISITED
    

# TODO: implement this
# The function should annotate the nodes with their parent and distance from source
def dijkstra(g, src):
    # initialization
    for n in g.nodes:
        g.nodes[n][L_VISITED] = False
        g.nodes[n][L_DIST] = float("inf")
        g.nodes[n][L_PARENT] = ''
    g.nodes[src][L_DIST] = 0
    unvisited = set(g.nodes)
    while unvisited:
        # select the unvisited node with the smallest distance
        current = min(unvisited, key=lambda node: g.nodes[node][L_DIST])
        unvisited.remove(current)
        g.nodes[current][L_VISITED] = True
        for neighbor in g.neighbors(current):
            if not g.nodes[neighbor][L_VISITED]:
                alt = g.nodes[current][L_DIST] + g.edges[current, neighbor]['weight']
                if alt < g.nodes[neighbor][L_DIST]:
                    g.nodes[neighbor][L_DIST] = alt
                    g.nodes[neighbor][L_PARENT] = current

def shortest_path(g, src, dest):
    path = []
    current = dest
    while current != src:
        path.append(current)
        current = g.nodes[current][L_PARENT]
        if current is None:
            return []  # no path found
    path.append(src)
    path.reverse()
    return path

if __name__ == "__main__":
    g = nx.Graph()
    default_node_attributes = { L_VISITED : False, L_PARENT: None, L_DIST: float("inf"), L_LABEL: '', L_COLOR: '#ededed'}
    g.add_nodes_from(char_range('A','F'), **default_node_attributes)
    g.add_weighted_edges_from([('A','B',1), ('A','F',3), ('B','C',3), ('B','E',5), ('B','F',1), 
                            ('C','D',2), ('D','E',1), ('D','F',6), ('E','F',2)], )
    pos = { 'A': (0,0.5), 'B': (1,1), 'C': (2,1), 'D':(3,0.5), 'E':(2,0), 'F':(1,0) }

    print("*** BFV")
    bfv(g, 'A', print) # expected print order: ABFCED
    print("\n*** DFV")
    dfv(g, 'A', lambda x: None, print) # expected print order: ABCDEF
    print("\n*** Dijkstra")
    dijkstra(g, 'A') # should annotate the nodes in the graph
    print(['A', 'B', 'F', 'E', 'D'] == shortest_path(g, 'A', 'D')) # should be True

    plot_basic_graph(g, pos)