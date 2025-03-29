/*
 *   Copyright (c) 2025 Jawaid Bazyar

 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <cstdio>
#include <cstring>
#include "Matrix3x3.hpp"


Matrix3x3::Matrix3x3() {
    // Initialize to identity matrix
    for (int i = 0; i < 9; i++) {
        data[i] = (i % 4 == 0) ? 1.0f : 0.0f;
    }
}

Matrix3x3::Matrix3x3(float a11, float a12, float a13,
            float a21, float a22, float a23, 
            float a31, float a32, float a33) {
    data[0] = a11; data[1] = a12; data[2] = a13;
    data[3] = a21; data[4] = a22; data[5] = a23;
    data[6] = a31; data[7] = a32; data[8] = a33;
}

// Print a 3x3 matrix
void Matrix3x3::print() {
    for (int i = 0; i < 3; i++) {
        printf("[%8.4f %8.4f %8.4f]\n", 
            data[i*3 + 0], 
            data[i*3 + 1], 
            data[i*3 + 2]);
    }
}

float& Matrix3x3::operator[](int i) { return data[i]; }
const float& Matrix3x3::operator[](int i) const { return data[i]; }

// Multiply two 3x3 matrices, storing result back in A
void Matrix3x3::multiply(const Matrix3x3& B) {
    //Matrix3x3 A = *this;
    
    // Create a temporary array to store the result
    float temp[9] = {0}; // Initialize to zero
    
    // Matrix multiplication: temp = B * A
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                // Match the OEMatrix3 implementation
                temp[i * 3 + j] += B.data[i * 3 + k] * data[k * 3 + j];
            }
        }
    }
    
    // Copy the result back to A
    /* for (int i = 0; i < 9; i++) {
        this->data[i] = temp[i];
    } */
    std::memcpy(data, temp, 9 * sizeof(float));
    this->print();
}

