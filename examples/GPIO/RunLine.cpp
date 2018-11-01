#include "GPIOlib.h"

using namespace GPIO;

int main()
{
	init();
	for (int i=1;i<=10;i++){
		controlLeft(FORWARD,60);
		controlRight(FORWARD,60);
		delay(200);
	}
	stopLeft();
	stopRight();
	return 0;
	//Move forward
	controlLeft(FORWARD,50);
	controlRight(FORWARD,50);
	delay(1000);

	//Stop
	stopLeft();
	stopRight();
	delay(1000);

	//Move backward
	controlLeft(BACKWARD,50);
	controlRight(BACKWARD,50);
	delay(1000);

	//Stop
	stopLeft();
	stopRight();
	delay(1000);

	//2 motors can work at different speeds.
	controlLeft(FORWARD,30);
	controlRight(FORWARD,40);
	delay(1000);

	//Stop
	stopLeft();
	stopRight();
	delay(1000);

	//Even directions can differ from each other.
	controlLeft(BACKWARD,35);
	controlRight(FORWARD,20);
	delay(1000);

	//Don't forget to stop all motors before exiting.
	stopLeft();
	stopRight();
	return 0;
}
