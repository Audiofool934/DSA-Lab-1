#include <iostream>
#include <vector>

using std::vector;

template<typename T>
class TripletList {
private:
    struct Triplet {
        int row, col;
        T value;
        Triplet(int r, int c, T v) : row(r), col(c), value(v) {}
    };

    vector<Triplet> triplets;
    int numRows, numCols;

public:
    TripletList();
    TripletList(const vector<vector<T>> &inArray);
    vector<Triplet> getTriblets() { return triplets; }
    TripletList<T> transpose();
    TripletList<T> multiplyMatrix(const TripletList<T>& mat);
    vector<vector<T>> transformTo2DArray();
    void printMatrix();
    void makeEmpty();
};

template<typename T>
TripletList<T>::TripletList() : numRows(0), numCols(0) {}

template<typename T>
TripletList<T>::TripletList(const vector<vector<T>>& inArray) : numRows(inArray.size()), numCols(inArray.size() > 0 ? inArray[0].size() : 0) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (inArray[i][j] != T(0)) {
                triplets.emplace_back(i, j, inArray[i][j]); // 直接在 triplets 向量中构造 Triplet 对象
            }
        }
    }
}

template<typename T>
TripletList<T> TripletList<T>::transpose() {
    TripletList<T> transposedMatrix;
    transposedMatrix.numRows = numCols;
    transposedMatrix.numCols = numRows;

    for (const auto& triplet : triplets) {
        transposedMatrix.triplets.emplace_back(triplet.col, triplet.row, triplet.value);
    }

    return transposedMatrix;
}

template<typename T>
TripletList<T> TripletList<T>::multiplyMatrix(const TripletList<T>& otherMatrix) {
    if (numCols != otherMatrix.numRows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
    }

    TripletList<T> resultMatrix;
    resultMatrix.numRows = numRows;
    resultMatrix.numCols = otherMatrix.numCols;

    for (const auto& tripletA : triplets) {
        for (const auto& tripletB : otherMatrix.triplets) {
            if (tripletA.col == tripletB.row) {
                bool found = false;
                for (auto& tripletC : resultMatrix.triplets) {
                    if (tripletC.row == tripletA.row && tripletC.col == tripletB.col) {
                        tripletC.value += tripletA.value * tripletB.value;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    resultMatrix.triplets.emplace_back(tripletA.row, tripletB.col, tripletA.value * tripletB.value);
                }
            }
        }
    }

    return resultMatrix;
}

template<typename T>
vector<vector<T>> TripletList<T>::transformTo2DArray() {
    vector<vector<T>> result(numRows, vector<T>(numCols, T(0)));

    for (const auto& triplet : triplets) {
        result[triplet.row][triplet.col] = triplet.value;
    }

    return result;
}

template<typename T>
void TripletList<T>::printMatrix() {
    vector<vector<T>> result = transformTo2DArray();
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void TripletList<T>::makeEmpty() {
    triplets.clear();
    numRows = 0;
    numCols = 0;
}
