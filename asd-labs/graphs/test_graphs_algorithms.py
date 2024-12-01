import unittest
import networkx as nx
import graphs

class TestGraphsAlgorithms(unittest.TestCase):
    def setUp(self):
        self.g = nx.Graph()
        default_node_attributes = {'visited': False, 'parent': None, 'distance': float("inf"), 'label': '', 'color': '#ededed'}
        self.g.add_nodes_from(list('ABCDEF'), **default_node_attributes)
        self.g.add_weighted_edges_from([('A','B',1), ('A','F',3), ('B','C',3), ('B','E',5), ('B','F',1), 
                            ('C','D',2), ('D','E',1), ('D','F',6), ('E','F',2)], )
        
    def tearDown(self):
        pass

    def test_bfv(self):
        # test 
        bfv_nodes = []
        graphs.bfv(self.g, 'A', lambda x: bfv_nodes.append(x))
        self.assertEqual(bfv_nodes, list('ABFCED'))

    def test_bdv(self):
        # test 
        bdv_nodes = []
        graphs.dfv(self.g, 'A', lambda x: bdv_nodes.append(x))
        self.assertEqual(bdv_nodes, list('ABCDEF'))

    def test_dijkstra(self):
        graphs.dijkstra(self.g, 'A')
        self.assertEqual(list('ABFED'), graphs.shortest_path(self.g, 'A', 'D'))

if __name__ == '__main__':
    unittest.main(verbosity=2)