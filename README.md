
# Small Steps To C++

## Online resources
- Hub ISO C++ : https://isocpp.org/
- Reference : https://en.cppreference.com/w/
- Papers : https://wg21.link/
- Core guidelines : https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
- Online compilers :
  - https://godbolt.org/
  - https://wandbox.org/,
  - http://quick-bench.com/

## Installing software on Windows
- VS Code : https://code.visualstudio.com/, with recommended extensions :
  - Remote Development
  - C/C++ Extension Pack
  - Cmake Extension Pack
  - GitLens
  - Clang-format
- Git *(Optional)* : https://git-scm.com/
- TortoiseGit *(Optional)* : https://tortoisegit.org/
- MS C++ Build Tools *(Optional)* : https://visualstudio.microsoft.com/visual-cpp-build-tools/

## Enabling WSL on Windows + Installing Ubuntu 20.04
- WSL Docs : https://docs.microsoft.com/en-us/windows/wsl/
- Enable WSL :
    ```
    dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
    ```
- WSL-Ubuntu Docs : https://wiki.ubuntu.com/WSL
- Download :
  - With Microsoft Store : https://www.microsoft.com/fr-fr/p/ubuntu-2004-lts/9n6svws3rx71?activetab=pivot:overviewtab
  - Manually :
    - Image : https://cloud-images.ubuntu.com/releases/focal/release/ubuntu-20.04-server-cloudimg-amd64-wsl.rootfs.tar.gz
    - Import image :
        ```
        wsl --import <WSLInstanceName> <WSLStorageDirectory> <WSLImageFilePath>
        ```
- Log in :
    ```
    wsl --distribution <WSLInstanceName>
    ```

## Installing software on WSL-Ubuntu-20.04
- General utilities (make, gcc, unzip, …) :
    ```
    sudo apt update
    sudo apt install -y build-essential curl unzip tar
    ```
- Gcc 9 debugger : https://gcc.gnu.org/gcc-9/
    ```
    sudo apt install -y g++ gdb
    ```
- Clang 10 : https://clang.llvm.org/
    ```
    sudo apt install -y clang lldb lld
    ```
- Cmake : https://cmake.org/
    ```
    sudo apt install -y cmake
    ```
- Vcpkg : https://github.com/microsoft/vcpkg
    ```
    cd ~ && mkdir -p DEV && cd DEV
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh -disableMetrics
    sudo update-alternatives --install /usr/bin/vcpkg vcpkg $HOME/DEV/vcpkg/vcpkg 202004
    echo VCPKG_ROOT=$HOME/DEV/vcpkg >> $HOME/.bashrc
    source $HOME/.bashrc
    ```

## Testing C++ compiler on WSL-Ubuntu-20.04
- Gcc :
    ```
    mkdir -p ~/DEV/test
    cd ~/DEV/test
    echo "int main() {}" > test.cpp
    g++ -c test.cpp
    g++ test.o -o test
    ./test
    ```
- Clang : same process using *clang++*

## Testing Git on WSL-Ubuntu-20.04
- Clone this repository using https :
    ```
    cd ~/DEV
    git clone https://github.com/menuet/ajcf_steps.git ajcf_steps-HTTPS
    ```
- Create ssh private/public keys :
    ```
    ssh-keygen
    ```
- Add the public key to your github account : https://help.github.com/en/github/authenticating-to-github/adding-a-new-ssh-key-to-your-github-account
- Clone this repository using ssh :
    ```
    cd ~/DEV
    git clone git@github.com:menuet/ajcf_steps.git ajcf_steps-SSH
    ```

## Testing CMake without Vcpkg on WSL-Ubuntu-20.04
- Clone this repository :
    ```
    cd ~/DEV
    git clone https://github.com/menuet/ajcf_steps.git
    ```
- Build with cmake :
    ```
    cd ~/DEV/ajcf_steps
    mkdir -p build-without-vcpkg
    cd build-without-vcpkg
    cmake -DAJCF_ENABLE_VCPKG=OFF ..
    cmake --build . --parallel 4
    ```
- Test with cmake :
    ```
    ctest
    ```

## Testing CMake with Vcpkg on WSL-Ubuntu-20.04
- Install some third-parties libraries with vcpkg :
    ```
    vcpkg install --triplet x64-linux catch2 fmt date tl-expected ms-gsl
    ```
- Build with cmake using vcpkg's third-parties :
    ```
    cd ~/DEV/ajcf_steps
    mkdir -p build-with-vcpkg
    cd build-with-vcpkg
    cmake -DAJCF_ENABLE_VCPKG=ON -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake ..
    cmake --build . --parallel 4
    ```

## Editing code on WSL-Ubuntu-20.04 from Windows
- Launch VS Code from within WSL-Ubuntu :
    ```
    cd ~/DEV/ajcf_steps
    code .
    ```
