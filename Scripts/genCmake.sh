#!/bin/sh

sudo python3 -m pip install cmake-converter

cd ..

premake5 vs2019 --os=linux

cmake-converter -s Internal.sln

rm Internal.sln
rm Internal/InternalEngine.vcxproj
rm Internal/InternalEngine.vcxproj.filters
rm Internal/libs/glad/Glad.vcxproj
rm Internal/libs/glad/Glad.vcxproj.filters
rm Sandbox/Sandbox.vcxproj