#include <SPI.h>
#include <SPI_ESP.h>


uint8_t errorCode = 0;

SPI_ESP mySPI = SPI_ESP(D1,D2);

void setup()
{
	Serial.begin(38400);
	//long unsigned debug_start = millis();
	//while (!Serial && ((millis() - debug_start) <= 5000));
	errorCode = mySPI.begin();
	if (errorCode) {
		Serial.println(">> SPI FIFO ready <<");
		Serial.print("Using bus:SPI");
		Serial.println(mySPI.getSPIbus());
	}
	else {
		Serial.println(">> ERROR <<");
		if (bitRead(errorCode, 0) == 0) {
			Serial.print("mosi,sclk or miso pin mismatch!");
		}
		else if (bitRead(errorCode, 1) == 0){
			Serial.print("cs pin mismatch");
		}
		else if (bitRead(errorCode, 2) == 0) {
			Serial.print("dc pin mismatch");
		}
	}

}

void loop()
{


}

/*
writeByte_cont(data,dataMode)
writeByte_last(data,dataMode)
writeWord_cont(data,dataMode)
writeWord_last(data,dataMode)
dataMode default is false, mean command mode. This is true only when DC pin is configured or it will be ignored
Here's an example of use, we have 2 basic type of functions, _cont and _last, _cont = pull down CS, _last = pull Up it (disable)
after trasfer data so must be used as last one, if you just have one trasfer, use always _last.
It's important start with startTransaction and end with endTransaction!
In this example a byte is tranfered using only CS (even you you are using the dc pin as well)
If you want transfer a byte in data mode (such used in some OLED/TFT libraries), use:
mySPI.writeByte_last(data,true);
*/
void writeByte(byte data)
{
	mySPI.startTransaction();
	mySPI.writeByte_last(data);
	mySPI.endTransaction();
}