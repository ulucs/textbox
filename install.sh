#!/bin/bash
mkdir -p bin
mkdir -p src/textbox/build
pushd src/textbox/build; cmake ..; make; popd
sudo cp src/textbox/build/TextBox bin
# mkdir -p src/boxslice/build
# pushd src/boxslice/build; cmake ..; make; popd
# sudo cp src/boxslice/build/boxslice /usr/local/bin
mkdir -p src/selecttext/build
pushd src/selecttext/build; cmake ..; make; popd
sudo cp src/selecttext/build/selecttext bin
mkdir -p src/textbox2/build
pushd src/textbox2/build; cmake ..; make; popd
sudo cp src/textbox2/build/textbox2 bin
mkdir -p src/layoutfit/build
pushd src/layoutfit/build; cmake ..; make; popd
sudo cp src/layoutfit/build/layoutfit bin
mkdir -p src/layoutcrop/build
pushd src/layoutcrop/build; cmake ..; make; popd
sudo cp src/layoutcrop/build/layoutcrop bin