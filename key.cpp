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

Status Key::encrypt(std::vector<char>& data)
{
    Status position;

    // Don't try to encrypt 0 bytes
    if (!data.empty())
    {
        // Allocate a range of bytes to use
        position = usedBlocks.allocate(data.size(), left, right);

        // Encrypt the data if there was enough space
        if (position.status)
            position = encrypt(data, position.position, false);
    }

    return position;
}

Status Key::encrypt(std::vector<char>& data, Position pos, bool needToAllocate)
{
    Status position;

    // Don't try to encrypt 0 bytes
    if (!data.empty())
    {
        // Open the key file for reading and writing
        std::fstream keyFile(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
        if (keyFile)
        {
            // Read/write at the specified position
            keyFile.seekg(pos);
            keyFile.seekp(pos);

            // Read the needed portion of the key file
            std::vector<char> keyData(data.size());
            keyFile.read(keyData.data(), keyData.size());

            if (keyFile)
            {
                // Encrypt the data using the key data
                unsigned i = 0;
                for (char& c: data)
                    c ^= keyData[i++];

                // Generate new random data
                KeyGenerator::generate(keyData, keyData.size());

                // Overwrite the old key data with the new random data
                keyFile.write(keyData.data(), keyData.size());

                // Try to immediately write the data
                keyFile.flush();
                keyFile.close();

                // Save the used range of data
                if (needToAllocate)
                    usedBlocks.markAsUsed(pos, keyData.size());

                position.status = true;
            }
        }
    }
    // Return the position used, and if it was successful
    position.position = pos;
    return position;
}

Key::operator bool() const
{
    return (usedBlocks.getSize() > 0);
}

Position Key::bytesTotal() const
{
    return usedBlocks.getSize();
}

Position Key::bytesFree() const
{
    return usedBlocks.getFreeSpace();
}

Position Key::bytesUsed() const
{
    return (bytesTotal() - bytesFree());
}

Position Key::bytesTotalRange() const
{
    return (right - left + 1);
}

Position Key::bytesFreeRange() const
{
    return usedBlocks.getFreeSpace(left, right);
}

Position Key::bytesUsedRange() const
{
    return (bytesTotalRange() - bytesFreeRange());
}

void Key::setKeyRange(Position minimum, Position maximum)
{
    left = minimum;
    right = maximum;
}

}
