g++ -Wall -o $1 $1.cpp -L. -lwiringPi -lpthread -lGPIO $2 `pkg-config --cflags --libs opencv` -L../../lib/libGPIO.a
