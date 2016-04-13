## Quick Start

Just paste this into terminal

	cd ~ && /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" && brew install homebrew/science/opencv --without-python && brew install tesseract && brew install cmake && brew install imagemagick && cd Desktop && git clone https://github.com/ulucs/textbox && pushd textbox && ./install.sh && cp /tst/tst.traineddata /usr/local/Cellar/tesseract/3.04.01_1/share/tessdata/tst.traineddata && popd

## How to run

1. Open the 

## Pre-reqs

1. Tesseract
2. OpenCV

## How to install pre-reqs

	brew tap brew/science
	brew install opencv --without-python
	brew install tesseract

## How-to build and run

The file "install.sh" builds and moves the binaries to /usr/local/bin without requiring su access

    ./install.sh

After completing this step, you can use the helper .py files to select folders and process all of the images inside it