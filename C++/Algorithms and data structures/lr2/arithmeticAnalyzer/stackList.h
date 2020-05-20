#ifndef STACKLIST_H
#define STACKLIST_H

#include "basicheaders.h"
#include "expstr.h"

template <typename StackType>
class Stack
{
private:
    size_t top;                     // номер текущего элемента в стеке
    std::vector<StackType> data;    // иерархический список

public:
    Stack();
    ~Stack();

    inline void push(const StackType newEl);
    inline StackType pop();
    inline int getStackSize() const;
    inline int getTop() const;
    inline const StackType &peek(int) const;  // n-ый элемент от вершины стека
    inline void printStack();
};

/**
 * Для использования шаблона, компилятор должен видеть
 * как определение шаблона (а не только объявление),
 * так и тип шаблона,
 * используемый для создания экземпляра шаблона.
 */

template <typename StackType>
Stack<StackType>::Stack()
{
    top = 0;
}

template <class StackType>
Stack<StackType>::~Stack()
{
    data.clear();
    top = 0;
}

template <typename StackType>
inline void Stack<StackType>::push(StackType newEl)
{
    data.push_back(newEl);
    top++;
}

template <typename StackType>
inline StackType Stack<StackType>::pop()
{
    assert(top > 0);
    StackType tmp = data[getTop() - 1];
    data.erase(data.begin() + (--top));

    return tmp;
}

template <typename StackType>
inline const StackType &Stack<StackType>::peek(int num) const
{
    assert (num <= top);

    return data[top - num];
}

template <typename StackType>
inline void Stack<StackType>::printStack()
{
    for (int ix = top - 1; ix >= 0; ix--)
        std::cout << '|' << data[ix] << std::endl;
}

template <typename StackType>
inline int Stack<StackType>::getStackSize() const
{
    return data.size();
}

template <typename StackType>
inline int Stack<StackType>::getTop() const
{
    return top;
}

#endif // STACKLIST_H
