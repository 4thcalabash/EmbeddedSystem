cmake_minimum_required(VERSION 2.8)

PROJECT(LaneDetection_Demo)

FIND_PACKAGE(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(../../lib)
LINK_DIRECTORIES(/home/pi/workspace/EmbeddedSystemNJU2018-Demo/lib/libGPIO.a)
LINK_DIRECTORIES(/usr/lib)
ADD_COMPILE_OPTIONS(-lwiringPi -lpthread)
SET(SRC
	RunLine.cpp
)

SET(PROJECT_NAME
 demo
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} /home/pi/workspace/EmbeddedSystemNJU2018-Demo/lib/libGPIO.a /usr/lib/ ${OpenCV_LIBS})
