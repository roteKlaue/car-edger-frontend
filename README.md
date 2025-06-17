# car-edger-frontend
A native GUI application for interacting with the CarEdger backend.

## 🚀 Get Started

### Prebuilt Binaries
You can download prebuilt binaries from the [Releases page](https://github.com/roteKlaue/car-edger-frontend/releases).

### 🧰 Manual Build Instructions
#### Prerequisites
- `Visual Studio 2022` with the `C++` development workload installed
- `vcpkg` package manager cloned and bootstrapped
#### Step-by-Step Setup
1. Install & Integrate `vcpkg`
    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.bat
    .\vcpkg integrate install
    ```
2. Add `nlohmann-json`
    ```bash
    .\vcpkg install nlohmann-json
    ```
3. Clone the project
    ```bash
    git clone https://github.com/roteKlaue/car-edger-frontend.git
    cd car-edger-frontend
    ```
4. Open the Visual Studio Solution
    1. Launch Visual Studio 2022
    2. Select `File` → `Open` → `Project/Solution...`, navigate to the cloned car-edger-frontend folder and select the `CarEdger.sln` file
    3. This should auto-detect the project sources
6. Build & Run
  - Build via Build → Build Solution (or press Ctrl+Shift+B)
  - Run with Debug → Start Without Debugging (or Ctrl+F5)

## 🛠 Technologies Used
- C++20 – Core language used for development
- Win32 API – For creating the native GUI and handling system-level operations
- Visual Studio 2022 – IDE for building and debugging
- vcpkg – Dependency manager for C++ libraries
- nlohmann/json – JSON library used for data serialization and configuration
- Windows x64 – Target platform
- GitHub Actions – CI/CD dreams (crashed and burned — see commit history for proof 🚒)
