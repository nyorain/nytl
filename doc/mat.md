The Matrix<T> type concept:

```cpp
struct Matrix {
public:
	using Size = ...; // usually std::size_t. Must be convertible from/to int.
	using Value = ...; // value type, mathematical field
	using RowType = ...; // Vector type able to hold one row of this matrix
	using ColType = ...; // Vector type able to hold one column of this matrix

	// Rebinds the Matrix implementation
	template<typename T> using Rebind = ...;

	// Whether the matrix is static sized
	static constexpr bool staticSized = ...;

	// if static sized:
	static constexpr Size rows(); // the number of rows
	static constexpr Size cols(); // the number of columns
	template<Size R, Size C> static Matrix create(); // creates a new matrix with given size

	// if not static sized:
	Size rows(); // the number of rows
	Size cols(); // the number of columns
	static Matrix create(Size rows, Size cols);

public:
	// returns a (const-)reference to [c, r]
	auto get(Size c, Size r) [const];

	// sets the value in [c, r]
	// can return void or a reference to the value
	// the third parameter may be a (const) reference
	auto set(Size c, Size r, Value value);

	// Like get, but throw when out of bounds
	auto at(Size r, Size c); // return value must be convertible to Value
	const auto at(Size r, Size c) const; // return value must be convertible to Value

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

optional (not used by nytl but should be implemented for consistency):

``` cpp
// matrix[r][c] returns a reference to the value of matrix in row r and column c.
auto operator[](Size); // must return some kind of referencing vector.
const auto operator[](Size) const; // must return some kind of referencing vector.
```

__NOTE__: As specified in the Vector concept, vectors are interpreted column vectors.
The matrix vector multiplication operators must be implemented this way.
Invalid operations (such as multiply 3x2 matrix with 4x4 matrix) should not be implemented
or throw an exception.

For an example Matrix implementation see nytl/mat.
