cmake_minimum_required(VERSION 2.8)
project( layoutcrop )
find_package( OpenCV REQUIRED )
include_directories (/usr/local/Cellar/tesseract/3.04.01_1/include)
link_directories (/usr/local/Cellar/tesseract/3.04.01_1/lib)
include_directories (/usr/local/Cellar/leptonica/1.72/include)
link_directories (/usr/local/Cellar/leptonica/1.72/lib)
add_executable( layoutcrop layoutcrop.cpp )
target_link_libraries( layoutcrop ${OpenCV_LIBS} )
target_link_libraries( layoutcrop tesseract )