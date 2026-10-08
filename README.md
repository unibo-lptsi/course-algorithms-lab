# Corso *Algoritmi e Strutture Dati (Edizione 2026-27)*: Laboratorio

Gli esercizi di ogni laboratorio sono contenuti in  **`asd-labs/<NOME-LAB>/`**.  I percorsi relativi vanno intesi a partire dalla cartella del lab corrispondente.


## Lab `recursion` (2026-10-12): ricorsione

Razionale/obiettivo: acquisire familiarità con la ricorsione, le tipologie di ricorsione, e alcuni esempi pratici.

1. [Tempo stimato: 30'] Studio sorgenti dati
    - `recursion-hanoi.py`: implementazione della soluzione ricorsiva al problema della Torre di Hanoi
    - `recursion-types.py`: implementazione di algoritmi ricorsivi per le tipologie di ricorsione viste a lezione
2. [Tempo stimato: 45'] Esercizi sulla ricorsione: si parta da file `recursion.py` con lo scheletro dell'esercizio
    - NOTA: oltre all'implementazione della soluzione, prevedere una serie di test per verificarne la correttezza. 
    Alcuni sono già forniti a titolo esemplificativo, basati sulla funzionalità di utilità in `test_utils.py`. 
    - Implementare `sum_numbers(a,b)` (somma di tutti i numeri interi compresi tra `a` e `b`) in modo *ricorsivo*
    - Implementare `pow(a,n)` per realizzare l'elevamento a potenza $a^n$ in modo *ricorsivo*
    - Implementare `palindrome(s)` che verifica se la stringa è [palindroma](https://it.wikipedia.org/wiki/Palindromo) in modo *ricorsivo*
    - Implementare `list_contains(lst,elem)` (funzione che restituisce `True` se `elem` è contenuto nella lista `lst` o `False` altrimenti) in modo *ricorsivo*
    - Implementare `list_filter(lst,predicate)`: che restituisce una nuova lista con soli gli elementi di `lst` che soddisfano la funzione predicato `pred` in modo *ricorsivo*
3. [Tempo stimato: 30'] Stack overflow e tail recursion
    1. Si osservi e si esegua il sorgente `recursion_limit.py`: si dovrebbe incorrere in un `RecursionError` (ma si noti che il limite è platform-dependent)
    2. Si provi a impostare [`sys.setrecursionlimit(limit)`](https://docs.python.org/3/library/sys.html#sys.setrecursionlimit) per evitare il problema. Dalla documentazione: **`sys.setrecursionlimit(limit)`** *"Set the maximum depth of the Python interpreter stack to limit. This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python"*.
        - ovviamente in pratica va usato con cautela, in casi circoscritti in cui si conosce il limite finito di ricorsione in modo deterministico
    3. Si ricordi che Python (implementazione CPython) non supporta la tail-call optimization (TCO), anche se esistono moduli come [`tail-recursive`](https://pypi.org/project/tail-recursive/) per abilitarla mediante decoratori e una gestione ad-hoc, seppur con qualche limitazione
    4. Invece, `gcc` dovrebbe supportare TCO. Si provi a compilare `tailrec.c` con due modalità differenti (`-O<N>` per impostare livello di ottimizzazione e `-S` per generare un file assembly `tailrec.s`):
        1. **normale (o senza ottimizzazione)**: `gcc -O1 -S asd-labs/recursion/tailrec.c`
        2. **con ottimizzazione aggressiva**: `gcc -O2 -S asd-labs/recursion/tailrec.c`
            - Visionare l'assembly in `tailrec.s`
            - Osservare come nel caso (2) non sono presenti le chiamate ricorsive (instruzioni `call` per `_factorial_tail`)
    5. Si compili con `gcc -O[1|2] asd-labs/recursion/tailrec.c` e si esegua l'eseguibile prodotto (`.\a.exe` o `./a.out`) e si osservi come l'ottimizzazione possa consentire di evitare un `segmentation fault` 
        - nota: il risultato potrebbe dipendere dall'implementazione/macchina


## Lab `testing` (2026-10-05): testing di algoritmi
<a name="lab01-testing"></a>

**Premessa**: lo studio degli algoritmi si concentrano sulle loro proprietà formali, specialmente quelle legate alla **correttezza**  e all'**efficienza**. Per ottenere risposte precise o garanzie su questi aspetti, generalmente si usano metodi formali/matematici (ne vedremo qualcuno nel corso). 
Un modo alternativo di ottenere informazioni circa queste proprietà è mediante la **verifica sperimentale**. Poiché tipicamente non è possibile coprire tutti i possibili input e casistiche, l'informazione e dunque la garanzia sarà parziale o comunque informale. Ognimodo, la verifica sperimentale (**testing**) è una pratica comunemente usata per la verifica del software.

0. Si consideri [`minmax.py`](asd-labs/testing/minmax.py), un semplice algoritmo per restituire il più piccolo e il più grande elemento in una lista di interi
    - Osserva il codice della funziona: è corretto?
    - Possiamo esercitare tale funzione in un programma: [`main_minmax.py`](asd-labs/testing/main_minmax.py)
    - Si noti la definizione di una funzione `test(input)` per automatizzare l'esecuzione e reportistica dei risultati
1. Possiamo migliore l'infrastruttura di testing, generalizzando ed automatizzando ulteriormente
    1. Possiamo astrarre dalla **function-under-test** `f`
    2. Possiamo fornire più **test case** alla volta (quindi, non solo un input, ma diversi input da testare)
    3. Possiamo fornire non solo input ma intere **specifiche** di funzionamento in termini di coppie **(input, output)**: è quello che viene fatto in [`test_utils.py`](asd-labs/testing/test_utils.py)
```python
# signature
def test(tests: Dict[str, Tuple[Tuple,Any]], f: Callable, tolerance: float = 0.) -> None: # ...

# usage
tests = {
    "singleton element": (([77],), (77, 77)), 
}
test_utils.test(tests, min_max)
```
    4. Osserva il codice in [`main_minmax_test.py`](asd-labs/testing/main_minmax_test.py)
2. Il problema di automatizzare test sul codice include vari "caveat" ed è già stato affrontato dalla comunità. E' quindi conveniente ricorrere a librerie come ad esempio **`unittest`**
    - Consultare le slide di laboratorio su questa libreria dal sito del corso
    - Osserva il codice in  [`main_minmax_unittest.py`](asd-labs/testing/main_minmax_unittest.py)
3. Se non l'hai già fatto, risolvi il bug in `min_max` e riesegui i test ;)

### Soluzione

- Osserva la correzione di `min_max.py`: se la lista è vuota, lancia un `ValueError` (tipo di eccezione indicante parametri di valore non valido); a quel punto, il min/max temporaneo è il primo elemento della lista. 


## Preliminari (2026-10-05): ambiente di sviluppo

- Si raccomanda l'uso di **Visual Studio (VS) Code**
- In VS Code:
    - si usi `File -> Open folder` per selezionare la cartella di lavoro
    - si apre un terminale via `Terminal -> New terminal`
    - si usino i comandi `python` o `gcc` per eseguire/compilare i sorgenti
- Si può lavorare con i notebook Jupyter all'interno di VS Code
    - estensione VSCode `Jupyter` ed ambienti `conda` configurati con pacchetto `jupyter`
- In generale, si può utilizzare l'IDE che si preferisce
    - si tenga però presente che l'ambiente standardizzato nei laboratori usa VS Code
    - e che in sede di esame, Internet è bloccato (i.e., niente Colab)


