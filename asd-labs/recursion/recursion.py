import math
from test_utils import test_all

# Implementare `sum_numbers(a,b)` (somma di tutti i numeri interi compresi tra `a` e `b`) in modo *ricorsivo*
def sum_numbers(a,b):
    pass

# Implementare `pow(a,n)` (elevamento a potenza) in modo *ricorsivo*
def pow(a, n):
    pass

# Implementare `list_contains(lst,elem)` (funzione che restituisce `True` 
# se `elem` è contenuto nella lista `lst` o `False` altrimenti) in modo *ricorsivo*
def list_contains(lst, elem):
    pass

# Implementare `palindrome(string)` (funzione che restituisce `True` se `string` è una stringa palindroma) 
# in modo *ricorsivo*
def palindrome(s):
    pass

# Implementare `filter(lst,pred)` (funzione che restituisce una nuova lista con soli gli elementi di `lst` 
# che soddisfano la funzione predicato `pred`) in modo ricorsivo
def list_filter(lst,pred):
    pass

sum_numbers_tests = { f"sum(1,{n})": ((1, n), n*(n+1)//2) for n in range(1,4) }
pow_tests = { 
    # add tests
}
contains_tests = {
    "empty list": (([],0), False), 
    # add more tests
}
palindrome_tests = {
    # add tests
}
def even(x): return x%2==0
filter_tests = {
    "keep evens": ((list(range(0,10)), even), [0,2,4,6,8]),
    # add more tests
}

# Main: test delle funzioni
if __name__ == "__main__":
    test_all(sum_numbers_tests, sum_numbers)
    test_all(pow_tests, pow)
    test_all(contains_tests, list_contains)
    test_all(palindrome_tests, palindrome)
    test_all(filter_tests, list_filter)

