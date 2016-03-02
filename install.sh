pushd src/textbox/build; cmake ..; make; popd
pushd src/boxslice/build; cmake ..; make; popd
sudo cp src/textbox/build/TextBox /usr/local/bin
sudo cp src/boxslice/build/boxslice /usr/local/bin