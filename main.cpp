// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#include <iostream>
#include "key.h"
#include "keygenerator.h"

void printBufferInHex(const std::vector<char>& data);

void genKeyFile();

int main()
{
//    genKeyFile();
//    return 0;

    std::cout << "OTPlib Test v1.0.0\n";
    std::cout << "Enter filename of key file: ";
    std::string filename;
    std::cin >> filename;
    otp::Key keyFile(filename);

    std::cout << "Enter some text:\n";
    std::string text;
    while (getline(std::cin, text))
    {
        std::vector<char> data(text.begin(), text.end());
        keyFile.encrypt(data);
        printBufferInHex(data);
    }
    return 0;
}

void printBufferInHex(const std::vector<char>& data)
{
    for (char c: data)
    {
        std::cout << std::hex << c;
    }
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
