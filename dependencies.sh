#!/bin/bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" && \
brew install homebrew/science/opencv --without-python && \
brew install tesseract && \
brew install cmake && \
git clone https://github.com/ulucs/textbox && \
pushd textbox && \
./install.sh && \
cp /tst/tst.traineddata /usr/local/Cellar/tesseract/3.04.01_1/share/tessdata/tst.traineddata && \
popd