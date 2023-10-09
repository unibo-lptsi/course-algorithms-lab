import math
import random

def prompt_help():
    print("""
       Enter a command:
            - quit: to exit
            - play <FROM> <TO> <NATTEMPTS>: to play   
    """)
    return input()

def guess_number(num_from, num_to, nattempts):
    secret = random.randint(num_from, num_to)
    while nattempts > 0:
        guess = int(input(f"Guess a number in [{num_from},{num_to}] ({nattempts} attempts remaining): "))
        nattempts -= 1
        if guess == secret:
            print("WON")
            return
        else:
            if secret < guess: num_to = guess-1
            else: num_from = guess+1
    print(f"LOSS (secret was {secret})")

cmd = ""
while cmd != "quit":
    cmd = prompt_help()
    if cmd.startswith("play"):
        cmdparts = cmd.split(" ")
        num_from = int(cmdparts[1])
        num_to = int(cmdparts[2])
        n_attempts = int(cmdparts[3])
        if num_to < num_from: num_from, num_to = num_to, num_from
        guess_number(num_from, num_to, n_attempts)