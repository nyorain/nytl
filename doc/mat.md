
The Matrix type concept:

```cpp
struct Matrix {
public:
	using Size = ...; // usually std::size_t. Must be convertible from/to int.
	using Value = ...; // value type, mathematical field
	using RowType = ...; // Vector type able to hold one row of this matrix
	using ColType = ...; // Vector type able to hold one column of this matrix

	// Rebinds the Matrix implementation
	template<Size MaxR, Size MaxC, typename T> using Rebind = ...;

	// dimensions of the matrix. Might be symbolic constants
	static constexpr Size rowDim = ..;
	static constexpr Size colDim = ..;

	// creates a matrix for the given rows and cols.
	static Matrix create(Size rows, Size cols);

public:
	// matrix[r][c] must return a reference to the value of matrix in row r and column c.
 	auto operator[](Size); // must return some kind of referencing vector.
 	const auto operator[](Size) const; // must return some kind of referencing vector.

	Size rows() const; // the number of rows
	Size cols() const; // the number of columns
};

auto operator*(Value, Matrix);
auto operator*(Matrix, Matrix); // default matrix multiplication
auto operator*(Matrix, Vector); // multiplication of matrix with vector
auto operator*(Vector, Matrix); // multiplication of vector with 1-row matrix
auto operator+(Matrix, Matrix);
auto operator-(Matrix, Matrix);
bool operator==(Matrix, Matrix);
bool operator!=(Matrix, Matrix);

```

__NOTE__: As specified in the Vector concept, vectors are interpreted column vectors.
The matrix vector multiplication operators must be implemented this way.
Invalid operations (such as multiply 3x2 matrix with 4x4 matrix) should not be implemented
or throw an exception.

For an example Matrix implementation see nytl/mat.
