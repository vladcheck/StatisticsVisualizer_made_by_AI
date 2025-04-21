import random

name = "random_multiline_with_gaps_uneven"
rows = 10
probability_of_number = 0.7  # Вероятность появления числа вместо тире

# Генерация натуральных чисел (1-1000) и тире
numbers = []
for _ in range(rows):
    row = []
    for _ in range(random.randint(10, 101)):
        if random.random() < probability_of_number:
            # Генерируем натуральное число (целое, >= 1)
            row.append(random.randint(1, 1000))
        else:
            row.append("-")
    numbers.append(row)

# Создание файлов
files = [open(f"{name}.csv", "w+"), open(f"{name}.txt", "w+")]

for row in numbers:
    # Преобразуем числа в строки и соединяем пробелами
    line = " ".join(str(item) for item in row)
    for f in files:
        f.write(line + "\n")

for f in files:
    f.close()
