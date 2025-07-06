# BigIntegerOperations

A C++ library for performing **arbitrary-precision integer arithmetic**, designed for use in **Data Structures and Algorithms (DSA)** projects or competitive programming environments.

---

## ✨ Features

- ✅ Arbitrary-size integers (`BigInteger`)
- ✅ Supports negative numbers
- ✅ Input expressions like `2 - 32`, `-100 + 200` auto-parsed
- ✅ Core arithmetic:
  - ➕ Addition
  - ➖ Subtraction
  - ✖️ Multiplication
  - ➗ Division
  - 🧮 Modulus
- ✅ Comparisons: `<`, `>`, `==`, etc.
- ✅ Clean object-oriented design
- ✅ Easy to extend with exponentiation, factorial, etc.

---

## 🧱 Folder Structure

```
BigIntegerOperations/
├── include/
│   └── BigInteger.h # Main header
├── src/
│   ├── BigInteger.cpp # Implementation
│   └── main.cpp # User input + demo
├── CMakeLists.txt # CMake support
├── Build.bat # Windows one-click build + run
├── tests/ # (optional future tests)
└── README.md
```

---

## 🧪 Sample Input & Output

```txt
Enter first large integer or expression (a): 2 - 32
Enter second large integer or expression (b): 1000000000000000000000000 + 500

a + b = 999999999999999999999970
b - a = 1000000000000000000000532
a * b = -32000000000000000000000000
b / a = -31250000000000000000000
b % a = 0
```

---

## 🚀 Build Instructions

### ✅ Option 1: Build with CMake (Cross-platform)

```sh
git clone https://github.com/yourname/BigIntegerOperations.git
cd BigIntegerOperations
mkdir build
cd build
cmake ..
cmake --build .
```

Then run:

```sh
./main
```

### ✅ Option 2: Build with Batch (Windows only)

Double-click `Build.bat` or run:

```cmd
Build.bat
```

---

## 🛠 Usage in Your Project

Include the header and link against the compiled .lib / .a or source:

```cpp
#include "BigInteger.h"

BigInteger a("123456789123456789");
BigInteger b("-987654321987654321");

BigInteger sum = a + b;
std::cout << "Sum = " << sum << '\n';
```

---

## 🧪 Testing (Coming Soon)

Tests will be added under `tests/`. Once available:

```sh
cd build
ctest
```

---

## 🤝 Contributing

Pull requests are welcome! For major changes, open an issue to discuss them first.

- Clean, readable C++14+ code
- Avoid STL features that break determinism or increase complexity
- Add unit tests for new operations

---

## 📄 License

This project is licensed under the MIT License — free to use, modify, and distribute.

---

## 🙏 Acknowledgements

Thanks to everyone exploring DSA and low-level arithmetic — you're building your own compiler-level logic! 🔢⚙️ 