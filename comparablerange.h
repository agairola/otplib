// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef COMPARABLERANGE_H
#define COMPARABLERANGE_H

#include <string>

template <typename Type>
class ComparableRange
{
public:
    ComparableRange():
        minimum(0),
        maximum(0)
    {
    }

    ComparableRange(Type minimum, Type maximum):
        minimum(minimum),
        maximum(maximum)
    {
    }

    Type getSize() const
    {
        return (maximum - minimum);
    }

    Type getSize(Type left, Type right) const
    {
        left = std::max(minimum, left);
        right = std::min(maximum, right);
        return (right - left);
    }

    bool operator<(const ComparableRange& rhs) const
    {
        return (minimum < rhs.minimum);
    }

    bool operator==(Type num) const
    {
        return (minimum <= num && num <= maximum);
    }

    std::string toString() const
    {
        return (std::to_string(minimum) + ' ' + std::to_string(maximum));
    }

    Type minimum;
    Type maximum;
};

#endif
