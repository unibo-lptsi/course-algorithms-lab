import math

def min_max(lst):
    min = math.inf
    max = -math.inf
    for item in lst:
        if item < min: min = item
        elif item > max: max = item
    return min, max