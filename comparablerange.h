// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef COMPARABLERANGE_H
#define COMPARABLERANGE_H

#include <string>

namespace otp
{

/*
A simple class that holds a minimum and maximum value.
Its purpose is to keep track of free block ranges in the OTP library.
*/
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

    // Returns the size of the range
    Type getSize() const
    {
        return (maximum - minimum + 1);
    }

    // Returns the size of the range with boundaries
    Type getSize(Type left, Type right) const
    {
        left = std::max(minimum, left);
        right = std::min(maximum, right);
        Type size = 0;
        // Makes sure the ranges are overlapping to prevent getting a negative result
        if (right >= left)
            size = (right - left + 1);
        return size;
    }

    // Compares the minimum value with another range
    bool operator<(const ComparableRange& rhs) const
    {
        return (minimum < rhs.minimum);
    }

    // Checks if a number is within the range
    bool operator==(Type num) const
    {
        return (minimum <= num && num <= maximum);
    }

    // Used for writing to a file
    std::string toString() const
    {
        return (std::to_string(minimum) + ' ' + std::to_string(maximum));
    }

    Type minimum;
    Type maximum;
};

}

#endif
