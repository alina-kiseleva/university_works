import matplotlib.pyplot as plt
from timeit import timeit
from main import *
from random import randint
import math


def testTimSort(n: int):
    arr = [randint(-1000, 1000) for i in range(n)]
    ans = TimSort(arr)


sizes = [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000]
times = []

for size in sizes:
    times.append(timeit(lambda: testTimSort(size), number=1) * 10**6)

n_log_n = [n * math.log2(n) for n in sizes]
C = times[-1] / n_log_n[-1]
scaled_n_log_n = [C * val for val in n_log_n]

plt.figure(figsize=(10, 6))
plt.plot(sizes, times, 'o-', linewidth=2, markersize=8)
plt.plot(sizes, scaled_n_log_n, 'r--', linewidth=2)
plt.xlabel('Размер массива')
plt.ylabel('Время выполнения (в микросекундах)')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()