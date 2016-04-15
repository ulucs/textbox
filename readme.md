## Quick Start

Just paste this into terminal

	/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" ; brew install homebrew/science/opencv --without-python && brew install tesseract && brew install cmake && brew install imagemagick && cd Desktop && pushd ~ && git clone https://github.com/ulucs/textbox && pushd textbox && ./install.sh && cp /tst/tst.traineddata /usr/local/Cellar/tesseract/3.04.01_1/share/tessdata/tst.traineddata && popd && popd

## How to run

1. Open the application
2. Select the pdf file to be read
3. If the desired area is shown, select it
4. If not, press any button to skip
5. After selecting the area of interest, press any button to run
6. For each pdf, the readings are in cevaplar.txt in the folder with the name of pdf

## Pre-reqs

1. Tesseract
2. OpenCV
3. Imagemagick