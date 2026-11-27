# Lab `graphs`: grafi e algoritmi su grafi

Si consideri il file [`graphs.py`](graphs.py). Viene dato un grafo, creato mediante la libreria **NetworkX**. 

1. Si studi il sorgente fornito: si osservi come viene creato il grafo, e come è strutturata la funzione che si occupa di plottarlo.
    * Si faccia riferimento alla [documentazione di NetworkX](https://networkx.org/documentation/stable/reference/index.html)
2. Si implementino le funzioni:
    * `bfv` (visita in ampiezza)
    * `dfv` (visita in profondità)
    * `dijkstra` (annotazione grafo per cammini di costo minimo da un nodo sorgente)
    * `shortest_path` (produzione del cammino di costo minimo da un grafo annotato con Dijkstra)
3. Si verifichi la correttezza di quanto implementato eseguendo lo script di test `test_graphs_algorithms.py`
    * Si osservi l'implementazione del test con la libreria `unittest`

