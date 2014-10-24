// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include "key.h"
#include <fstream>
#include "keygenerator.h"

namespace otp
{

Key::Key(const std::string& filename):
    filename(filename),
    usedBlocks(filename)
{
}

Position Key::encrypt(std::vector<char>& data)
{
    auto position = usedBlocks.getFreePosition(data.size());
    return encrypt(data, position);
}

Position Key::encrypt(std::vector<char>& data, Position pos)
{
    // Open the key file at the correct position
    std::fstream keyFile(filename.c_str(), std::ios::binary);
    keyFile.seekg(pos);

    // Read the needed portion of the key file
    std::vector<char> keyData(data.size());
    keyFile.read(keyData.data(), keyData.size());

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
    usedBlocks.markAsUsed(pos, keyData.size());

    // Return the position used
    return pos;
}

Position Key::bytesTotal() const
{
    return fileSize;
}

Position Key::bytesUsable() const
{
    // freeSpace = fileSize - usedBlocks.getSize();
    return freeSpace;
}

Position Key::bytesTotalRange() const
{
    return (right - left);
}

Position Key::bytesUsableRange() const
{
    return (bytesTotalRange() - usedBlocks.getSize(left, right));
}

void Key::setKeyRange(Position minimum, Position maximum)
{
    left = minimum;
    right = maximum;
}


}
