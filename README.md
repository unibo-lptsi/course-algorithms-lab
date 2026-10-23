# Corso *Algoritmi e Strutture Dati*: Laboratorio

Ogni laboratorio è contenuto in **`asd-labs/<NOME-LAB>/`**: il file `README.md` della cartella contiene il testo
degli esercizi, e i percorsi relativi vanno intesi a partire da quella cartella.
Le soluzioni vengono pubblicate, dopo lo svolgimento del laboratorio, in **`solutions/<NOME-LAB>/`**.

Per aggiornare la propria copia locale: `git pull` (si consiglia di lavorare su copie dei file, o su un proprio
branch, per evitare conflitti con gli aggiornamenti).

## Calendario

<!-- asd-labs:begin -->
| Data | Lab | Testo e sorgenti | Soluzioni |
|---|---|---|---|
| 2026-10-05 | `testing`: testing di algoritmi | [`asd-labs/testing/`](asd-labs/testing/) | [`solutions/testing/`](solutions/testing/) |
| 2026-10-12 | `recursion`: ricorsione | [`asd-labs/recursion/`](asd-labs/recursion/) | [`solutions/recursion/`](solutions/recursion/) |
| 2026-10-19 | `time`: misura dei tempi d'esecuzione di funzioni | [`asd-labs/time/`](asd-labs/time/) | [`solutions/time/`](solutions/time/) |
| 2026-10-26 | `plotting`: grafici di funzioni e di tempi d'esecuzione | [`asd-labs/plotting/`](asd-labs/plotting/) | dal 2026-10-30 |
<!-- asd-labs:end -->

## Preliminari: ambiente di sviluppo

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
