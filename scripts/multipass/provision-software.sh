#!/bin/bash

sudo apt-get update
sudo apt-get -y install software-properties-common gnupg rsync zip openssh-server make sudo git curl unzip tar
sudo apt-get update
sudo apt-get -y install software-properties-common gnupg rsync zip openssh-server make sudo git curl unzip tar

#sudo sed -i 's/PasswordAuthentication no/PasswordAuthentication yes/g' /etc/ssh/sshd_config
#echo "ubuntu:mulubucpp" | sudo chpasswd
cat /mnt/win_repo/scripts/multipass/sshkey.pub >> $HOME/.ssh/authorized_keys
sudo service ssh stop
sudo service ssh start

# ubuntu 18.04
# sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
# sudo apt-get install -y g++ g++-9 gdb
# sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
# sudo apt-get install -y clang-9 lldb-9 lld-9
# sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-9 90 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-9

# ubuntu 20.04: gcc 9 by default and clang 10 by default
sudo apt-get install -y g++ gdb
sudo apt-get install -y clang lldb lld

mkdir -p $HOME/DEV
cd $HOME/DEV

curl -L -o cmake-3.17.2.tar.gz https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2-Linux-x86_64.tar.gz
tar -xzvf cmake-3.17.2.tar.gz
mv cmake-3.17.2-Linux-x86_64 cmake
sudo update-alternatives --install /usr/bin/cmake cmake $HOME/DEV/cmake/bin/cmake 3172 --slave /usr/bin/ctest ctest $HOME/DEV/cmake/bin/ctest

curl -L -o vcpkg-2020.04.zip https://github.com/microsoft/vcpkg/archive/2020.04.zip
unzip vcpkg-2020.04.zip
mv vcpkg-2020.04 vcpkg
$HOME/DEV/vcpkg/bootstrap-vcpkg.sh -disableMetrics
sudo update-alternatives --install /usr/bin/vcpkg vcpkg $HOME/DEV/vcpkg/vcpkg 202004

echo VCPKG_ROOT=$HOME/DEV/vcpkg >> $HOME/.bashrc
source $HOME/.bashrc
echo VCPKG_ROOT="$VCPKG_ROOT"
# vcpkg install --triplet x64-linux catch2 fmt date tl-expected
