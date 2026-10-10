# 🐍 Snake Clone (SDL3, C++)

You play as a snake, whose main objective
is eating apples and growing in size!
But be aware of the rocks &mdash; they are bad for the stomach!

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow)](https://opensource.org)
[![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C)](https://cppreference.com)
[![SDL3](https://img.shields.io/badge/Library-SDL3-008080)](https://libsdl.org)

## Implementation details

### 📁 Project Structure

```text
├── assets/  # Assets and media files
├── bin/     # Platform specific compiled libraries
├── include/ # C++ Project header files (.hpp)
├── lib/     # Third-party libraries
├── src/     # Source files (.cpp)
└── CMakeLists.txt
```

### 🏗️ Architecture & design highlights

This project implements modern C++20 design patterns, safe resource management and clean architectural separation.

- **Type-Safe State Machine:** The game loops through 3 distinct states (`Menu`, `Gameplay`, `GameOver`). Instead of raw pointers or virtual inheritance, the state machine leverages **`std::variant`** and **`std::visit`** for type-safe, compile-time polymorphism.
- **Strict RAII Lifecycle:** All SDL3 subsystems, windows, and renderer lifecycles are strictly bound to object lifetimes. The initialization and cleanup are fully encapsulated within a custom **`SDLState`** abstraction class, preventing memory and resource leaks.
- **Modern Cross-Platform Build Pipeline:** Engineered with a platform-independent CMake system that automatically routes builds into structured target directories based on architecture (`bin/Windows64`, `bin/Linux64`, `bin/Darwin64`).

## ⚡ Building the project

### Windows

#### Install Toolchain Prerequisites

Modern C++20 compiler and cmake are required.
Download the official installers here:
- [Microsoft Visual Studio Community](https://visualstudio.microsoft.com/downloads/)
- [CMake](https://cmake.org)
- [Git](https://gitforwindows.org)

#### Configure and Compile
Run the following lines of code from project's root directory:
```powershell
cmake -B build
cmake --build build --config Release
```

#### Run
From project's root directory, you would launch the executable with the following:
```powershell
# For 64-bit systems
.\bin\Windows64\Release\app.exe

# For 32-bit systems
.\bin\Windows32\Release\app.exe
```

### MacOS

#### Install Toolchain Prerequisites

Modern C++20 compiler and cmake are required. If you use the [Homebrew](https://brew.sh) package manager, the following commands will install all needed packages:
```bash
# Install Xcode command tools
xcode-select --install

# Install CMake with brew
brew install cmake
```
#### Configure and Compile
Run the following lines of code from project's root directory:
```bash
cmake -B build
cmake --build build --config Release
```

#### Run
From project's root directory, you would launch the executable with the following:
```bash
# For 64-bit systems
./bin/Darwin64/Release/app

# For 32-bit systems
./bin/Darwin32/Release/app
```

### Linux

#### Install Toolchain Prerequisites

Modern C++20 compiler, CMake, a build system like GNU/Make and git are needed:
```bash
# On Ubuntu based distros
sudo apt update
sudo apt install g++ cmake make git

# On Arch based distros
sudo pacman -Syu
sudo pacman -S gcc cmake make git

# On Fedora based distros
sudo dnf install gcc-c++ cmake make git
```
#### Configure and Compile
Run the following lines of code from project's root directory:
```bash
cmake -B build
cmake --build build --config Release
```

#### Run
From project's root directory, you would launch the executable with the following:
```bash
# For 64-bit systems
./bin/Linux64/Release/app

# For 32-bit systems
./bin/Linux32/Release/app
```

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
