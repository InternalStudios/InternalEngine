#!/bin/bash

sudo apt install gnupg ca-certificates
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 3FA7E0328081BFF6A14DA29AA6A19B38D3D831EF
echo "deb https://download.mono-project.com/repo/ubuntu stable-focal main" | sudo tee /etc/apt/sources.list.d/mono-official-stable.list
sudo apt update

sudo apt install mono-complete

sudo mkdir /usr/include/mono
sudo cp -r /usr/include/mono-2.0/mono/* /usr/include/mono/

wget https://vulkan.lunarg.com/sdk/home#sdk/downloadConfirm/1.2.148.1/linux/vulkansdk-linux-x86_64-1.2.148.1.tar.gz
tar xf vulkansdk-linux-x86_64-1.2.148.1.tar.gz
mkdir Internal/libs/vulkan
mkdir Internal/libs/vulkan/Linux
cp -r 1.2.148.1/* Internal/libs/vulkan/Linux/