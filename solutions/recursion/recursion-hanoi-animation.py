"""
Graphical animation of the recursive Tower of Hanoi algorithm implemented in `recursion-hanoi.py`.

It uses py5 (https://py5coding.org), i.e., Processing for Python 3.
Requirements: Java >= 17 and `pip install py5`.

Usage:  python asd-labs/recursion/recursion-hanoi-animation.py [NUM_DISKS]   (default: 4, max: 10)

Keys:   SPACE play/pause  -  RIGHT one step  -  UP/DOWN (or +/-) faster/slower  -  R restart
"""
import contextlib
import copy
import importlib.util
import io
import os
import sys

import py5

# Load `hanoi` from recursion-hanoi.py (its name contains a dash, so a plain `import` is not possible).
# Loading it also runs its top-level demo code, whose output we silence.
spec = importlib.util.spec_from_file_location(
    "recursion_hanoi", os.path.join(os.path.dirname(os.path.abspath(__file__)), "recursion-hanoi.py"))
hanoi_module = importlib.util.module_from_spec(spec)
with contextlib.redirect_stdout(io.StringIO()):
    spec.loader.exec_module(hanoi_module)


def trace_hanoi(n):
    """Runs the original `hanoi` on a problem with n disks and returns the list of events it generates.

    The recursive calls inside `hanoi` look up the name `hanoi` among the module globals: by temporarily
    replacing it with a wrapper, every (recursive) call goes through the wrapper, which records it.
    Events: ("call", n, o, d, i, disks moved by the call), ("move", disk, o, d, state after move), ("return",)
    """
    original = hanoi_module.hanoi
    events = []

    # The wrapper function that records the events and calls the original `hanoi`, through the `original` variable. 
    # Note: It is important that it has the same signature as `hanoi`, so that recursive calls work correctly.  
    def traced(p, n, o, d, i):
        events.append(("call", n, o, d, i, p[o][-n:]))
        disk = p[o][-1]
        original(p, n, o, d, i)
        if n == 1:
            events.append(("move", disk, o, d, copy.deepcopy(p)))
        events.append(("return",))

    problem = [[i for i in range(n*2, 0, -2)], [], []]  # same encoding as in recursion-hanoi.py
    hanoi_module.hanoi = traced
    # We try to run the traced version of hanoi, and finally restore the original hanoi function, even if an exception occurs.
    try:
        traced(copy.deepcopy(problem), n, 0, 2, 1)
    finally:
        hanoi_module.hanoi = original
    return problem, events


N = int(sys.argv[1]) if len(sys.argv) > 1 else 4
if not 1 <= N <= 10:
    sys.exit("The number of disks must be between 1 and 10")
PROBLEM, EVENTS = trace_hanoi(N)
TOTAL_MOVES = 2**N - 1

# Layout
W, H = 1100, 545 + 24 * N
PEG_X = [W / 6, W / 2, 5 * W / 6]
PEG_TOP, BASE_Y, LIFT_Y = 125, 390, 95
DISK_H = min(30, (BASE_Y - PEG_TOP - 25) / N)
MIN_DISK_W, MAX_DISK_W = 60, 300
STACK_Y, STACK_LINE_H, STACK_INDENT = 500, 24, 22
ROLE_NAMES = {"o": "origin", "d": "destination", "i": "intermediate"}
ROLE_HUES = {"o": 20, "d": 140, "i": 210}

# Duration of each kind of event (in seconds, at speed x1)
DURATIONS = {"call": 0.3, "move": 0.9, "return": 0.15}


class HanoiAnimation:
    def __init__(self):
        self.speed = 1.0
        self.last_millis = py5.millis()
        self.reset()

    def reset(self):
        self.pegs = copy.deepcopy(PROBLEM)
        self.stack = []     # active frames: dicts with keys n, o, d, i, disks, pc (index of the sub-call being run)
        self.flying = None  # (disk, o, d) while a disk is moving
        self.step = 0       # index of the current event
        self.progress = 0.  # in [0,1], progress of the current event
        self.started = False
        self.paused = True
        self.single_step = False
        self.moves = 0
        self.calls = 0
        self.last_move = None

    def done(self):
        # EVENTS contains all the events from an Hanoi execution, so we are done when we have processed all of them.
        return self.step >= len(EVENTS)

    def start_event(self, ev):
        if ev[0] == "call":
            if self.stack:
                self.stack[-1]["pc"] += 1
            _, n, o, d, i, disks = ev
            self.stack.append(dict(n=n, o=o, d=d, i=i, disks=disks, pc=0))
            self.calls += 1
        elif ev[0] == "move":
            _, disk, o, d, _ = ev
            self.pegs[o].pop()
            self.flying = (disk, o, d)
        else:
            self.stack.pop()

    def end_event(self, ev):
        if ev[0] == "move":
            _, disk, o, d, state = ev
            self.pegs = copy.deepcopy(state)  # the state actually produced by the algorithm
            self.flying = None
            self.moves += 1
            self.last_move = (disk, o, d)

    def update(self, dt):
        while not self.paused and not self.done() and dt > 0:
            ev = EVENTS[self.step]
            if not self.started:
                self.start_event(ev)
                self.started = True
            duration = DURATIONS[ev[0]] / self.speed
            remaining = (1 - self.progress) * duration
            if dt < remaining:
                self.progress += dt / duration
                return
            dt -= remaining
            self.end_event(ev)
            self.step += 1
            self.progress = 0.
            self.started = False
            if self.single_step:
                self.paused = True
                self.single_step = False

    def focus_frame(self):
        """The deepest frame with n > 1, i.e., the sub-problem currently being solved."""
        for f in reversed(self.stack):
            if f["n"] > 1:
                return f
        return self.stack[-1] if self.stack else None


