# Lab `recursion`: ricorsione

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
    - Implementare `list_filter(lst,pred)`: che restituisce una nuova lista con soli gli elementi di `lst` che soddisfano la funzione predicato `pred` in modo *ricorsivo*
3. [Tempo stimato: 30'] Stack overflow e tail recursion
    1. Si osservi e si esegua il sorgente `recursion_limit.py`: si dovrebbe incorrere in un `RecursionError` (ma si noti che il limite è platform-dependent)
    2. Si provi a impostare [`sys.setrecursionlimit(limit)`](https://docs.python.org/3/library/sys.html#sys.setrecursionlimit) per evitare il problema. Dalla documentazione: **`sys.setrecursionlimit(limit)`** *"Set the maximum depth of the Python interpreter stack to limit. This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python"*.
        - ovviamente in pratica va usato con cautela, in casi circoscritti in cui si conosce il limite finito di ricorsione in modo deterministico
    3. Si ricordi che Python (implementazione CPython) non supporta la tail-call optimization (TCO), anche se esistono moduli come [`tail-recursive`](https://pypi.org/project/tail-recursive/) per abilitarla mediante decoratori e una gestione ad-hoc, seppur con qualche limitazione
    4. Invece, `gcc` dovrebbe supportare TCO. Si provi a compilare `tailrec.c` con due modalità differenti (`-O<N>` per impostare livello di ottimizzazione e `-S` per generare un file assembly `tailrec.s`):
        1. **normale (o senza ottimizzazione)**: `gcc -O1 -S tailrec.c`
        2. **con ottimizzazione aggressiva**: `gcc -O2 -S tailrec.c`
            - Visionare l'assembly in `tailrec.s`
            - Osservare come nel caso (2) non sono presenti le chiamate ricorsive (istruzioni `call` per `_factorial_tail`)
    5. Si compili con `gcc -O[1|2] tailrec.c` e si esegua l'eseguibile prodotto (`.\a.exe` o `./a.out`) e si osservi come l'ottimizzazione possa consentire di evitare un `segmentation fault` 
        - nota: il risultato potrebbe dipendere dall'implementazione/macchina

