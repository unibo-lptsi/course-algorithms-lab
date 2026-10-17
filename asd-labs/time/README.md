# Lab `time`: misura dei tempi d'esecuzione di funzioni

**Razionale/obiettivo:** acquisire familiarità con il concetto di complessità temporale, attraverso tecniche di misure dei tempi d'esecuzione.

Consultando le slide sulla misura dei tempi d'esecuzione in Python, si svolgano i seguenti esercizi:

1. *[Tempo stimato: 30']* Si consideri **`measure-fib.py`**. Si misuri e si confronti il tempo d'esecuzione di `fib` (Fibonacci in versione ricorsiva) e `fib_iter` (Fibonacci in versione iterativa). Si utilizzino i *millisecondi* come unità di misura.
    * Si definisca una funzione `measure_running_time(f)` che restituisca il tempo d'esecuzione della funzione `f` fornita in input e la si applichi a `fib` e `fib_iter` per input 10, 20, 30.
        * Si implementi la funzione in due varianti: (A) una usando `time.perf_counter()` e (B) l'altra usando `timeit.repeat`
    * DOMANDA: Quale tra `fib` e `fib_iter` dà luogo a tempi d'esecuzione minori?
    * Osservazione: qual è una limitazione d'uso della funzione `measure_running_time`? Posso impiegarla as-is per misurare altri algoritmi (ad es. di ordinamento liste di numeri)? **Come potrebbe essere generalizzata?** 
    * Esempio di output del programma da implementare:
```
[perf_counter ] fib_rec(5) took 1.347306533716619e-06 sec 	 fib_iter(5) took 1.0011048289015888e-06 sec
[timeit.repeat] fib_rec(5) took 5.810987204313279e-07 sec 	 fib_iter(5) took 4.1349849198013546e-07 sec

[perf_counter ] fib_rec(10) took 7.195401121862233e-06 sec 	 fib_iter(10) took 7.672031642869115e-07 sec
[timeit.repeat] fib_rec(10) took 6.401899736374617e-06 sec 	 fib_iter(10) took 5.166017217561602e-07 sec

...
```

2. *[Tempo stimato: 30']* Si consideri **`profile-function.py`**. Si utilizzi `cProfile` per profilare `function_to_be_profiled()`
    * Qual è la parte più lenta di `function_to_be_profiled()`?
    * Dovresti ottenere un output simile al seguente:
```
### Profiling for n=1000 ###

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        1    0.000    0.000    0.000    0.000 cProfile.py:133(__exit__)
        1    0.000    0.000    0.000    0.000 profile-function.py:17(create_list)
        1    0.000    0.000    0.056    0.056 profile-function.py:20(function_to_be_profiled)
        1    0.056    0.056    0.056    0.056 profile-function.py:4(selection_sort)
        2    0.000    0.000    0.000    0.000 {built-in method builtins.len}
        1    0.000    0.000    0.000    0.000 {built-in method builtins.print}
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
```
3. *[Tempo stimato: 15']* Consultare la pagina [TimeComplexity (python.org)](https://wiki.python.org/moin/TimeComplexity) che documenta la complessità in tempo di varie operazioni su strutture dati nell'implementazione Python `CPython`. Ci si concentri in particolare sulle operazioni su `list`.
    - Si noti come sia importante per chi programma in un linguaggio conoscere la complessità delle funzioni che utilizza.
    - Più avanti nel corso vedremo queste strutture dati e capiremo da dove vengono fuori questi upper bound all'efficienza asintotica
4. *[Tempo stimato: 30']* Si legga la descrizione del progetto [`big-O`](https://pypi.org/project/big-O/)
    * Si rifletta sul problema generale: dedurre la forma/formula di una funzione a partire da punti (e.g., coppie (x,y)). Il problema si chiama **curve fitting**.
    * Un approccio è quello della cosiddetta **[regression analysis](https://en.wikipedia.org/wiki/Regression_analysis)**, dove si considera una certa forma di funzione (ad es `ax+b` per una retta nella cosiddetta **regressione lineare**), e si cerca di trovare i valori dei parametri `a` e `b` che meglio corrispondano ai dati.
    * DA SVOLGERE: Completamento di **`bigo-fib.py`**: si provi, consultando la [documentazione di big_O](https://github.com/pberkes/big_O), ad applicare il modulo allo scopo di inferire la complessità di Fibonacci in versione ricorsiva ed iterativa.
        * nota: il modulo va installato, ad es. con `pip install big_O`
    * Esempio di output atteso:
```
FIB_REC:  Exponential: time = 3.5E-06 * 1.6^n (sec)
FIB_ITER:  Linear: time = -8.5E-05 + 3.6E-06*n (sec)
```

