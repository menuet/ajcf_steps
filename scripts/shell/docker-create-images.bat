
docker pull ubuntu:20.04
docker images
docker build -t ubuntu-cpp-root -f "%~dp0..\docker\cpp-root.Dockerfile" "%~dp0..\docker"
docker build -t ubuntu-cpp-dev -f "%~dp0..\docker\cpp-dev.Dockerfile" "%~dp0..\docker"
