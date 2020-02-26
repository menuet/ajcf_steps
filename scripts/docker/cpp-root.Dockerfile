# Reference: https://devblogs.microsoft.com/cppblog/build-c-applications-in-a-linux-docker-container-with-visual-studio/

# Useful docker commands
# Get ubuntu image:
#    docker pull ubuntu
# List images:
#    docker images
# Create image:
#    docker build -t ubuntu-cpp-root .
# Execute container:
#    docker run -p 5000:22 -i -t ubuntu-cpp-root /bin/bash
# Stop container:
#    exit
# Execute again:
#    docker ps -a && docker start ubuntu-cpp-root && docker attach ubuntu-cpp-root


# Base image
FROM ubuntu

LABEL description = "Container for compilation on linux (root)"

# Install general utilities
RUN apt-get update && \
    apt-get -y install software-properties-common gnupg rsync zip openssh-server make sudo git curl unzip tar

# Configure SSH
RUN mkdir -p /var/run/sshd
RUN echo 'PasswordAuthentication yes' >> /etc/ssh/sshd_config && ssh-keygen -A

# Expose port 22
EXPOSE 22

# Add account developer:developer
RUN useradd -m -d /home/developer -s /bin/bash -G sudo developer && echo "developer:developer" | chpasswd

# Install g++9
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y g++ g++-9 gdb

# Use gcc9 as gcc
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9

# Install clang-9
RUN apt-get update && \
    apt-get install -y clang-9 lldb-9 lld-9

# Use clang-9 as clang
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-9 90 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-9
