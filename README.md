# Corso *Algoritmi e Strutture Dati*: Laboratorio

## Lab 01: Semplici algoritmi in Python

ISTRUZIONI: leggere attentamente i passi seguenti. Completare ogni passo prima di passare al successivo.

(I sorgenti indicati sono contenuti in `code-python/`.)

1. [Tempo stimato: 45'] Studio sorgente `01-intro-algorithms.py` 
    - Questo sorgente include implementazioni di algoritmi molto semplici.
    - E' organizzato in modo tale da semplificare il testing di funzioni realizzate, sfruttando una funzion `test`.
    - COSA FARE:
        1. cercare di capire come si comporta il programma (PRIMA DI ESEGUIRE).
        2. eseguire lo script
        3. approfondire la comprensione dello script, consultando slide e/o documentazione
        4. prendere nota di tutti gli elementi nel codice che non sono chiari: 
    - DOMANDA: quale proprietà desiderata degli algoritmi si intende verificare con la funzione `test`?
2. [Tempo stimato: 45'] Estendere il sorgente dato implementando i seguenti algoritmi (con test):
    1. una funzione `fact(n)` per il calcolo del fattoriale di un numero `n` dato
    2. una funzione `compute_perimeter(shape,*kargs)` per il calcolo del perimetro di una forma `shape` sulla base di una sequenza di parametri `kargs` (da passare dipendentemente dalla forma)
        - lo si computi per rettangolo, quadrato, e cerchio
3. [Tempo stimato: 30'] Realizzare un nuovo script che implementi un algoritmo per il gioco `guess-a-number`
    - Di cosa si tratta: è un gioco che coinvolge un banco e un giocatore. Il banco sceglie casualmente un numero segreto da indovinare (ad es. 7). Il giocatore non conosce tale numero, e dispone di un certo numero di tentativi per indovinarlo. Il banco chiede ripetutamente al giocatore un tentativo, fino a vittoria (il giocatore ha indovinato il segreto) o sconfitta (il giocatore ha esaurito il numero di tentativi); ad ogni risposta errata del giocatore, il banco fornisce un indizio, ovvero l'indicazione se il numero tentativo proposto è più grande o più piccolo del segreto; il giocatore può dunque usare tale informazione per correggere il tentativo seguente.
    - Input:
        - range di valori `(min,max)` del gioco
        - numero max di tentativi ammessi
    - Output:
        - stampa in stdout della stringa `WON` in caso di vittoria e della stringa `LOSS` in caso di sconfitta
    - Consigli
        - si veda [`random.randint()`](https://docs.python.org/3/library/random.html?highlight=randint#random.randint)