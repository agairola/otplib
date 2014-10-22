// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef RANDNUMGEN_H
#define RANDNUMGEN_H

#include <random>

namespace otp
{

/*
This class generates secure random numbers. For now, just integers.
*/
class RandNumGen
{
    public:
        // Sets the range of numbers to generate, also gets entropy
        RandNumGen(int minimum, int maximum);

        // Returns a random number in the specified range
        int generate();

        // Same as above, but casts an int to a specified type
        template <typename Type>
        Type generate();

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<> dist;
};

template <typename Type>
Type RandNumGen::generate()
{
    return static_cast<Type>(dist(gen));
}

}

#endif
