import math

def min_max(lst):
    if len(lst) == 0: raise ValueError("List is empty")
    min = lst[0]
    max = lst[0]
    for item in lst[1:]:
        if item < min: min = item
        elif item > max: max = item
    return min, max