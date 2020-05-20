def sort_it(x):
    for i in range(1, len(x)):
        j = i - 1
        key = x[i]
        while x[j] > key and j >= 0:
            x[j + 1] = x[j]
            j -= 1
        x[j + 1] = key
    return x


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
list_sort = list(kinds.keys())  # получаем ключи словаря в виде объекта dict_keys и преобразуем к обычному списку
sort_it(list_sort)  # сортируем ключи
for i in list_sort:
    print(i, kinds[i])
