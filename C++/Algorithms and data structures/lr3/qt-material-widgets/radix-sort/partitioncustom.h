#ifndef PARTITTIONCUSTOM_H
#define PARTITTIONCUSTOM_H

#include "basicheaders.h"

class radix_test
{
    const int bit; // bit position [0..31] to examine

    public:
        radix_test(int offset) : bit(offset) {} // constructor

        bool operator()(int value) const // function call operator
        {
            if (bit == 31) // sign bit
                return value < 0; // negative int to left partition
            else
                return !(value & (1 << bit)); // 0 bit to left partition
        }
};

template<typename ForwardIterator, typename UnaryPredicate>
ForwardIterator partitionCustom(ForwardIterator first, ForwardIterator last, \
                                UnaryPredicate pred, int& operationCount) {
    // look for the first element that does not satisfy the predicate
    while (first != last && pred(*first))
    {
        ++first;  // algorithm std::find_if_not
        operationCount++;
    }

    if (first == last) return first;  // all elements satisfy the predicate

    // first indicates the end of the sequence pred(x) == true
    for (auto i = std::next(first); i != last; ++i)
    {
        operationCount++;
        // the element satisfies the predicate
        if (pred(*i))
        {
            // move it to the end of the sequence pred(x) == true
            std::iter_swap(first, i);
            ++first;
            operationCount++;
        }
    }

    return first;
}

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator stablePartitionCustom(ForwardIterator begin, ForwardIterator end, \
                                 UnaryPredicate p, int& operationCount)
{
    using val = typename std::iterator_traits<ForwardIterator>::value_type;
    std::vector<val> failed;

    ForwardIterator dest = begin;

    for (ForwardIterator src = begin; src != end; ++src)
    {
        operationCount++;
        if (p(*src))
            * dest++ = std::move(*src);
        else
            failed.push_back(std::move(*src));
    }
    auto ret = dest;
    for (auto const& f : failed)
    {
        operationCount++;
        * dest++ = f;
    }
    return ret;
}

#endif // PARTITTIONCUSTOM_H
