import random

# Генерируем 100 случайных чисел в диапазоне (0, 1000]
numbers = [round(random.uniform(0.0001, 1000.0), 4) for _ in range(100)]
files = [open("random_numbers.csv", "w+"),open("random_numbers.txt", "w+")]
for num in numbers:
    for f in files:
        f.write(f"{num:.4f} ")  # 4 знака после запятой
for f in files:
    f.close()
