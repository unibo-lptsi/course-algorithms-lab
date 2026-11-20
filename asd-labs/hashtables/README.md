# Lab `hashtables`: tabelle hash

1. [Tempo stimato: 30'] Hashtable a indirizzamento chiuso (chained)
    - Studiare il sorgente `hashtable-chained.c`
    - Implementare una funzione `HashTable *hashtable_init(int nbuckets, TInfo* entries, int nentries)` che crei e inizializzi una hashtable con le entry fornite
    - Implementare una funzione `HashTable *hashtable_merge(HashTable* h1, HashTable *h2)` che restituisca una nuova hashtable data dall'unione delle due tabelle hash fornite in input.
    - Si scriva un test per verificare le funzionalità implementate.
2. [Tempo stimato: 30'] Si copi il file `hashtable-chained.c` in un nuovo file `hashtable-chained-str.c` e si vada ad adattare il sorgente per utilizzare il tipo stringa `char*` per le chiavi.
    - Occorre modificare la `typedef`, aggiustare implementazioni di funzioni, e definire una nuova funzione di `hash` (si faccia riferimento alle slide di teoria per un esempio di implementazione)
3. [EXTRA - Tempo stimato: 30'] Esercizio di realtà: esplorare l'implementazione della classe [java.util.HashMap](https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/HashMap.java) e cercare di ritrovare concetti introdotti a lezione
4. [EXTRA - Tempo stimato: 120'] Prendendo spunto da `hashtable-chained.c`, implementare una hashtable a indirizzamento aperto.
    - Si ricorda che una hashtable a indirizzamento aperto risolve le collisioni andando a occupare bucket successivi della tabella.

