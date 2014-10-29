// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include <iostream>
#include "key.h"
#include "keygenerator.h"

void genKeyFile();
void printBufferInHex(const std::vector<char>& data);

int main()
{
//    genKeyFile();
//    return 0;

    std::cout << "OTPlib Test v1.0.2\n";
    std::cout << "Enter filename of key file: ";
    std::string filename;
    getline(std::cin, filename);
    otp::Key keyFile(filename);
    if (keyFile)
    {
        //keyFile.setKeyRange(0, 200);
        std::cout << "Key file \"" << filename << "\" loaded.\n";
        std::cout << "Space free: " << keyFile.bytesFree() << " bytes\n";
        std::cout << "Space used: " << keyFile.bytesUsed() << "/" << keyFile.bytesTotal() << " bytes\n";
        std::cout << "Space free (range): " << keyFile.bytesFreeRange() << " bytes\n";
        std::cout << "Space used (range): " << keyFile.bytesUsedRange() << "/" << keyFile.bytesTotalRange() << " bytes\n";

        std::cout << "\nEnter text: ";
        std::string text;
        while (getline(std::cin, text))
        {
            std::vector<char> data(text.begin(), text.end());
            std::cout << "Original: ";
            printBufferInHex(data);

            // Encrypt the user's input
            auto position = keyFile.encrypt(data);
            if (position.status)
            {
                std::cout << "Encrypted: ";
                printBufferInHex(data);
                std::cout << "Key file position: " << std::dec << position.position << "\n\n";
            }
            else
                std::cout << "Error: Failed to encrypt. The key file is depleted or invalid.\n\n";
            std::cout << "Enter text: ";
        }
    }
    else
        std::cout << "Error loading key file \"" << filename << "\".\n";

    return 0;
}

void genKeyFile()
{
    std::cout << "Key file generator test\nEnter filename and size: ";
    std::string filename;
    std::size_t amount;
    std::cin >> filename >> amount;
    std::cout << "Generating...";
    otp::KeyGenerator::generateToFile(filename, amount);
    std::cout << " Done.\n";
}

void printBufferInHex(const std::vector<char>& data)
{
    for (char c: data)
        std::cout << std::hex << (int)((unsigned char)c) << ' ';
    std::cout << "\n";
}
