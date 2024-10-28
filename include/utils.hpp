#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>

std::string cleanString(const std::string& input) {
    std::string cleaned = input;

    cleaned.erase(cleaned.begin(), std::find_if(cleaned.begin(), cleaned.end(), [](unsigned char c) {
        return !std::isspace(c);
    }));

    cleaned.erase(std::find_if(cleaned.rbegin(), cleaned.rend(), [](unsigned char c) {
        return !std::isspace(c);
    }).base(), cleaned.end());

    return cleaned;
}


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


// KMP - next
std::vector<int> buildNext(const std::string& P) {
    int m = P.size();
    std::vector<int> next(m, -1);
    int j = 0;
    int t = next[0] = -1; // 初始化 next[0] = -1

    while (j < m - 1) {
        if (0 > t || P[j] == P[t]) {
            ++j;
            ++t;
            next[j] = (P[j] != P[t]) ? t : next[t]; // 避免重复比较
        } else {
            t = next[t]; // 若不匹配则回溯
        }
    }
    return next;
}

// KMP
int matchKMP(const std::string& P, const std::string& T) {
    std::vector<int> next = buildNext(P);
    int n = T.size(), i = 0;
    int m = P.size(), j = 0;
    while (j < m && i < n) {
        if (0 > j || T[i] == P[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    return (j == m) ? (i - j) : -1; // 匹配成功则返回起始位置，否则返回 -1
}


void displayMatrix(const std::vector<std::vector<int>>& matrix) {
    displayLine(1);
    for (const auto& row : matrix) {
        for (const auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
    displayLine(1);
}

#endif