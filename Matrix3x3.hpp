
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
