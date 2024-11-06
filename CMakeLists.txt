cmake_minimum_required(VERSION 3.10)

project(LSB-Steganography)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")


find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} SRC/main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}
opencv_core 
opencv_imgcodecs 
opencv_highgui)