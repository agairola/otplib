// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <vector>
#include <string>

namespace otp
{

/*
Future improvements:
    Take input from stdin, which could be other sources of random data,
        like /dev/urandom. This input would be XORed with the regular
        algorithm's output.
*/

/*
This class uses the C++11 secure psuedorandom number generators to generate
    random key data, which can be saved to a buffer or a file.
The main purpose is to be used with OTP implementations.
*/
class KeyGenerator
{
    public:

        // Generate random key data to a buffer
        static void generate(std::vector<char>& data, std::size_t amount);

        // Generate random key data to a file (returns true if successful)
        static bool generateToFile(const std::string& filename, std::size_t amount);
};

}

#endif
