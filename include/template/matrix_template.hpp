#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

template<typename T>
class OrthList {
private:
    struct OrthAtom {
        int row, col;
        T ele;
        OrthAtom *rowNext, *colNext;
        OrthAtom() : row(0), col(0), ele(T(0)), rowNext(nullptr), colNext(nullptr) {}
    };

    vector<OrthAtom> rowDummy, colDummy;
    void insertNode(int row, int col, T value);

public:
    OrthList(); // Default constructor
    OrthList(const vector<vector<T>> &inArray);
    ~OrthList();
    OrthList<T> transpose();
    OrthList<T> multiplyMatrix(const OrthList<T>& mat);
    vector<vector<T>> transformTo2DArray();
    void printMatrix();
    void makeEmpty();
};

/* ------------------------------- 二维数组->十字链表 ------------------------------- */

template<typename T>
OrthList<T>::OrthList() : rowDummy(0), colDummy(0) {
    // Default constructor implementation
}

template<typename T>
OrthList<T>::OrthList(const vector<vector<T>>& inArray) : rowDummy(inArray.size()), colDummy(inArray.size() > 0 ? inArray[0].size() : 0) {
    if (inArray.size() > 0 && inArray[0].size() > 0) {
        for (int i = 0; i < int(inArray.size()); i++) {
            for (int j = 0; j < int(inArray[i].size()); j++) {
                if (inArray[i][j] != T(0)) {
                    insertNode(i, j, inArray[i][j]);
                }
            }
        }
    }
}

template<typename T>
void OrthList<T>::insertNode(int row, int col, T value) {
    OrthAtom* rowIter = &rowDummy[row];
    OrthAtom* colIter = &colDummy[col];
    
    // 找到非空元素的行、列位置
    while (rowIter->rowNext && rowIter->rowNext->col < col) rowIter = rowIter->rowNext;
    while (colIter->colNext && colIter->colNext->row < row) colIter = colIter->colNext;

    OrthAtom* newNode = new OrthAtom();
    // 0-based index
    newNode->row = row;
    newNode->col = col;
    newNode->ele = value;

    // 插入到行链表中
    newNode->rowNext = rowIter->rowNext;
    rowIter->rowNext = newNode;

    // 插入到列链表中
    newNode->colNext = colIter->colNext;
    colIter->colNext = newNode;
}

/* ---------------------------------- 矩阵转置 ---------------------------------- */

// template<typename T>
// void OrthList<T>::transposeMatrix() {
//     OrthAtom* cycleIter = nullptr; // 遍历迭代器
//     for (int i = 0; i < int(rowDummy.size()); i++) {
//         cycleIter = rowDummy[i].rowNext;
//         while (cycleIter != nullptr) {
//             OrthAtom* tmp = cycleIter;
//             cycleIter = cycleIter->rowNext;
//             // 交换下标和指针
//             std::swap(tmp->row, tmp->col);
//             std::swap(tmp->rowNext, tmp->colNext);
//         }
//         // 新的列指针指向原行的第一个非零元素
//         std::swap(rowDummy[i].rowNext, rowDummy[i].colNext);
//     }

//     for (int i = 0; i < int(colDummy.size()); i++) {
//         std::swap(colDummy[i].rowNext, colDummy[i].colNext);
//     }
// }
template<typename T>
OrthList<T> OrthList<T>::transpose() {
    OrthList<T> transposedMatrix;
    transposedMatrix.rowDummy.resize(colDummy.size());
    transposedMatrix.colDummy.resize(rowDummy.size());

    for (int i = 0; i < int(rowDummy.size()); i++) {
        OrthAtom* cycleIter = rowDummy[i].rowNext;
        while (cycleIter != nullptr) {
            transposedMatrix.insertNode(cycleIter->col, cycleIter->row, cycleIter->ele);
            cycleIter = cycleIter->rowNext;
        }
    }

    return transposedMatrix;
}

/* ---------------------------------- 矩阵相乘 ---------------------------------- */

template<typename T>
OrthList<T> OrthList<T>::multiplyMatrix(const OrthList<T>& otherMatrix) {

    if (colDummy.size() != otherMatrix.rowDummy.size()) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    OrthList<T> resultMatrix;
    resultMatrix.rowDummy.resize(rowDummy.size());
    resultMatrix.colDummy.resize(otherMatrix.colDummy.size());


    // 用于累积当前行的列结果
    vector<T> tmpAccumulate(otherMatrix.colDummy.size(), T(0));

    for (size_t i = 0; i < rowDummy.size(); i++) {
        std::fill(tmpAccumulate.begin(), tmpAccumulate.end(), T(0));

        // 获取当前行的第一个节点
        OrthAtom* firstIter = rowDummy[i].rowNext;

        while (firstIter != nullptr) {
            int rowIndex = firstIter->col;

            OrthAtom* secondIter = otherMatrix.rowDummy[rowIndex].rowNext;

            // 对应的 otherMatrix 行与当前行元素相乘并累加
            while (secondIter != nullptr) {
                tmpAccumulate[secondIter->col] += firstIter->ele * secondIter->ele;
                secondIter = secondIter->rowNext;
            }

            firstIter = firstIter->rowNext;
        }

        // 将非零的累积结果插入结果矩阵
        for (size_t j = 0; j < tmpAccumulate.size(); j++) {
            if (tmpAccumulate[j] != T(0)) {
                resultMatrix.insertNode(i, j, tmpAccumulate[j]);
            }
        }
    }

    return resultMatrix;
}

/* ------------------------------- 十字链表->二维数组 ------------------------------- */

template<typename T>
vector<vector<T>> OrthList<T>::transformTo2DArray() {
    vector<vector<T>> result(rowDummy.size(), vector<T>(colDummy.size(), T(0)));

    for (size_t i = 0; i < rowDummy.size(); i++) {
        OrthAtom* cycleIter = rowDummy[i].rowNext;
        while (cycleIter != nullptr) {
            result[cycleIter->row][cycleIter->col] = cycleIter->ele;
            cycleIter = cycleIter->rowNext;
        }
    }

    return result;
}

/* -------------------------------- 打印（二维数组） -------------------------------- */

template<typename T>
void OrthList<T>::printMatrix() {
    vector<vector<T>> result = transformTo2DArray();
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

/* ---------------------------------- 析构函数 ---------------------------------- */

template<typename T>
OrthList<T>::~OrthList() {
    for (size_t i = 0; i < rowDummy.size(); i++) {
        OrthAtom* current = rowDummy[i].rowNext;
        while (current != nullptr) {
            OrthAtom* toDelete = current;
            current = current->rowNext;
            delete toDelete;
        }
    }
}
