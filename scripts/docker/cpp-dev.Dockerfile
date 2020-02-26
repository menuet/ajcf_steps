
# Base image
FROM ubuntu-cpp-root

LABEL description = "Container for compilation on linux (dev)"

# Use account developer
USER developer

# Install vcpkg
RUN mkdir -p $HOME/DEV
RUN cd $HOME/DEV && git clone https://github.com/Microsoft/vcpkg.git && $HOME/DEV/vcpkg/bootstrap-vcpkg.sh
ENV VCPKG_ROOT /home/developer/DEV/vcpkg

# Install catch2 & fmt
RUN $HOME/DEV/vcpkg/vcpkg install --triplet x64-linux catch2 fmt date tl-expected

# Start SSH service when starting container (need developer's password)
ENTRYPOINT sudo service ssh start && /bin/bash
