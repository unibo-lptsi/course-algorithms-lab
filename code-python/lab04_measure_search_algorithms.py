import timeit
import random
import csv
from lab04_search_binary_search import binary_search
from lab04_search_linear_search import linear_search

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


def random_array(fromN, toN, n, seed = None):
    random.seed(seed)
    return [random.randint(fromN, toN) for i in range(n)]

def write_csv(filename, data):
    with open(filename, "w", newline='') as f:
        w = csv.writer(f, delimiter=",", lineterminator='\n')
        for data_item in data:
            w.writerow(data_item)

input_arrays = [ random_array(0, n*10, n) for n in [1,5,10,100,1000,10000,100000] ]
average_inputs = [(input_array, random.randint(0, len(input_array)*10)) for input_array in input_arrays for cases in range(0,10)]
worst_inputs = [(input_array, input_array[-1]) for input_array in input_arrays]

if __name__ == '__main__': 
    ls_times_avg = measure(linear_search, average_inputs, "LINEAR SEARCH (AVG)")
    # sort inputs for running binary_search
    for input in average_inputs: input[0].sort()
    bs_times_avg = measure(binary_search, average_inputs, "BINARY SEARCH (AVG)")

    ls_times_worst = measure(linear_search, worst_inputs, "LINEAR SEARCH (WORST)")
    # sort inputs for running binary_search
    for input in worst_inputs: input[0].sort()
    bs_times_worst = measure(binary_search, worst_inputs, "BINARY SEARCH (WORST)")

    data = [("lab04_ls_times_avg.csv", ls_times_avg), 
            ("lab04_ls_times_worst.csv", ls_times_worst),
            ("lab04_bs_times_avg.csv", bs_times_avg), 
            ("lab04_bs_times_worst.csv", bs_times_worst)]
    for item in data:
        item[1].insert(0,["n","time[msec]"])
        write_csv(item[0], item[1])