import matplotlib.pyplot as plt
import math

N = []
update = []
getv = []

with open("results.txt") as f:
    for line in f:
        n, a, b = map(int, line.split())
        N.append(n)
        update.append(a)
        getv.append(b)

plt.plot(N, update, marker='o', label="update")
plt.plot(N, getv, marker='o', label="getValue")

logn = [math.log(x) for x in N]
coef = update[0] / logn[0]
scaled_logn = [coef * v for v in logn]
plt.plot(N, scaled_logn, linestyle='--', label="log n")

plt.xscale('log')
plt.yscale('log')

plt.xlabel("Размер n")
plt.ylabel("Время (наносекунды)")
plt.title("Сложность update и getValue")
plt.grid(True)
plt.legend()
plt.savefig("rope_complexity.png", dpi=300)
plt.savefig("pic.png")
