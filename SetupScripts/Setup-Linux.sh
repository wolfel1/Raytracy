#!/bin/bash

pushd .
sudo vcpkg install
vendor/bin/premake/Linux/premake5 --file=build.lua gmake
popd
