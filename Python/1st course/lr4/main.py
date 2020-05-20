# класс узла - контейнера, хранящего значение и ссылку на след/пред эл.
class Node:
    __data = None  # искажение имени (приватное поле)
    prev = None
    next = None

    def __init__(self, data, prev=None, next=None):
        self.__data = data
        self.prev = prev
        self.next = next

    def getData(self):
        return self.__data

    # переопределение метода пребразования объекта в строку [data: __, prev: __, next: __]
    def __str__(self):
        out_for_s = "data: " + str(self.getData())  # юзаем геттер
        out_for_prev = ", prev: " + (str(self.prev.getData()) if self.prev is not None else "None")
        out_for_next = ", next: " + (str(self.next.getData()) if self.next is not None else "None")
        return out_for_s + out_for_prev + out_for_next


'''
Каждый узел содержит элемент, ссылку на предыдущий и следующий узел. 
Связанный список состоит из последовательности узлов, 
каждый из которых предназначен для хранения объекта определенного при создании типа.
Также хранит свою длину.
'''


class LinkedList:
    __length = 0
    first = None
    last = None

    # переопределение метода получения длины
    def __len__(self):
        return self.__length

    def __init__(self, first=None, last=None):
        if first is None:
            if last is not None:
                raise ValueError("invalid value for last")
        else:
            f_node = Node(first)
            self.first = f_node
            if last is None:
                self.last = f_node  # список из одного элемента
                self.__length = 1
            else:
                l_node = Node(last, self.first)  # список двунаправный -> ссылка на пред.
                self.first.next = l_node  # ссылка на предыдущий эл.
                self.last = l_node
                self.__length = 2

    '''преобразование объекта к строковому представлению. 
    Вызывается, когда объект передается функциям print() и str()'''
    def __str__(self):
        res = []
        if len(self) == 0:
            return "LinkedList[]"
        else:
            now_el = self.first  # берём первый эл.
            while now_el != self.last:
                res.append(str(now_el))  # приводим элемент к строковому представлению
                now_el = now_el.next
            res.append(str(now_el))  # последний эл.
            return "LinkedList[length = " + str(len(self)) + ", [" + "; ".join(res) + "]]"

    # очистка списка
    def clear(self):
        self.first = None
        self.last = None
        self.__length = 0

    # метод добавления в список одного узлового эл. спереди
    def append(self, element):
        new_el = Node(element, self.last)  # передаём в генератор класса значение Data и ссылку на последний. эл.
        if len(self) == 0:
            self.first = new_el
        else:
            self.last.next = new_el  # ссылка последнего элемента на следующий
        self.last = new_el
        self.__length += 1

    # метод удаления одного узлового эл. спереди списка
    def pop(self):
        if len(self) == 0:
            raise IndexError("LinkedList is empty!")
        elif len(self) == 1:
            self.first.prev = None
            self.first.next = None
            self.__length -= 1
        elif len(self) > 1:
            self.last = self.last.prev  # перевешиваем последий элемент на предпоследний
            self.last.next = None  # новый последний эл. указывает на None
            self.__length -= 1

    def popitem(self, element):
        now_el = self.first
        if len(self) == 0:
            raise KeyError(str(element) + " doesn't exist!")
        # находим элемент с need Data
        while now_el.getData() != element:
            if now_el == self.last:
                raise KeyError(str(element) + " doesn't exist!")
            now_el = now_el.next
        if len(self) == 1:
            self.clear()
            self.__length = 0
        # если первый элемент содержит Data
        elif now_el == self.first:
            self.first = now_el.next  # перенавешиваем указатель на след. эл.
            self.first.prev = None  # у ного первого эл. указатель на предыдущий None
            self.__length -= 1
        # если последний элемент содержит Data
        elif now_el == self.last:
            self.last = self.last.prev  # перенавешиваем указатель на предыдущий эл.
            self.last.next = None  # у последнего элемента указатель на след. None
            self.__length -= 1
        else:
            now_el.prev.next = now_el.next  # указатель на следующий эл. предыдущего эл. перенавешиваем, минуя текущий
            now_el.next.prev = now_el.prev  # указатель на предыдущий эл. след. эл перенавешиваем , минуя текущий
            self.__length -= 1
