import random


def quick_sort(x):
    if len(x) <= 1:
        return x
    else:
        q = random.choice(x)
        return quick_sort([n for n in x if n < q]) + [q] * x.count(q) + quick_sort([n for n in x if n > q])


kinds = {}
temp = ''
while temp != 'Total':
    temp = input()
    if temp == 'Total':
        break
    else:
        temp = temp.split()
        temp[0] = temp[0].lower()
        if temp[0] not in kinds:
            kinds[temp[0]] = int(temp[1])
        else:
            kinds[temp[0]] += int(temp[1])
list_sort = kinds.keys()  # получаем ключи словаря в виде объекта dict_keys
list_sort = list(list_sort)  # преобразуем к обычному словарю
list_sort = quick_sort(list_sort)  # сортируем ключи
for i, list_sort[i] in enumerate(list_sort):
    print(list_sort[i], kinds[list_sort[i]])
