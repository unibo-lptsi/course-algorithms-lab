# Lab `search`: algoritmi di ricerca

**Razionale/obiettivi:** esercitarsi con gli **algoritmi di ricerca** approfonditi a lezione (ricerca lineare e binaria)

Svolgere i seguenti esercizi, leggendo attentamente le istruzioni.

### Parte 1

1. *[Tempo stimato: 90']* Implementare, testare, e misurare gli algoritmi `linear_search` (ricerca lineare) e `binary_search` (ricerca binaria/dicotomica). Ovvero, per ogni algoritmo:
    1. scrivere un'implementazione in Python
        - si può lavorare a partire dai file `search_linear_search.py` e `search_binary_search.py`
        - ATTENZIONE: occorre **evitare di procedere semplicemente traducendo lo pseudocodice dato verso un'implementazione Python** (questo approccio trasforma il problema dall'*implementare l'algoritmo* a *tradurre istruzioni di pseudocodice ad istruzioni del linguaggio di programmazione scelto*!!!). Il procedimento dovrebbe essere il seguente:
            1. prima di iniziare a scrivere codice, cercare di comprendere l'idea / principio dietro un algoritmo: si consiglia di partire esercitando l'algoritmo con carta e penna su esempi piccoli
            2. disattivando CoPilot o altri strumenti di code-completion, provare a implementare l'idea (N.B.: senza guardare lo pseudocodice fornito, oppure guardandolo una sola volta all'inizio)
            3. verificare la propria soluzione, e correggere in caso di errori
            4. se dopo un po' di tentativi ancora non vi è soluzione, dare un'occhiata allo pseudocodice fornito
    2. scrivere alcuni test per stimare la correttezza dell'implementazione
        - si scelga se usare `test_utils` oppure il modulo `unittest`

### Parte 2

2. Con riferimento al file `measure_search_algorithms.py`, **misurare e graficare i tempi d'esecuzione di `linear_search` e `binary_search`**
    1. misurare i tempi d'esecuzione (usando `time.perf_counter()` o il modulo `timeit`) con input di diversa dimensione e struttura
    2. tracciare i tempi d'esecuzione nel caso peggiore al variare della dimensione dell'array e graficarli con matplotlib
        - prima di farlo: cosa ci aspettiamo di vedere?  
3. *[Tempo stimato: 30']* Implementare e testare `linear_search_rec` e `binary_search_rec`, varianti degli algoritmi di ricerca strutturati in modo ricorsivo
    - si può lavorare a partire dai file `search_linear_search.py` e `search_binary_search.py`, dove sono fornite le funzioni con corpo vuoto

