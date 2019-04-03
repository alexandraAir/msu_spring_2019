#include <new>

class Matrix {
    class Columns;
    int rows;
    int cols;
    Columns *matrix;
    
    class Columns {
    public:
        int *column;
        int cols;
        
        Columns(int size) : cols(size) {
            column = new int[cols];
            
        }
        
        const int& operator[](int index) const {
            if (index >= cols)
                throw std::out_of_range("");
            return column[index];
        }
        
        int& operator[](int index) {
            if (index >= cols)
                throw std::out_of_range("");
            return column[index];
        }
        
        
    };
    
    bool check(const Matrix& m) const {
        if (rows != m.rows || cols != m.cols)
            return false;
        
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (matrix[i][j] != m.matrix[i][j])
                    return false;
        return true;
        
    }
    
    
public:
    
    Matrix(const int r, const int c) : rows(r), cols(c) {
        matrix = static_cast<Columns*>(operator new[] (sizeof(Columns) * rows));
        for (int i = 0; i < rows; i++)
            new(matrix + i) Columns(cols);
        
    }
    
    int getRows() const { return rows; }
    int getColumns() const { return cols; }
    
    Columns& operator[](int index) {
        if (index >= rows)
            throw std::out_of_range("");
        return matrix[index];
    }
    
    const Columns& operator[](int index) const {
        if (index >= rows)
            throw std::out_of_range("");
        return matrix[index];
    }
    
    
    bool operator==(const Matrix& m) const {
        return check(m);
    }
    
    bool operator!=(const Matrix& m) const {
        return !check(m);
    }
    
    Matrix& operator*=(int n) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i][j] *= n;
        
        
        return *this;
        
    }
    
    ~Matrix() {
        for (int i = 0; i < rows; i++)
            matrix[i].~Columns();
        
        operator delete[] (matrix);
        
    }
    
    
};

