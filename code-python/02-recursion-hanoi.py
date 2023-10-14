import copy # for copy.deepcopy

n = 5
problem = [[i for i in range(n*2,0,-2)],[],[]] # uses even numbers only as they're easier to denote graphically

def print_hanoi(p):
    m = max(max(p[0]+[0]),max(p[1]+[0]),max(p[2]+[0]))
    h = max(len(p[0]), len(p[1]), len(p[2]))
    for r in range(h):
        row = "|"
        for pos in range(3):
            if len(p[pos]) >= h-r:
                disk = '-'*p[pos][h-r-1]
                row += f"{disk:^{m}}"
            else:
                row += " " * m
            row += "|"
        print(row)
    print("|" *(m * 3+4))

# p is the problem; o is the origin (index); d is the destination; i is the intermediate position
def hanoi(p,n,o,d,i):
    if n==1: 
        if len(p[d])>0 and max(p[d]) < p[o][-1]:
            print_hanoi(p) 
            raise Exception(f"cannot move as {len(p[d])} {max(p[d])} < {p[o][-1]}")
        p[d].append(p[o][-1])
        p[o].pop()
        return 
    hanoi(p,n-1,o,i,d)
    hanoi(p,1,o,d,i)
    hanoi(p,n-1,i,d,o)

print_hanoi([[i for i in range(n*2,0,-2)],[],[]])
print_hanoi([[],[],[i for i in range(n*2,0,-2)]])
print_hanoi([[],[i for i in range(n*2,0,-2)],[]])

print("\nINPUT\n")

print(problem)
print_hanoi(problem)

print("\nINTERMEDIATE\n")

res = copy.deepcopy(problem) # N.B. deepcopy as `problem` is a list of lists
hanoi(res,n-1,0,1,2)
print(res)
print_hanoi(res)

print("\nOUTPUT\n")

res = copy.deepcopy(problem)
print(res)
hanoi(res,n,0,2,1)
print_hanoi(res)