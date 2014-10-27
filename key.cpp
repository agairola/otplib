// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "key.h"
#include <fstream>
#include <iostream>
#include "keygenerator.h"

namespace otp
{

Key::Key(const std::string& filename):
    filename(filename),
    usedBlocks(filename)
{
    left = 0;
    right = usedBlocks.getSize() - 1;
}

Position Key::encrypt(std::vector<char>& data)
{
    auto position = usedBlocks.allocate(data.size(), left, right);
    return encrypt(data, position, false);
}

Position Key::encrypt(std::vector<char>& data, Position pos, bool needToAllocate)
{
    // Open the key file at the correct position
    std::fstream keyFile(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    keyFile.seekg(pos);

    // Read the needed portion of the key file
    std::vector<char> keyData(data.size());
    keyFile.read(keyData.data(), keyData.size());

    // Debugging code
    std::cout << std::dec << "\nEncrypting " << keyData.size() << " bytes at " << pos << "...\n\n";
    std::cout << "Original:  ";
    printBufferInHex(data);
    std::cout << "Key:       ";
    printBufferInHex(keyData);

    // Encrypt the data using the key data
    unsigned i = 0;
    for (char& c: data)
        c ^= keyData[i++];

    // Overwrite the key data with new random data
    KeyGenerator::generate(keyData, keyData.size());

    // Save the key data back to the file
    keyFile.seekp(pos);
    keyFile.write(keyData.data(), keyData.size());
    keyFile.close();

    // Save the used range of data
    if (needToAllocate)
        usedBlocks.markAsUsed(pos, keyData.size());

    // Return the position used
    return pos;
}

Position Key::bytesTotal() const
{
    return usedBlocks.getSize();
}

Position Key::bytesUsable() const
{
    return usedBlocks.getFreeSpace();
}

Position Key::bytesTotalRange() const
{
    return (right - left);
}

Position Key::bytesUsableRange() const
{
    return usedBlocks.getFreeSpace(left, right);
}

void Key::setKeyRange(Position minimum, Position maximum)
{
    left = minimum;
    right = maximum;
}

void printBufferInHex(const std::vector<char>& data)
{
    for (char c: data)
        std::cout << std::hex << (int)((unsigned char)c) << ' ';
    std::cout << "\n";
}

}
