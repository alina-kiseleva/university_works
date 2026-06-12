import matplotlib.pyplot as plt
import timeit
from lab_1 import CircularDeque
from random import randint


def test_push_front(n: int):
    deq = CircularDeque(n, True)
    for _ in range(n):
        deq.push_front(randint(-1000, 1000))


def test_push_back(n: int):
    deq = CircularDeque(n, True)
    for _ in range(n):
        deq.push_back(randint(-1000, 1000))


def test_pop_front(n: int, deq):
    for _ in range(n):
        deq.pop_front()


def test_pop_back(n: int, deq):
    for _ in range(n):
        deq.pop_back()


def test_resize(n: int, deq):
    deq.resize(n)


arr_capacities = [100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000]

deq_rsz = CircularDeque(99)
for i in range(99):
    deq_rsz.push_front(i)

times_push_front = []
times_push_back = []
times_pop_front = []
times_pop_back = []
times_resize = []

for cap in arr_capacities:
    times_push_front.append(timeit.timeit(lambda: test_push_front(cap), number=5) / cap / 5 * 10**6)
    times_push_back.append(timeit.timeit(lambda: test_push_back(cap), number=5) / cap / 5 * 10**6)
    
    deq_1 = CircularDeque(cap)
    deq_2 = CircularDeque(cap)
    for i in range(cap):
         deq_1.push_front(i)
         deq_2.push_front(i)
    
    times_pop_front.append(timeit.timeit(lambda: test_pop_front(cap, deq_1), number=1) / cap * 10**6)
    times_pop_back.append(timeit.timeit(lambda: test_pop_back(cap, deq_2), number=1) / cap * 10**6)
    times_resize.append(timeit.timeit(lambda: test_resize(cap, deq_rsz), number=1) * 10**6)

plt.figure(figsize=(10, 6))
plt.plot(arr_capacities, times_push_front, 'o-', linewidth=2, markersize=8)
mean_value = sum(times_push_front)/len(arr_capacities)
plt.axhline(y=mean_value, color='r', linestyle='--', linewidth=2,)
plt.xlabel('Емкость дека')
plt.ylabel('Время выполнения (в миллисекундах)')
plt.title('Производительность метода push_front')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(arr_capacities, times_push_back, 'o-', linewidth=2, markersize=8)
mean_value = sum(times_push_back)/len(arr_capacities)
plt.axhline(y=mean_value, color='r', linestyle='--', linewidth=2,)
plt.xlabel('Емкость дека')
plt.ylabel('Время выполнения (в миллисекундах)')
plt.title('Производительность метода push_back')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(arr_capacities, times_pop_back, 'o-', linewidth=2, markersize=8)
mean_value = sum(times_pop_back)/len(arr_capacities)
plt.axhline(y=mean_value, color='r', linestyle='--', linewidth=2,)
plt.xlabel('Емкость дека')
plt.ylabel('Время выполнения (в миллисекундах)')
plt.title('Производительность метода pop_back')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(arr_capacities, times_pop_front, 'o-', linewidth=2, markersize=8)
mean_value = sum(times_pop_front)/len(arr_capacities)
plt.axhline(y=mean_value, color='r', linestyle='--', linewidth=2,)
plt.xlabel('Емкость дека')
plt.ylabel('Время выполнения (в миллисекундах)')
plt.title('Производительность метода pop_front')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(arr_capacities, times_resize, 'o-', linewidth=2, markersize=8)
k = times_resize[0] / arr_capacities[0]
x_ref = [min(arr_capacities), max(arr_capacities)]
y_ref = [k * x_ref[0], k * x_ref[1]]
plt.plot(x_ref, y_ref, 'r--', linewidth=2)
plt.xlabel('Емкость дека')
plt.ylabel('Время выполнения (в миллисекундах)')
plt.title('Производительность метода resize')
plt.xscale('log')
plt.yscale('log')
plt.grid(True, alpha=0.3)
plt.show()
