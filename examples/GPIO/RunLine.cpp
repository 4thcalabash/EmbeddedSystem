#include "GPIOlib.h"

using namespace GPIO;

int main()
{
	init();
	for (int i=1;i<=12;i++){

		if (i&1){
			controlLeft(FORWARD,15);
			controlRight(FORWARD,15);
		}else{
			controlRight(FORWARD,15);
			controlLeft(FORWARD,15);
		}
		delay(200);
	}
	stopLeft();
	stopRight();
	return 0;


}
