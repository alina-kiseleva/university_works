import matplotlib.pyplot as plt
import math

sizes = []
insert_t, delete_t, index_t, concat_t, split_t = [], [], [], [], []

with open("results.txt") as f:
    for line in f:
        n, a, b, c, d, e = map(float, line.split())
        sizes.append(int(n))
        insert_t.append(a)
        delete_t.append(b)
        index_t.append(c)
        concat_t.append(d)
        split_t.append(e)

theoretical = [math.log2(n) for n in sizes]
scale = max(insert_t) / max(theoretical)
theoretical_scaled = [x * scale for x in theoretical]

plt.figure(figsize=(10, 6))
plt.plot(sizes, insert_t, 'o-', label="Insert")
plt.plot(sizes, delete_t, 'o-', label="Delete")
plt.plot(sizes, index_t, 'o-', label="Index")
plt.plot(sizes, concat_t, 'o-', label="Concat")
plt.plot(sizes, split_t, 'o-', label="Split")
plt.plot(sizes, theoretical_scaled, '--', color='black', label="O(log n) theoretical")
plt.xscale('log')
plt.yscale('log')
plt.xlabel("Размер данных (n)")
plt.ylabel("Время (мкс)")
plt.title("Асимптотическая сложность операций Rope")
plt.legend()
plt.grid(True)
plt.savefig("rope_complexity.png", dpi=300)
plt.show()