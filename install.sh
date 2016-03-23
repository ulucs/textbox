mkdir src/textbox/build
pushd src/textbox/build; cmake ..; make; popd
sudo cp src/textbox/build/TextBox /usr/local/bin
# mkdir src/boxslice/build
# pushd src/boxslice/build; cmake ..; make; popd
# sudo cp src/boxslice/build/boxslice /usr/local/bin
mkdir src/selecttext/build
pushd src/selecttext/build; cmake ..; make; popd
sudo cp src/selecttext/build/selecttext /usr/local/bin
mkdir src/textbox2/build
pushd src/textbox2/build; cmake ..; make; popd
sudo cp src/textbox2/build/textbox2 /usr/local/bin
mkdir src/layoutfit/build
pushd src/layoutfit/build; cmake ..; make; popd
sudo cp src/layoutfit/build/layoutfit /usr/local/bin
mkdir src/layoutcrop/build
pushd src/layoutcrop/build; cmake ..; make; popd
sudo cp src/layoutcrop/build/layoutcrop /usr/local/bin