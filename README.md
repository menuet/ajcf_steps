
# INSTALLING THE TOOLS

## Install software on Windows

- **VSCode** : https://code.visualstudio.com/
- **VSCode Extensions** :
  - **Remote Development** (author: *Microsoft*)
  - **C/C++** (author: *Microsoft*)
  - **CMake Tools** (author: *Microsoft*)
  - **CMake** (*optional*) (author: *twxs*)
  - **Clang-format** (*optional*) (author: *xaver*)
- **Git** (*optional*) : https://git-scm.com/
- **TortoiseGit** (*optional*) : https://tortoisegit.org/
- **Microsoft C++ Build Tools** (*optional*) : https://visualstudio.microsoft.com/visual-cpp-build-tools/ 

### Windows Subsystem for Linux

- **Requires Windows 10**
- **WSL Docs** : https://docs.microsoft.com/en-us/windows/wsl/
- **Ubuntu on WSL Docs** : https://wiki.ubuntu.com/WSL

### Enable WSL

- **Enable WSL 1** (*then Reboot*)
```
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
```
- **Enable WSL 2 *(Requires Windows 10 2004 Build 19041)*** (*then Reboot*)
```
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```
- *Set WSL 2 as the default version (optional)*
```
wsl --set-default-version 2
```

### Download and Install Ubuntu 20.04 on WSL

- **Use Microsoft Store** : https://www.microsoft.com/fr-fr/p/ubuntu-2004-lts/9n6svws3rx71?activetab=pivot:overviewtab
- **Or download image** :<br>https://cloud-images.ubuntu.com/releases/focal/release/ubuntu-20.04-server-cloudimg-amd64-wsl.rootfs.tar.gz
- **Then import image in WSL** :
```
wsl --import <InstanceName> <InstanceStorageDirectory> <ImageFileFullPath>
```

## Install Software on Ubuntu 20.04

### Utilities

```
sudo apt update
sudo apt install -y build-essential curl unzip tar tig
```

### Compilers

- **GCC 9**: https://gcc.gnu.org/gcc-9/
  ```
  sudo apt install -y g++ gdb
  ```
- **Clang 10** *(optional)*: https://clang.llvm.org/
  ```
  sudo apt install -y clang lldb lld
  ```

### Build Engine

**CMake**: https://cmake.org/
```
sudo apt install -y cmake
```

### Package Manager

**Vcpkg**: https://github.com/microsoft/vcpkg
```
cd ~ && mkdir -p DEV && cd DEV && git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh -disableMetrics
sudo update-alternatives --install /usr/bin/vcpkg vcpkg $HOME/DEV/vcpkg/vcpkg 202004
echo VCPKG_ROOT=$HOME/DEV/vcpkg >> $HOME/.bashrc
source $HOME/.bashrc
```

## Alternate possibilities

- VirtualBox + Ubuntu 20.04
- Multipass
- Ubuntu < 20.04
- Cloud VMs + SSH
- ...

## If Ubuntu < 20.04

```
sudo apt-get update
sudo apt-get -y install software-properties-common gnupg rsync zip openssh-server make sudo git curl unzip tar

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y g++ g++-9 gdb

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9

sudo apt-get update
sudo apt-get install -y clang-9 lldb-9 lld-9

sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-9 90 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-9

curl -L -o cmake-3.17.2.tar.gz https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2-Linux-x86_64.tar.gz
tar -xzvf cmake-3.17.2.tar.gz
mv cmake-3.17.2-Linux-x86_64 cmake
sudo update-alternatives --install /usr/bin/cmake cmake $HOME/DEV/cmake/bin/cmake 3172 --slave /usr/bin/ctest ctest $HOME/DEV/cmake/bin/ctest
```

## Testing C++ compiler on WSL-Ubuntu-20.04

- **GCC**:
    ```
    mkdir -p ~/DEV/test
    cd ~/DEV/test
    echo "int main() {}" > test.cpp
    g++ -c test.cpp
    g++ test.o -o test
    ./test
    ```
- **Clang**: same process using *clang++*

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
