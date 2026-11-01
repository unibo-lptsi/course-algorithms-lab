import timeit # for measuring time
import os
from pathlib import Path
from collections.abc import Callable
import matplotlib.pyplot as plt # for plotting
from search_linear_search import linear_search # to be implemented
from search_binary_search import binary_search # to be implemented

GEN_DIR = Path(__file__).parent / "gen" # where to save the generated figure

# Type aliases (cf. lab testing), documenting the structure of the data exchanged by the functions below
type SearchAlgorithm = Callable[[list[int], int], int] # search(array, x) returns an index of x in array, or -1
type SearchInput = tuple[list[int], int] # a pair (array, x): an input to a search algorithm
type Measures = list[tuple[int, float]] # a list of pairs (n, running time in msec), where n is the length of the array

def measure_ms(f, n_times = 10, n_repeats = 5):
    """ Returns the running time of f() in milliseconds (minimum over n_repeats measures, each averaged over n_times runs) """
    exec_times = timeit.repeat(f, number=n_times, repeat=n_repeats)
    return min(exec_times) / n_times * 1000

def measure(search_algorithm: SearchAlgorithm, inputs: list[SearchInput], title: str = '') -> Measures:
    """ Measures the running time of search_algorithm(array, x) for each pair (array, x) in inputs.
    Returns a list of pairs (n, time in msec), where n is the length of the array """
    print(f"\n{'*'*20}\nMEASURING {title}\n{'*'*20}")
    results = []
    for array, x in inputs:
        n = len(array)
        t = 0.0 # TODO: measure the running time of search_algorithm on (array, x) with measure_ms (N.B.: it takes a function without parameters)
        print(f"running time for n={n:10} => {t:10.4g} msec")
        results.append((n, t))
    return results

def plot_times(times: dict[str, Measures], title: str, filename: str) -> None:
    """ Plots the running times in `times`, which maps each label (e.g., the name of an algorithm) to its measures,
    in two plots side by side: the left one with linear scales, the right one with logarithmic scales (on both axes).
    The figure is saved in the folder gen/, with the given filename """
    fig, (ax_lin, ax_log) = plt.subplots(1, 2, figsize=(11, 4.5), layout="constrained")
    fig.suptitle(title)
    for axes, scale in [(ax_lin, "linear"), (ax_log, "log")]:
        for label, measures in times.items():
            pass # TODO: draw the curve of `measures` in `axes` (n on the x axis, time on the y axis), with the given label (cf. lab plotting)
        axes.set_xscale(scale)
        axes.set_yscale(scale)
        axes.set_title(f"Scala {'lineare' if scale == 'linear' else 'logaritmica (assi x e y)'}")
        axes.set_xlabel("n (dimensione dell'array)")
        axes.set_ylabel("tempo (ms)")
        axes.grid(True, which="major", alpha=0.3)
        axes.legend(loc="upper left")
    os.makedirs(GEN_DIR, exist_ok=True)
    fig.savefig(GEN_DIR / filename, bbox_inches='tight', format='pdf')

if __name__ == '__main__':
    # Input arrays of increasing size n; they must be sorted, as required by binary_search
    sizes = [2**k for k in range(18)] # 1, 2, 4, ..., 131072
    arrays: list[list[int]] = [list(range(n)) for n in sizes]
    worst_inputs: list[SearchInput] = [] # TODO: one pair (array, x) for each array in arrays, such that searching x in array is a worst case

    ls_times_worst = measure(linear_search, worst_inputs, "LINEAR SEARCH (WORST CASE)")
    bs_times_worst = measure(binary_search, worst_inputs, "BINARY SEARCH (WORST CASE)")

    plot_times({"linear_search": ls_times_worst, "binary_search": bs_times_worst},
               "Tempo d'esecuzione nel caso peggiore", "search_times_worst.pdf")
    plt.show()
