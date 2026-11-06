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
        - si parta dalla test suite parziale in `test_search_algorithms.py`, basata sul modulo `unittest` (cf. lab `testing`), e la si estenda con ulteriori casi di test
        - si noti come **la stessa suite di test serva per entrambi gli algoritmi**: i test comuni sono definiti nella classe `SearchTests`, mentre le classi `TestLinearSearch` e `TestBinarySearch` (sottoclassi di `SearchTests` e di `unittest.TestCase`) ereditano tali test e specificano solo l'algoritmo da eseguire (campo `search`), più eventuali test specifici
        - si eseguano i test con `python3 test_search_algorithms.py`
        - DOMANDE: quale precondizione devono rispettare gli input dei test comuni? Se l'elemento cercato compare più volte nell'array, che risultato ci aspettiamo?

### Parte 2

2. Con riferimento al file `measure_search_algorithms.py`, che fornisce uno scheletro da completare, **misurare e graficare i tempi d'esecuzione di `linear_search` e `binary_search`**
    1. completare la funzione `measure`, che misura i tempi d'esecuzione di un algoritmo su una lista di input `(array, x)` di dimensione crescente, usando la funzione `measure_ms` (già fornita, cf. lab `plotting`)
        - si notino i *type alias* `SearchAlgorithm`, `SearchInput` e `Measures` (cf. lab `testing`) usati nelle firme delle funzioni e nelle variabili: documentano la struttura dei dati
    2. definire gli input `worst_inputs` corrispondenti al caso peggiore: quale elemento `x` conviene cercare?
    3. completare la funzione `plot_times`, che grafica con matplotlib i tempi d'esecuzione nel caso peggiore al variare della dimensione dell'array, in due grafici affiancati: uno in scala lineare e uno in scala logaritmica (su entrambi gli assi)
        - la struttura della figura (assi, scale, titoli, legenda) è già fornita: mancano le chiamate a `plot()` che tracciano le curve. Si osservi il tipo del parametro `times`: come si ricavano le liste di ascisse e ordinate?
        - prima di farlo: cosa ci aspettiamo di vedere?
        - DOMANDA: in scala logaritmica su entrambi gli assi, una funzione del tipo `c * n^k` appare come una retta di pendenza `k`. Che andamento hanno le curve di `linear_search` e `binary_search`?
    4. (facoltativo) misurare i tempi anche con input di struttura diversa (ad es. elemento cercato in prima posizione, o in posizione casuale)
