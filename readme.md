## Pre-reqs

1. Tesseract
2. OpenCV

## How to install pre-reqs

	brew tap brew/science
	brew install opencv
	brew install tesseract

Opencv might take a very long time to build in the "making bootstrap step"

## How-to build and run

The file "install.sh" builds and moves the binaries to /usr/local/bin without requiring su access

    ./install.sh

After completing this step, you can use the helper .py files to select folders and process all of the images inside it