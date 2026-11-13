# Lab `sorting`: algoritmi di ordinamento

**Razionale/obiettivi:** esercitarsi con gli **algoritmi di ordinamento** approfonditi a lezione

1. *[Tempo stimato: 90']* Nel file modulo `sorting.py`, implementare e testare gli algoritmi di ordinamento visti a lezione: `selection_sort`, `insertion_sort`, `bubble_sort`, `merge_sort`, `quick_sort`
    * ATTENZIONE: occorre **evitare di procedere semplicemente traducendo lo pseudocodice dato a lezione verso un'implementazione Python** (questo approccio riduce il problema dall'*implementare l'algoritmo* a *tradurre istruzioni di pseudocodice ad istruzioni del linguaggio di programmazione scelto*!!!). Il procedimento dovrebbe essere il seguente:
        1. prima di iniziare a scrivere codice, cercare di comprendere l'idea / principio dietro un algoritmo: si consiglia di partire esercitando l'algoritmo con carta e penna su esempi piccoli
        2. disattivando CoPilot o altri strumenti di code-completion, provare a implementare l'idea (N.B.: senza guardare lo pseudocodice fornito, oppure guardandolo una sola volta all'inizio)
        3. verificare la propria soluzione, e correggere in caso di errori
        4. se dopo un po' di tentativi ancora non vi è soluzione, dare un'occhiata allo pseudocodice fornito
        5. una volta verificata la correttezza di una soluzione, confrontarsi con lo pseudocodice (per scorgere eventuali aspetti da migliorare--ad esempio, piccoli interventi volti a ridurre il numero di iterazioni di un ciclo, o condizioni volte a promuovere una terminazione anticipata di un algoritmo)
    * per il **testing**, si usi la test suite in `test_sorting_algorithms.py`, basata sul modulo `unittest` e strutturata come nel lab `search`: i test comuni sono definiti nella classe `SortingTests`, mentre le classi `TestSelectionSort`, `TestInsertionSort`, ... (sottoclassi di `SortingTests` e di `unittest.TestCase`) ereditano tali test e specificano solo l'algoritmo da eseguire (campo `sort`)
        * si eseguano i test con `python3 test_sorting_algorithms.py`
        * si noti che gli algoritmi ordinano la lista **in-place** (modificandola, senza restituire nulla): il metodo di utilità `assertSorts` ordina una *copia* dell'input e confronta la copia (ordinata) con il risultato atteso
        * si ragioni su quali ulteriori test case considerare, e li si aggiungano a `SortingTests`
2. *[Tempo stimato: 60']* Con riferimento al file `measure_sorting_algorithms.py`, che fornisce uno scheletro da completare, **misurare e graficare i tempi d'esecuzione degli algoritmi di ordinamento** in quattro casi, corrispondenti a diverse strutture dell'input:
    1. array già ordinato (`gen_sorted`, già fornito)
    2. array quasi ordinato (`gen_nearly_sorted`): ottenuto da un array ordinato scambiando `k` coppie di posizioni casuali, con `k` scelto a caso in `[1, log2(n)]`
    3. array casuale (`gen_random`): `n` valori casuali nell'intervallo `[1, n^2]`
    4. array contrordinato (`gen_reversed`)

    Per ogni caso e per ogni dimensione `n`, si generano `N_SEEDS = 10` input diversi, ciascuno con un generatore di numeri casuali `random.Random(seed)` inizializzato con un seed diverso (`0, 1, ..., 9`): i grafici mostrano la **media** dei tempi e, come banda attorno alla curva, la **deviazione standard**.
    * completare i generatori di input `gen_nearly_sorted`, `gen_random`, `gen_reversed`: tutte le scelte casuali vanno fatte con il generatore `rnd` ricevuto come parametro (perché?)
    * DOMANDE: in quali casi la deviazione standard è (quasi) nulla, e perché? Quali algoritmi traggono vantaggio da un input (quasi) ordinato? Quale algoritmo ha il comportamento peggiore su un array ordinato, e perché?

## Soluzioni

- `sorting.py`:
    - notate che per alcuni algoritmi sono definite delle varianti (`selection_sort2` e `insertion_sort2` che non usano funzioni di utilità, e `merge_sort2`)
    - osservate che `merge_sort` fornisce una implementazione più semplice ma anche naive: lavorando con gli slice di liste Python, si incorre in alti costi legati alle copie (`merge_sort2`, invece, lavora sugli indici dell'array originale e usa un unico array di appoggio `temp`)
    - può essere interessante misurare i tempi d'esecuzione di queste diverse implementazioni
- Test suite: `test_sorting_algorithms.py`
    - stessa struttura del lab `search` (classe "mixin" `SortingTests` + una sottoclasse di `unittest.TestCase` per algoritmo, incluse le varianti)
    - `test_in_place` verifica che venga ordinata proprio la lista ricevuta (`assertIs`), non una sua copia
    - `test_random` usa un generatore con seed fisso e la funzione built-in `sorted` come **oracolo**
- Misure: `measure_sorting_algorithms.py`
    - i casi "ordinato" e "contrordinato" non dipendono dal seed: la (piccola) deviazione standard misura solo il rumore della misura. Nel caso "quasi ordinato", invece, la variabilità è alta per gli algoritmi sensibili alla posizione degli elementi scambiati (es. `bubble_sort`, che termina presto solo se non ci sono elementi "piccoli" in fondo, e `quick_sort`)
    - array ordinato: `insertion_sort` e `bubble_sort` (con terminazione anticipata) sono lineari (`Θ(n)`, pendenza 1 in log-log); `selection_sort` resta `Θ(n^2)` (pendenza 2) in ogni caso; `quick_sort` con pivot sul primo elemento degenera in `Θ(n^2)`, perché ogni partizione è sbilanciata (0 e `n-1` elementi), e raggiunge una profondità di ricorsione pari a `n` (da cui `sys.setrecursionlimit`)
    - array quasi ordinato: `insertion_sort` resta quasi lineare (costa `Θ(n + I)`, con `I` numero di inversioni), mentre `bubble_sort` peggiora sensibilmente: ogni passata sposta un elemento "piccolo" indietro di una sola posizione
    - array casuale: gli algoritmi quadratici hanno pendenza 2, `merge_sort` e `quick_sort` `Θ(n log n)` (pendenza poco più di 1)
    - array contrordinato: caso peggiore per `insertion_sort` e `bubble_sort` (e di nuovo per `quick_sort`); `merge_sort` è `Θ(n log n)` in ogni caso
    - `list.sort` (Timsort, in C) è di ordini di grandezza più veloce, ed è lineare su input ordinati o contrordinati (riconosce le sequenze già ordinate, le *run*)
