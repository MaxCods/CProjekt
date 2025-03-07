# 📌 **myfind** - A Custom File Search Utility

## 📖 Project Description

**myfind** is a custom implementation of the find command in Linux. It allows users to search for files and directories based on specified criteria, such as name patterns. The program supports options like:


`-name <pattern>`: Search for files matching a pattern.

`-R / -r`: Enable recursive search.

`-a`: show hidden files

`-t`:  use multiple threads

`-type`: filter by file type

`-size SIZE`: filter by file size

`-mtime DAYS`: filter by last modification time

`-perm MODE`: filter by file permissions

`-l`: show detailed information

`-o FILE` output the results to the specified FILE

`-h`: Display help information.

The program is designed to be efficient and supports **multithreading** for performance optimization.


## 📋 System Requirements

To compile and run myfind, you need:

- GCC (GNU Compiler Collection)

- make utility

- A POSIX-compliant operating system (Linux/macOS)

To install the necessary tools on Debian-based systems:

```bash
sudo apt update && sudo apt install gcc make
```

## ⚙️ Build Instructions

To compile the project, navigate to the project directory and run:

```bash
make all
```

This will generate the executable file myfind in the project directory.


## 🚀 Running the Program

Once compiled, myfind can be executed as follows:

search exact name 
```bash
./myfind -name beispiel.txt
```

search with name pattern:
```bash
./myfind -name "*.txt"
```

Example with recursive search with name pattern:

```bash
./myfind -R -name "*findme*"
```

Example with output to a file:
```bash
./myfind -r -o output.txt
```

Example for searching only directories:
```bash
./myfind -type d
```

Example for searching only files:
```bash 
./myfind -type f
```
Example for searching with more information:
```bash
./myfind -l
``` 

For help on available options:

```bash
./myfind -h
```

## 🛠 Running Test Cases

If test cases are included, they can be executed using:

```bash
make test
```

(Ensure that the **Makefile has a test rule.**)


## 🧹 Cleaning Up

To remove compiled files and the executable:

```bash
make clean
```

## 📂 Project Structure

```md
├── myfind.c      # Main program logic
├── search.c      # Search implementation
├── queue.c       # Directory handling data structures
├── library.c     # Utility functions
├── myfind.h      # Function definitions
├── Makefile      # Build script
└── README.md     # Documentation
```
## ⚠️ Notes

The program is designed for **UNIX-like operating systems**.

Future improvements could include additional filtering options and performance enhancements.

### 📌 Developed for educational purposes. Contributions and feedback are welcome!
