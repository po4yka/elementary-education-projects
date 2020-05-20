tape = [i for i in input()]
res = ''
state = "q1"
pos = 0
while state != "q12":
    res += state + " "  # выполненные операции для проверки

    if state == "q1":  # сдвиг вправо
        if tape[pos] == "+" or tape[pos] == "-":
            state = "q2"
        pos += 1

    elif state == "q2":  # определение цифры справа от знака
        if tape[pos] == "0":
            state = "q12"
        elif tape[pos] == "1":
            pos -= 1
            state = "q3"
        elif tape[pos] == "2":
            pos -= 1
            state = "q8"

    elif state == "q3":  # определение знака для 1
        if tape[pos] == "+":
            state = "q4"
        elif tape[pos] == "-":
            state = "q5"
        pos -= 1

    elif state == "q4":  # сложение для 1
        if tape[pos] == " ":
            state = "q12"
            tape[pos] = "1"
        elif tape[pos] != "2":
            state = "q12"
            tape[pos] = str(int(tape[pos]) + 1)
        else:
            tape[pos] = "0"
            pos -= 1

    elif state == "q5":  # вычитание для 1
        if tape[pos] == "0":
            tape[pos] = "2"
            pos -= 1
        elif tape[pos] == "1":
            tape[pos] = "0"
            state = "q6"
            pos -= 1
        elif tape[pos] == "2":
            tape[pos] = "1"
            state = "q12"

    elif state == "q6":  # переход в начало числа
        if tape[pos] == " ":
            state = "q11"
            pos += 1
        else:
            state = "q12"

    elif state == "q7":  # вставка результирующего нуля
        tape[pos] = "0"
        state = "q12"

    elif state == "q8":  # определение знака для 2
        if tape[pos] == "+":
            state = "q9"
        elif tape[pos] == "-":
            state = "q10"
        pos -= 1

    elif state == "q9":  # сложение для 2
        if tape[pos] == " ":
            tape[pos] = "2"
            state = "q12"
        else:
            if tape[pos] == "0":
                state = "q12"
            else:
                state = "q4"
            tape[pos] = str((int(tape[pos]) + 2) % 3)
            pos -= 1

    elif state == "q10":  # вычитание для 2
        if tape[pos] == "2":
            tape[pos] = "0"
            state = "q6"
        else:
            state = "q5"
            tape[pos] = str(int(tape[pos]) + 1)
            pos -= 1

    elif state == "q11":  # удаление незначащих 0
        if tape[pos] == "0":
            tape[pos] = " "
            pos += 1
        elif tape[pos] == "+" or tape[pos] == "-":
            state = "q7"
            pos -= 1
        else:
            state = "q12"

res += "q12"
print(''.join(tape))
