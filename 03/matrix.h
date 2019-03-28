class Matrix {
    class Columns;
    int rows;
    int cols;
    Columns *matrix;
    
    class Columns {
    public:
        int *column;
        int cols;

        Columns() {}
        Columns(int size) : cols(size) {
            column = new int[cols];
        }

        int& operator[](int index) const {
            if (index >= cols)
                throw std::out_of_range("");
            return column[index];
        }
   
        
    };
    

public:

    Matrix(const int r, const int c) : rows(r), cols(c) {
        matrix = new Columns[rows];
        for (int i = 0; i < rows; i++)
            matrix[i] = Columns(cols);
        
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
        if (rows != m.rows || cols != m.cols)
            return false;
        
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (matrix[i][j] != m.matrix[i][j])
                    return false;
        return true;
    }
    
    bool operator!=(const Matrix& m) const {
        if (rows != m.rows || cols != m.cols)
            return true;
        
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (matrix[i][j] != m.matrix[i][j])
                    return true;
        return false;
    }
    
    Matrix& operator*=(int n) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i][j] *= n;
        
        
        return *this;
        
    }
    
    ~Matrix() {
        for (int i = 0; i < rows; i++)
            delete[] matrix[i].column;
        
        delete[] matrix;
        
    }
    
    
    
};
