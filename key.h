// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef KEY_H
#define KEY_H

#include <cstddef>
#include <string>
#include <vector>

namespace otp
{

/*
TODO:
    Keep track of used blocks.
        They would still be securely erased, but the file wouldn't get truncated.
        This would solve the issue of a networked application with multiple
            clients accessing the same parts of the file, like in OTP Chat.
        This data could just be stored in another file next to the key file.
            That way, the key files can still be easily generated.
            This file could contain a list of the ranges of blocks used.
        The networked software would need to agree on which portions are allowed to be used.
            This Key class would need to allow specifying a position/range to use to encrypt the data.
            Encrypting should return a new position of available data.
        With blocks, the key could be used up in left-right order.
            This will allow for custom positions without worrying about size.
    Do some low-level verification of the secure erase algorithms, to make sure common
        file systems aren't reallocating the blocks being overwritten.
*/

/*
File layout/design

key
    Random data
    Used blocks are securely erased with random data
    This key file could be safely reused, by deleting the .index file and re-syncing.
        However, it increases the chances of someone obtaining your common unused key data.

key.index
    List of all used blocks (ranges of bytes)
        Using ranges of bytes instead of fixed block sizes to prevent wasting valuable key data.
    2345-2371
    100-150
    0-20
    9999-10000
    Ranges could be merged once loaded into memory and when the index file is rewritten.
        0-50
        51-100
        101-200
    Would turn into:
        0-200

*/

/*
This class reads from a key file of (ideally) random data.
Everytime something is encrypted with a portion of the key file, that part
    gets securely erased, and then truncated.
*/
class Key
{
    public:
        using Position = std::size_t;

        Key();
        Key(const std::string& filename);
        virtual ~Key();

        // Encryption ---------------------------------------------------------

        // Encrypts (or decrypts) a buffer of data in place
        void encrypt(std::vector<char>& data);

        // Encrypts a plain-text message, and returns a buffer
        std::vector<char> encryptMessage(const std::string& message);


        // Status -------------------------------------------------------------

        // Returns the size of the entire key file
        Position bytesTotal() const;

        // Returns how much more of the key file can be used
        Position bytesUsable() const;

        // Returns the size of the range
        Position bytesTotalRange() const;

        // Returns the available bytes in the current range
        Position bytesUsableRange() const;

        // Positions/blocks ---------------------------------------------------

        // Set the range of key data to use (default is entire file)
        // This is necessary so that multiple things don't use the same key data
        // (maximum == 0 || minimum >= maximum) means the maximum will be at
        //     the end of the file.
        void setKeyRange(Position minimum = 0, Position maximum = 0);

    private:
        std::string filename; // Name of the key file
        BlockTracker usedBlocks; // List of already used blocks that cannot be reused

        // Range of bytes to use
        Position left;
        Position right;
};

}

#endif
