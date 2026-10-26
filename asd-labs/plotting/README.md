# Lab `plotting`: grafici di funzioni e di tempi d'esecuzione

**Razionale/obiettivi:** acquisire familiarità con funzioni di diverse classi di complessità, e imparare a **graficare dati** per ottenere rappresentazioni utili allo studio e all'indagine.

Svolgere i seguenti esercizi, leggendo attentamente le istruzioni.

1. *[Tempo stimato: 45']* Si utilizzi `matplotlib` per costruire un'immagine simile a quella sottostante
    * Si faccia riferimento alle slide, al notebook [`../../notebooks/matplotlib.ipynb`](../../notebooks/matplotlib.ipynb), agli **esempi** inclusi in [`../../code-python/mpl/`](../../code-python/mpl/)
        * oltre che al [cheatsheet](https://matplotlib.org/cheatsheets/cheatsheets.pdf), alla [guida](https://matplotlib.org/stable/users/explain/quick_start.html), e alla [API Reference](https://matplotlib.org/stable/api/index.html) 
    * Si parta dal file `plot_functions_of_n.py`
    * Alcune indicazioni:
        * Si può stabilire una lista di funzioni da graficare con: `functions = [float, math.log, lambda n: n*math.log(n), ...]`
        * Si può usare `x_points = np.linspace(...)` per individuare i valori dell'asse `x` da produrre
        * Si può produrre le ordinate dei punti da graficare per ogni funzione con qualcosa del tipo:
        `y_points = np.array([some_function(x) for x in x_points])`
        * Si può plottare con `plot(x_points, y_points)`
        * Si può voler limitare l'asse y mediante la funzione `ylim(min,max)`
        * Si può usare `Axes#set_xscale("log")` per impostare una scala logaritmica sull'asse x 
        * Si può usare `Axes#text(x,y,s)` per posizionare un'etichetta con testo `s` al punto `(x,y)` del grafico
        * Si può usare `list(map(f,l))` per produrre una lista applicando `f` ad ogni elemento della lista `l`. Un'alternativa con NumPy potrebbe essere: usare `fvec = np.vectorize(f)` per ottenere una funzione `fvec` che usi `f` per lavorare su array in modo member-wise (cf. broadcasting).
        * Al posto della funzione fattoriale, si può considerare la funzione [gamma](https://en.wikipedia.org/wiki/Gamma_function), che è una generalizzazione del fattoriale su valori reali. In Python: `math.gamma()`

![](imgs/functions.png)

2. *[Tempo stimato: 45']* Si utilizzi `matplotlib` per graficare il **tempo d'esecuzione** di `fib` (Fibonacci in versione ricorsiva) e `fib_iter` (Fibonacci in versione iterativa) **al variare di `n`**, ottenendo un'immagine simile a quella sottostante
    * Si parta dal file `plot_fib_times.py`, che contiene già le due funzioni (le stesse del lab `time`)
    * Si completi la funzione `measure_ms(f, n_times, n_repeats)`, che restituisca il tempo d'esecuzione di `f()` in *millisecondi* (cf. `measure_timeit` nel lab `time`)
    * Si misurino i tempi delle due funzioni per `n` da 1 a 25 (attenzione: valori molto più grandi rendono `fib` molto lenta), e li si grafichi nella stessa figura con due curve distinte, con titolo, etichette degli assi e legenda
    * Si producano due grafici affiancati: uno con l'asse y in scala lineare e uno con l'asse y in **scala logaritmica**
    * Si salvi la figura in `gen/fib_times.pdf`
    * Alcune indicazioni:
        * Si può usare `fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.5))` per creare una figura con due grafici affiancati
        * Si può usare `Axes#plot(x, y, marker="o", label="...")` per evidenziare i punti misurati
        * Si può usare `Axes#set_yscale("log")` per impostare una scala logaritmica sull'asse y
        * Si può usare `Axes#set_title(s)` e `Figure#suptitle(s)` per i titoli del singolo grafico e della figura
    * DOMANDE:
        * Nel grafico in scala lineare, si riesce ad apprezzare l'andamento di `fib_iter`? Perché?
        * Che forma assume la curva di `fib` nel grafico in scala logaritmica? Cosa ci dice sulla sua complessità?

![](imgs/fib_times.png)

