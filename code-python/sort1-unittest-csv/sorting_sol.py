def selection_sort(a):
    sorted_idx = -1
    unsorted_idx = 0
    arr_len = len(a)
    # Selection sort loop
    while sorted_idx < arr_len - 1:
        min_idx = unsorted_idx
        for i in range(unsorted_idx, arr_len):
            if a[i] < a[min_idx]:
                min_idx = i
        a[unsorted_idx], a[min_idx] = a[min_idx], a[unsorted_idx]
        sorted_idx = unsorted_idx
        unsorted_idx += 1

def insertion_sort(a):
    for i in range(1,len(a)):
        e = a[i]
        j = i-1
        while j >= 0 and a[j] > e:
            a[j+1] = a[j]
            j = j-1
        a[j+1] = e
