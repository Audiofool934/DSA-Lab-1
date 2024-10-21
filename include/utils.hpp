#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>

void displayLine(int mode) {
    switch (mode) {
        case 1:
            std::cout << std::string(80, '-') << std::endl;
            break;
        case 2:
            std::cout << std::string(80, '=') << std::endl;
        default:
            break;
    }
}

void displayDots() {
    std::cout << std::left
            << std::setw(10) << "..."
            << std::setw(13) << "..."
            << std::setw(13) << "..."
            << std::setw(30) << "..."
            << std::setw(10) << "..."
            << std::setw(10) << "..."
            << std::endl;
}

void displayTitle() {
    std::cout << std::left
            << std::setw(10) << "PatentID"
            << std::setw(13) << "Grant Date"
            << std::setw(13) << "Appl Date"
            << std::setw(30) << "Title"
            << std::setw(10) << "Country"
            << std::setw(10) << "FirmID"
            << std::endl;
    displayLine(1);
}

#endif