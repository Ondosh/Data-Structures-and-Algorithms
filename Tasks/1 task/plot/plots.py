import pandas as pd
import matplotlib.pyplot as plt

# Если в файле разделитель - точка с запятой
df = pd.read_csv("C:\\Users\\Ondosh\\Desktop\\Data-Structures-and-Algorithms\\Tasks\\1 task\\src\\results.csv", sep=';')

print("Столбцы в файле:", df.columns.tolist())
print("\nПервые 5 строк:")
print(df.head())

plt.figure()

plt.plot(df["n"], df["seq"], label="Sequential")
plt.plot(df["n"], df["bin"], label="Binary")
plt.plot(df["n"], df["interp"], label="Interpolation")

plt.xscale("log", base=2)
plt.yscale("log")

plt.xlabel("Размер массива (n = 2^k)")
plt.ylabel("Время (нс)")
plt.title("Сравнение алгоритмов поиска")

plt.legend()
plt.grid()

plt.show()