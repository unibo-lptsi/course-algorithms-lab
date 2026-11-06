# Lab `sorting`: algoritmi di ordinamento

**Razionale/obiettivi:** esercitarsi con gli **algoritmi di ordinamento** approfonditi a lezione

1. *[Tempo stimato: 90']* Nel file modulo `sorting.py`, implementare e testare gli algoritmi di ordinamento visti a lezione: `selection_sort`, `insertion_sort`, `bubble_sort`, `merge_sort`, `quick_sort`
    * ATTENZIONE: occorre **evitare di procedere semplicemente traducendo lo pseudocodice dato a lezione verso un'implementazione Python** (questo approccio riduce il problema dall'*implementare l'algoritmo* a *tradurre istruzioni di pseudocodice ad istruzioni del linguaggio di programmazione scelto*!!!). Il procedimento dovrebbe essere il seguente:
        1. prima di iniziare a scrivere codice, cercare di comprendere l'idea / principio dietro un algoritmo: si consiglia di partire esercitando l'algoritmo con carta e penna su esempi piccoli
        2. disattivando CoPilot o altri strumenti di code-completion, provare a implementare l'idea (N.B.: senza guardare lo pseudocodice fornito, oppure guardandolo una sola volta all'inizio)
        3. verificare la propria soluzione, e correggere in caso di errori
        4. se dopo un po' di tentativi ancora non vi è soluzione, dare un'occhiata allo pseudocodice fornito
        5. una volta verificata la correttezza di una soluzione, confrontarsi con lo pseudocodice (per scorgere eventuali aspetti da migliorare--ad esempio, piccoli interventi volti a ridurre il numero di iterazioni di un ciclo, o condizioni volte a promuovere una terminazione anticipata di un algoritmo)
    * per il **testing**, si può utilizzare la funzione di test `test_utils.test_all_functions` fornita per sottoporre più funzioni-under-test a una stessa suite di test
    * si noti inoltre che il modulo `test_utils` è stato adattato per supportare, quando il parametro `in_place=True`, il testing di funzioni che modificano **in-place** il loro primo argomento
    * si ragioni su quali ulteriori test case considerare
2. *OPZIONALE*: cercando di riusare le funzionalità di misura tempi e plotting sviluppate nei laboratori precedenti, confrontare gli algoritmi di ordinamento producendo grafici corrispondenti ai casi: (1) array già ordinato, (2) array contrordinato, (3) array casuale 

