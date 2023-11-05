lst = [3, 7, 0, 9]

for i in range(len(lst)): print(lst[i])

def apply_rec(lst, f, direction='ltr'):
    if len(lst)==0: return
    if len(lst)==1: 
        f(lst[0] if direction=='ltr' else lst[-1])
        return
    apply_rec([lst[0] if direction=='ltr' else lst[-1]], f, direction)
    apply_rec(lst[1:] if direction=='ltr' else lst[:-1], f, direction)

print("---")

apply_rec(lst, print)

print("---")

apply_rec(lst, print, 'rtl')

print("---")

apply_rec(lst, lambda x: print(x*x))