# Global variable for the animation object, initialized in `setup()`.
anim = None

def disk_width(v):
    return MIN_DISK_W + (MAX_DISK_W - MIN_DISK_W) * (v // 2 - 1) / max(1, N - 1)

def slot_y(k):
    """y coordinate of the center of the k-th disk (from the bottom) of a peg."""
    return BASE_Y - DISK_H * (k + 0.5)

# Easing function for the animation of moving disks: ease-in-out cubic
# Why the formula? Because it is a simple polynomial that satisfies the conditions:
# ease(0) = 0, ease(1) = 1, ease'(0) = 0, ease'(1) = 0, and is symmetric around t=0.5.
# Easing means that the motion starts slow, speeds up in the middle, and slows down again at the end, which looks more natural than linear motion.
def ease(t):
    return t * t * (3 - 2 * t)

def draw_disk(v, x, y, dimmed):
    py5.fill(300 * (v // 2 - 1) / max(1, N - 1), 15 if dimmed else 60, 95 if dimmed else 92)
    py5.stroke(0, 0, 70 if dimmed else 25)
    py5.stroke_weight(1.5)
    py5.rect(x, y, disk_width(v), DISK_H - 3, 8)
    py5.fill(0, 0, 65 if dimmed else 15)
    py5.text_size(min(15, DISK_H - 6))
    py5.text(str(v), x, y - 2)

def draw_header():
    py5.text_align(py5.LEFT, py5.BASELINE)
    py5.fill(0, 0, 15)
    py5.text_size(22)
    py5.text(f"Tower of Hanoi: hanoi(p, n={N}, o=0, d=2, i=1)", 30, 36)
    py5.text_size(15)
    if anim.done():
        status = f"Done: {anim.moves} moves (2^{N}-1 = {TOTAL_MOVES}), {anim.calls} calls. Press R to restart."
    elif anim.step == 0 and not anim.started:
        status = "Initial configuration. Press SPACE to start."
    else:
        disk, o, d = anim.flying or anim.last_move or (None, None, None)
        status = f"Moves: {anim.moves}/{TOTAL_MOVES}   Calls: {anim.calls}"
        if disk is not None:
            status += f"   {'Moving' if anim.flying else 'Last move'}: disk {disk} from peg {o} to peg {d}"
    py5.text(status, 30, 64)
    py5.text_align(py5.RIGHT, py5.BASELINE)
    py5.fill(0, 0, 40)
    py5.text(("PAUSED   " if anim.paused and not anim.done() else "") + f"speed x{anim.speed:g}", W - 30, 64)
    py5.text("Keys: SPACE play/pause   RIGHT one step   UP/DOWN speed   R restart", W - 30, H - 16)

def draw_pegs():
    # peg roles (o, d, i) refer to the current call; dimmed disks are those outside the current sub-problem
    current, focus = (anim.stack[-1] if anim.stack else None), anim.focus_frame()
    roles = {current[r]: r for r in "odi"} if current else {}
    py5.no_stroke()
    py5.fill(30, 45, 55)
    py5.rect(W / 2, BASE_Y + 8, W - 40, 16, 6)
    py5.text_align(py5.CENTER, py5.CENTER)
    for k, x in enumerate(PEG_X):
        py5.no_stroke()
        py5.fill(30, 45, 45)
        py5.rect(x, (PEG_TOP + BASE_Y) / 2, 10, BASE_Y - PEG_TOP, 4)
        py5.fill(0, 0, 15)
        py5.text_size(16)
        py5.text(f"peg {k}", x, BASE_Y + 32)
        if k in roles:
            r = roles[k]
            py5.fill(ROLE_HUES[r], 70, 65)
            py5.text(f"{r} ({ROLE_NAMES[r]})", x, BASE_Y + 55)
    for k, peg in enumerate(anim.pegs):
        for h, v in enumerate(peg):
            draw_disk(v, PEG_X[k], slot_y(h), focus is not None and v not in focus["disks"])
    if anim.flying:
        v, o, d = anim.flying
        x0, y0 = PEG_X[o], slot_y(len(anim.pegs[o]))
        x1, y1 = PEG_X[d], slot_y(len(anim.pegs[d]))
        t = anim.progress * 3  # three phases: lift, translate, drop
        if t < 1:
            # lerp(from, to, t) is linear interpolation between from and to, with t in [0,1]
            x, y = x0, py5.lerp(y0, LIFT_Y, ease(t))
        elif t < 2:
            x, y = py5.lerp(x0, x1, ease(t - 1)), LIFT_Y
        else:
            x, y = x1, py5.lerp(LIFT_Y, y1, ease(min(1, t - 2)))
        draw_disk(v, x, y, False)

def frame_note(f):
    n, o, d, i, pc = f["n"], f["o"], f["d"], f["i"], f["pc"]
    if n == 1:
        return f"base case: move disk {f['disks'][-1]} from {o} to {d}"
    return ["",
            f"(1) hanoi(p,n-1,o,i,d): move {n-1} disk(s) from {o} to {i}",
            f"(2) hanoi(p,1,o,d,i): move disk {f['disks'][0]} from {o} to {d}",
            f"(3) hanoi(p,n-1,i,d,o): move {n-1} disk(s) from {i} to {d}"][pc]

def draw_stack():
    py5.text_align(py5.LEFT, py5.CENTER)
    py5.fill(0, 0, 15)
    py5.text_size(16)
    py5.text("Call stack (most recent call last)", 30, STACK_Y - 30)
    py5.stroke(0, 0, 75)
    py5.line(30, STACK_Y - 15, W - 30, STACK_Y - 15)
    py5.text_size(15)
    note_x = 40 + STACK_INDENT * (N - 1) + py5.text_width("hanoi(p, n=10, o=0, d=2, i=1)") + 30
    for depth, f in enumerate(anim.stack):
        y = STACK_Y + depth * STACK_LINE_H
        if depth == len(anim.stack) - 1:
            py5.no_stroke()
            py5.fill(50, 30, 100)
            py5.rect(W / 2, y, W - 60, STACK_LINE_H - 2, 4)
        py5.fill(0, 0, 15)
        py5.text(f"hanoi(p, n={f['n']}, o={f['o']}, d={f['d']}, i={f['i']})", 40 + STACK_INDENT * depth, y)
        py5.fill(220, 60, 50)
        py5.text(frame_note(f), note_x, y)

# Py5 setup and draw functions
# This function is called by py5 to set the sketch size and other settings before setup() is called.
def settings():
    py5.size(W, H)

# Py5 setup function, called once at the beginning of the sketch.
def setup():
    global anim
    py5.window_title(f"Tower of Hanoi ({N} disks)")
    py5.color_mode(py5.HSB, 360, 100, 100, 100)
    py5.rect_mode(py5.CENTER) # draw rectangles from their center
    py5.text_font(py5.create_font("Monospaced", 15))
    py5.frame_rate(60)
    anim = HanoiAnimation()

# Py5 draw function, called repeatedly to update the sketch.
def draw():
    now = py5.millis()
    anim.update(min(0.1, (now - anim.last_millis) / 1000))
    anim.last_millis = now
    py5.background(40, 4, 98)
    draw_header()
    draw_pegs()
    draw_stack()

# Py5 key_pressed function, called when a key is pressed.
def key_pressed():
    if py5.key == " ":
        anim.paused = not anim.paused
        anim.single_step = False
    elif py5.key in "rR":
        anim.reset()
    elif py5.key in "+=" or (py5.key == py5.CODED and py5.key_code == py5.UP):
        # py5.CODED is used for arrow keys, which are not regular characters
        anim.speed = min(64, anim.speed * 2)
    elif py5.key in "-_" or (py5.key == py5.CODED and py5.key_code == py5.DOWN):
        anim.speed = max(0.125, anim.speed / 2)
    elif py5.key == py5.CODED and py5.key_code == py5.RIGHT and anim.paused:
        anim.paused = False
        anim.single_step = True

if __name__ == "__main__":
    # Run the sketch if this file is executed as a script.
    # After run_sketch(), the sketch will run until the user closes the window, and then the program will exit.
    py5.run_sketch()
