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

#pragma once

// Define a 3x3 matrix type
struct Matrix3x3 {
    float data[9];  // Stored in row-major order
    Matrix3x3();
    
    Matrix3x3(float a11, float a12, float a13,
              float a21, float a22, float a23, 
              float a31, float a32, float a33);
    
    // Print a 3x3 matrix
    void print();

    /* Multiple A * B and store result back in A */
    void multiply(const Matrix3x3& B);

    /* Access the matrix elements */
    float& operator[](int i);
    const float& operator[](int i) const;
};
