import math
from test_utils import test_all

# Implementare `sum_numbers(a,b)` (somma di tutti i numeri interi compresi tra `a` e `b`) in modo *ricorsivo*
def sum_numbers(a,b):
    if a==b: return a
    if a > b: 
        return a + sum_numbers(a-1, b)
    else:
        return b + sum_numbers(a, b-1)

# Implementare `pow(a,n)` (elevamento a potenza) in modo *ricorsivo*
def pow(a, n):
    if n==0: return 1
    if n < 0:
        return pow(a, n+1) / a
    else:
        return a * pow(a, n-1)

# Implementare `list_contains(lst,elem)` (funzione che restituisce `True` 
# se `elem` è contenuto nella lista `lst` o `False` altrimenti) in modo *ricorsivo*
def list_contains(lst, elem):
    if len(lst) == 0: return False
    if lst[0] == elem: return True
    return list_contains(lst[1:], elem)

# Implementare `palindrome(string)` (funzione che restituisce `True` se `string` è una stringa palindroma) 
# in modo *ricorsivo*
def palindrome(s):
    if len(s)==0: return True
    return False if s[0]!=s[-1] else palindrome(s[1:-1])

# Implementare `filter(lst,pred)` (funzione che restituisce una nuova lista con soli gli elementi di `lst` 
# che soddisfano la funzione predicato `pred`) in modo ricorsivo
def list_filter(lst, pred):
    if len(lst)==0: return []
    return ([lst[0]] if pred(lst[0]) else []) + list_filter(lst[1:], pred)

# Specifiche di test
sum_numbers_tests = { f"sum(1,{n})": ((1, n), n*(n+1)//2) for n in range(1,10) }
pow_tests = { "pow({a},{n}": ((a, n), math.pow(a, n)) for a, n in [(0,5), (5,0), (3,3), (2,8), (2,-3)] }
contains_tests = {
    "empty list": (([],0), False), 
    "singleton list (pos)": (([0],0), True), 
    "singleton list (neg)": (([0],1), False), 
    "ordered list (neg)": (([1,3,7],4), False), 
    "ordered list (pos)": (([1,3,7],3), True), 
    "range(1,20) (pos)": ((list(range(1,20)),19), True), 
    "range(1,20) (neg)": ((list(range(1,20)),77), False)
}
palindrome_tests = {
    "simple palindrome": (("emme",), True), 
    "case sensitiveness": (("emmE",), False), 
    "empty spaces": (("emm e",), False), 
    "another palindrome": (("siris",), True), 
    "non palindrome": (("siri",), False)
}
filter_tests = {
    "keep evens": ((list(range(0,10)), lambda x: x%2==0), [0,2,4,6,8]),
    "keep 3-multiples": ((list(range(0,10)), lambda x: x%3==0), [0,3,6,9]),
    "keep larger than 5": ((list(range(0,10)), lambda x: x>5), [6,7,8,9]),
    "keep negatives in list with no negatives": ((list(range(0,10)), lambda x: x<0), [])
}

# Main: test delle funzioni
if __name__ == "__main__":
    test_all(sum_numbers_tests, sum_numbers)
    test_all(pow_tests, pow)
    test_all(contains_tests, list_contains)
    test_all(palindrome_tests, palindrome)
    test_all(filter_tests, list_filter)

