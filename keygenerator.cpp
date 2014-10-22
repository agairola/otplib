// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "keygenerator.h"
#include "randnumgen.h"
#include <fstream>

namespace otp
{

void KeyGenerator::generate(std::vector<char>& data, std::size_t amount)
{
    // Setup a random number generator for random characters
    RandNumGen numberGenerator(0, 255);

    // Setup the buffer
    data.clear();
    data.resize(amount);

    // Generate the random data
    for (char& c: data)
        c = numberGenerator.generate<char>();
}

bool KeyGenerator::generateToFile(const std::string& filename, std::size_t amount)
{
    // TODO: Generate small blocks at a time so it doesn't use as much memory

    bool status = false;

    // Open the output file
    std::ofstream outputFile(filename.c_str(), std::ios::binary);
    if (outputFile)
    {
        // Generate to a buffer
        std::vector<char> data;
        generate(data, amount);

        // Save to the file
        outputFile.write(data.data(), data.size());
        outputFile.close();
        status = true;
    }

    return status;
}

}
