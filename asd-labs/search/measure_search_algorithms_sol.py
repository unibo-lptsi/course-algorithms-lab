import timeit
import random
import csv
from pathlib import Path
import os 
from search_binary_search_sol import binary_search
from search_linear_search_sol import linear_search
import matplotlib.pyplot as plt

def measure_timeit(f, n_times=10):
    exec_times = timeit.repeat(f, number=n_times, repeat=5)
    return min(exec_times) / n_times

def measure(search_algorithm, input_set, title=''):
    print(f"\n{'*'*20}\nMEASURING {title}\n{'*'*20}")
    results = []
    for input in input_set:
        input_array = input[0]
        n = len(input_array)
        input_value_to_find = input[1]
        t = measure_timeit(lambda: search_algorithm(input_array, input_value_to_find))
        print(f"running time for n={n:10} => {t:8.2} msec")
        results.append((n, t))
    return results

def write_csv(filename, data):
    LAB_DIR = Path(__file__).parent
    os.makedirs(LAB_DIR / "gen", exist_ok=True)
    filename = LAB_DIR / "gen" / filename
    with open(filename, "w", newline='') as f:
        w = csv.writer(f, delimiter=",", lineterminator='\n')
        for data_item in data:
            w.writerow(data_item)

# The input arrays must be sorted for binary_search
input_arrays = [ list(range(0, n)) for n in [1,5,10,100,1000,10000,50000,100000] ]
average_inputs = [(input_array, input_array[len(input_array)//2]) for input_array in input_arrays for cases in range(0,10)]
worst_inputs = [(input_array, -1) for input_array in input_arrays] # the worst case is when the element to find is not included

if __name__ == '__main__': 
    ls_times_avg = measure(linear_search, average_inputs, "LINEAR SEARCH (AVG)")
    bs_times_avg = measure(binary_search, average_inputs, "BINARY SEARCH (AVG)")

    ls_times_worst = measure(linear_search, worst_inputs, "LINEAR SEARCH (WORST)")
    bs_times_worst = measure(binary_search, worst_inputs, "BINARY SEARCH (WORST)")

    # plotting worst case 
    fig, ax = plt.subplots()
    # set limit for y axis
    ax.set_ylim(0, min(max([x[1] for x in ls_times_worst]), max([x[1] for x in bs_times_worst])) * 1.1)
    ax.title.set_text('Worst case running time')
    print(bs_times_worst)
    ax.plot([x[0] for x in ls_times_worst], [x[1] for x in ls_times_worst], label='Linear Search')
    ax.plot([x[0] for x in bs_times_worst], [x[1] for x in bs_times_worst], label='Binary Search')
    ax.set(xlabel='n', ylabel='time', title='Worst case')
    ax.legend()
    plt.show()

    # saving to csv
    data = [("lab04_ls_times_avg.csv", ls_times_avg), 
            ("lab04_ls_times_worst.csv", ls_times_worst),
            ("lab04_bs_times_avg.csv", bs_times_avg), 
            ("lab04_bs_times_worst.csv", bs_times_worst)]
    for item in data:
        item[1].insert(0,["n","time[msec]"])
        write_csv(item[0], item[1])

