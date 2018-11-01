g++ -Wall -o $1 $1.cpp -lwiringPi -lGPIO -L ../../lib/libGPIO.a
sudo ./$1
