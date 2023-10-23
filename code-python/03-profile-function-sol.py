import cProfile

# A sorting function
def selection_sort(arr):
    sorted_idx = -1
    unsorted_idx = 0
    arr_len = len(arr)
    while sorted_idx < arr_len - 1:
        min_idx = unsorted_idx
        for i in range(unsorted_idx, arr_len):
            if arr[i] < arr[min_idx]:
                min_idx = i
        arr[unsorted_idx], arr[min_idx] = arr[min_idx], arr[unsorted_idx]
        sorted_idx = unsorted_idx
        unsorted_idx += 1

def create_list(max):
    return list(range(max,0,-1))

def function_to_be_profiled(max):
    arr = create_list(max)
    selection_sort(arr)
    print(arr[0], arr[len(arr)-1])

for n in (100, 1000):
    print(f"### Profiling for n={n} ###\n")
    
    with cProfile.Profile() as pf:
        function_to_be_profiled(n)

    pf.print_stats()