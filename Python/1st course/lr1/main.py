SIZE = 10

# проверка на число


def is_number(x):
    try:  # обработка исключения
        float(x)
        return True
    except ValueError:
        return False


def only_numb(ar0):
    s_on_numb = []
    for m in ar0:
        if is_number(m):
            s_on_numb.append(m)
    return s_on_numb


def even_sum(ar1):
    r_sum = 0
    for j in ar1:
        if is_number(j) and int(j) % 2 == 0:
            r_sum += int(j)
    return r_sum


def concat(ar2):
    s_con = ''
    for k in ar2:
        if len(k) <= 3:
            s_con += k
    return s_con


def odd_numb(ar3):
    odd_pr = 1
    for l in ar3:
        if is_number(l) and int(l) % 2 != 0:
            odd_pr *= int(l)
    return odd_pr


def five_str(ar4):
    fiv = ''
    for t in ar4:
        if len(t) > 4 and not is_number(t):
            for h in range(5, len(t), 5):
                fiv += t[h]
            fiv += ' '
    return fiv


def arr_ind(ar5):
    ind = 0
    for h in ar5:
        print(ind, h)
        ind += 1
    return


def just_list(ar6):
    for v in ar6:
        print(v, end=' ')


initial = []
for i in range(SIZE):
    new_el = input()
    initial.append(new_el)

command = input()
if is_number(command) and int(command) in range(6):
    command = int(command)
    if command == 0:
        temp = only_numb(initial)
        for z in temp:
            print(z, end=' ')
    elif command == 1:
        print(even_sum(initial))
    elif command == 2:
        print(concat(initial))
    elif command == 3:
        print(odd_numb(initial))
    elif command == 4:
        print(five_str(initial))
    elif command == 5:
        arr_ind(initial)
else:
    just_list(initial)
