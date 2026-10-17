# Lab `testing`: testing di algoritmi

**Premessa**: lo studio degli algoritmi si concentra sulle loro proprietà formali, specialmente quelle legate alla **correttezza**  e all'**efficienza**. Per ottenere risposte precise o garanzie su questi aspetti, generalmente si usano metodi formali/matematici (ne vedremo qualcuno nel corso). 
Un modo alternativo di ottenere informazioni circa queste proprietà è mediante la **verifica sperimentale**. Poiché tipicamente non è possibile coprire tutti i possibili input e casistiche, l'informazione e dunque la garanzia sarà parziale o comunque informale. In ogni modo, la verifica sperimentale (**testing**) è una pratica comunemente usata per la verifica del software.

0. Si consideri [`min_max.py`](min_max.py), un semplice algoritmo per restituire il più piccolo e il più grande elemento in una lista di interi
    - Osserva il codice della funzione: è corretto?
    - Possiamo esercitare tale funzione in un programma: [`main_minmax.py`](main_minmax.py)
    - Si noti la definizione di una funzione `test(input)` per automatizzare l'esecuzione e reportistica dei risultati
1. Possiamo migliorare l'infrastruttura di testing, generalizzando ed automatizzando ulteriormente
    1. Possiamo astrarre dalla **function-under-test** `f`
    2. Possiamo fornire più **test case** alla volta (quindi, non solo un input, ma diversi input da testare)
    3. Possiamo fornire non solo input ma intere **specifiche** di funzionamento in termini di coppie **(input, output)**: è quello che viene fatto in [`test_utils.py`](test_utils.py)
        ```python
        # signature
        def test(tests: Dict[str, Tuple[Tuple,Any]], f: Callable, tolerance: float = 0.) -> None: # ...

        # usage
        tests = {
            "singleton element": (([77],), (77, 77)), 
        }
        test_utils.test(tests, min_max)
        ```
    4. Osserva il codice in [`main_minmax_test.py`](main_minmax_test.py)
2. Il problema di automatizzare test sul codice include vari "caveat" ed è già stato affrontato dalla comunità. È quindi conveniente ricorrere a librerie come ad esempio **`unittest`**
    - Consultare le slide di laboratorio su questa libreria dal sito del corso
    - Osserva il codice in  [`main_minmax_unittest.py`](main_minmax_unittest.py)
3. Se non l'hai già fatto, risolvi il bug in `min_max` e riesegui i test ;)

## Soluzioni

- Osserva la correzione di `min_max.py`: se la lista è vuota, lancia un `ValueError` (tipo di eccezione indicante parametri di valore non valido); a quel punto, il min/max temporaneo è il primo elemento della lista.
    - il bug originale: inizializzando `min = math.inf` e `max = -math.inf` e usando `if ... elif ...`, il primo elemento aggiorna solo `min`, per cui ad es. `min_max([77])` restituisce `(77, -inf)`; inoltre, su lista vuota viene restituito `(inf, -inf)` invece di segnalare un errore
- In `main_minmax_unittest.py`, il test `test_empty` è reso più stringente verificando che venga sollevata proprio un'eccezione `ValueError` (`assertRaises(ValueError)`) e non una qualunque `Exception`.