3. *[Tempo stimato: 30']* Implementare e testare `linear_search_rec` e `binary_search_rec`, varianti degli algoritmi di ricerca strutturati in modo ricorsivo
    - si può lavorare a partire dai file `search_linear_search.py` e `search_binary_search.py`, dove sono fornite le funzioni con corpo vuoto
    - per il testing, si aggiungano in `test_search_algorithms.py` le classi di test per le due varianti, riusando i test già scritti
    - si misurino e si grafichino anche i tempi d'esecuzione delle due varianti ricorsive nel caso peggiore: `measure_search_algorithms.py` riusa `measure` e `plot_times` e salva la figura in `gen/search_times_worst_rec.pdf`
        - DOMANDE: come si confrontano le curve con quelle delle varianti iterative? Perché lo script imposta `sys.setrecursionlimit(...)` (cf. lab `recursion`)?
4. *[Tempo stimato: 20']* Implementare e testare `binary_search_rec_slicing`, variante "ingenua" di `binary_search_rec` che, invece di restringere i limiti della porzione da esaminare, effettua la chiamata ricorsiva su una metà dell'array ottenuta mediante *slicing* (`array[:mid]` oppure `array[mid+1:]`)
    - si può lavorare a partire dal file `search_binary_search.py`, dove è fornita la funzione con corpo vuoto
    - attenzione all'indice da restituire: la chiamata ricorsiva restituisce un indice della sotto-lista, non dell'array originale
    - per il testing, si aggiunga in `test_search_algorithms.py` la relativa classe di test, riusando i test già scritti
    - `measure_search_algorithms.py` ne misura i tempi d'esecuzione nel caso peggiore e li confronta con quelli di `binary_search_rec` in `gen/search_times_worst_slicing.pdf`
        - prima di eseguire lo script: cosa ci aspettiamo di vedere?
        - DOMANDE: che andamento ha la curva di `binary_search_rec_slicing` in scala log-log? Quanto costa un'operazione di slicing come `array[:mid]`? Qual è quindi la complessità di `binary_search_rec_slicing`?

## Soluzioni

- Implementazioni iterative:
    - `search_linear_search.py`
        - si noti l'uso di parametri di default affinché la firma della funzione richieda solo i parametri strettamente necessari (l'`array` e l'elemento `x` da cercare)
        - si noti come si è esplicitata la funzione di uguaglianza mediante parametro `eq`
    - `search_binary_search.py`
        - si noti l'uso di parametri di default affinché la firma della funzione richieda solo i parametri strettamente necessari (l'`array` e l'elemento `x` da cercare): visto che la soluzione prevede di lavorare sui limiti `start` e `to`, conviene che la funzione d'ingresso richiami una funzione che accetta anche tali parametri
        - si noti come si sono esplicitate la relazioni di uguaglianza e di "minore" mediante parametri `eq` e `less`
        - si noti l'uso dell'operatore `//` per la divisione intera
- Implementazioni ricorsive: 
    - `search_linear_search.py` (`linear_search_rec`): si noti uso di parametri `None` per gestire inizializzazione, e l'uso di un parametro che funga da "cursore"
    - `search_binary_search.py` (`binary_search_rec`): si noti come il restringimento della porzione da esaminare si attui mediante parametro attuale alla chiamata ricorsiva
    - DOMANDA: le implementazioni fornite sono tail recursive?
- Test suite: `test_search_algorithms.py`
    - `SearchTests` non estende `unittest.TestCase`, per cui i suoi test non vengono eseguiti "da soli" (fallirebbero, non essendoci un algoritmo da testare), ma solo tramite le sottoclassi `TestLinearSearch`, `TestBinarySearch`, ... che estendono anche `TestCase` (ereditarietà multipla) e ridefiniscono `search`
    - `search` è un campo che contiene la funzione da testare: `staticmethod(...)` fa sì che `self.search(array, x)` non passi `self` come primo argomento
    - nel costruttore di `SearchTests`, il nome di ogni test viene esteso con il nome della funzione in `search` (es. `test_empty[binary_search_rec]`), così che il report indichi quale algoritmo supera o fallisce ciascun test. Poiché `unittest` recupera il metodo di test tramite il suo nome (attributo `_testMethodName`), il metodo viene registrato anche sotto il nuovo nome
    - i test comuni usano solo array ordinati, precondizione di `binary_search`; i test che valgono solo per un algoritmo (es. `linear_search` su array non ordinati, o che restituisce la prima occorrenza) si definiscono nella relativa sottoclasse
    - le classi per le varianti ricorsive estendono quelle delle varianti iterative: ereditano quindi anche i loro test specifici
    - in presenza di duplicati, la specifica richiede solo di restituire *un* indice `i` tale che `array[i] == x`: `binary_search` può non restituire la prima occorrenza, per cui il test verifica l'appartenenza a un insieme di indici ammessi (`assertIn`)
    - `test_every_position` esercita tutte le posizioni e tutti i "buchi" di array di lunghezza sia pari che dispari: è il modo più semplice di scovare errori off-by-one nei limiti della ricerca binaria
    - `test_less_not_weak_order` (in `TestBinarySearch`) è un test **negativo**, su un input che viola la precondizione: l'array deve essere ordinato rispetto a un ordine debole stretto `<` tale che due elementi siano uguali (`==`) sse nessuno dei due è `<` dell'altro. Per gli insiemi, `<` è l'inclusione propria, che non è un ordine debole: `{2}` è incomparabile sia con `{1}` sia con `{1, 3}`, ma `{1} < {1, 3}`. L'array `[{1}, {2}, {1, 3}]` è ordinato (nessun elemento è `<` di uno precedente), ma il confronto con l'elemento centrale `{2}` dà lo stesso esito per `x = {1}` e `x = {1, 3}`, per cui la ricerca prosegue dallo stesso lato in entrambi i casi e uno dei due non viene trovato. Il test verifica che *qualche* elemento non venga trovato: quale dipende dall'implementazione (es. se si confronta `array[m] < x` oppure `x < array[m]`)
    - `test_random` usa un generatore con seed fisso (test riproducibili) e un **oracolo** (l'operatore `in` di Python) per calcolare il risultato atteso; `self.subTest(...)` fa sì che, in caso di fallimento, venga riportato l'input che ha causato l'errore (e che gli altri casi vengano comunque eseguiti)
- Misura e plotting del confronto: `measure_search_algorithms.py`
    - Il caso peggiore, per entrambi gli algoritmi, si ha quando l'elemento cercato non è presente (ad es. `-1`, visto che gli array contengono `0, 1, ..., n-1`)
    - Il confronto è impari: in scala lineare, la curva di `binary_search` è schiacciata sullo zero rispetto a quella di `linear_search`, che cresce linearmente
    - In scala log-log, la curva di `linear_search` tende a una retta di pendenza 1 (`O(n)`), mentre quella di `binary_search` è quasi piatta (`O(log n)` cresce più lentamente di qualunque `n^k` con `k > 0`)
    - Per `n` molto piccoli (fino a ~10) `linear_search` è invece più veloce: ogni passo di `binary_search` costa di più (calcolo del punto medio, due confronti, aggiornamento dei limiti), e con pochi elementi questo conta più del numero di passi. L'analisi asintotica descrive il comportamento per `n` grandi
    - Varianti ricorsive (`gen/search_times_worst_rec.pdf`): l'andamento è lo stesso delle varianti iterative (in log-log, retta di pendenza circa 1 per `linear_search_rec`, curva quasi piatta per `binary_search_rec`), ma i tempi sono più alti (nelle misure, da circa 2 a circa 5 volte per `linear_search_rec`): ogni passo costa una chiamata di funzione (creazione del record di attivazione, passaggio dei parametri), invece di un'iterazione del ciclo. La complessità asintotica non cambia
    - `linear_search_rec` raggiunge una profondità di ricorsione pari a circa `n`, ben oltre il limite di default di Python (circa 1000): senza `sys.setrecursionlimit` si otterrebbe un `RecursionError`. Le due implementazioni ricorsive sono tail recursive, ma Python non ottimizza le chiamate in coda: ogni chiamata ricorsiva resta sullo stack fino al ritorno. `binary_search_rec` arriva invece a una profondità di circa `log2(n)` (17 per `n = 131072`)
- A titolo esemplificativo, si fornisce un'implementazione naive (ingenua) della ricerca binaria ricorsiva, basata sullo slicing delle liste: `binary_search_rec_slicing` in `search_binary_search.py`, figura `gen/search_times_worst_slicing.pdf`
    - quando l'elemento si trova nella metà destra, la chiamata ricorsiva restituisce un indice `i` della sotto-lista `array[mid+1:]`, che corrisponde all'indice `mid+1 + i` di `array` (salvo `-1`, che va propagato così com'è); la metà sinistra `array[:mid]` parte invece dall'indice 0, per cui gli indici coincidono
    - lo slicing `array[a:b]` crea una **nuova lista**, copiando i `b - a` elementi: costa tempo (e memoria) `Θ(b - a)`, anche se nel codice appare come un'operazione "atomica". Ogni chiamata ricorsiva copia quindi metà della porzione corrente: `T(n) = T(n/2) + Θ(n)`, da cui `T(n) = Θ(n + n/2 + n/4 + ...) = Θ(n)`. Il numero di confronti resta `O(log n)`, ma la complessità in tempo passa da `O(log n)` a `O(n)`, come per la ricerca lineare! Anche la memoria aggiuntiva passa da `O(log n)` (i record di attivazione) a `O(n)` (le copie restano in vita finché le rispettive chiamate non ritornano)
    - nelle misure: in scala lineare la curva di `binary_search_rec_slicing` cresce linearmente, mentre quella di `binary_search_rec` è schiacciata sullo zero; in scala log-log, per `n` grandi la prima tende a una retta di pendenza 1, mentre la seconda resta quasi piatta. Per `n = 131072` la variante con slicing è circa 100-200 volte più lenta. Per `n` piccoli la differenza è invece modesta: copiare pochi elementi costa poco rispetto al resto della chiamata
    - `binary_search_rec_slicing` resta comunque più veloce di `linear_search` (nelle misure, circa 10 volte per `n = 131072`): la complessità asintotica è la stessa, ma la copia degli elementi è eseguita dall'interprete in C, con una costante molto più piccola di quella di un ciclo Python che esegue un confronto per elemento
    - morale: la ricerca binaria costa `O(log n)` solo se l'accesso alla porzione da esaminare costa `O(1)`; per questo si passano i limiti (`start`, `to`) invece delle sotto-liste
