#include "SPI_ESP.h"

SPI_ESP::SPI_ESP()
{

}

SPI_ESP::SPI_ESP(const uint8_t csPin,const uint8_t dcPin)
{
	postInstance(csPin,dcPin);
}

void SPI_ESP::postInstance(const uint8_t csPin,const uint8_t dcPin)
{
	_cs = csPin;
	_dc = dcPin;
}

bool SPI_ESP::begin(SPISettings settings,bool avoidInit)
{
	_spiSettings = settings;
	_initError = 0xFF;
	pinMode(_cs, OUTPUT);
	if (_dc != 255) pinMode(_dc, OUTPUT);
	if (!avoidInit) SPI.begin();
	disableCS();
	_changeMode(true);
	return 0xFF;
}

void SPI_ESP::enableCS(void)
{
	#if defined(_ESP8266_STANDARDMODE)
		digitalWrite(_cs,LOW);
	#else
		GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_cs));//L
	#endif
}
		
void SPI_ESP::disableCS(void)
{
	#if defined(_ESP8266_STANDARDMODE)
		digitalWrite(_cs,HIGH);
	#else
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
	#endif
}

void SPI_ESP::setSpiSettings(SPISettings settings)
{
	_spiSettings = settings;
}

uint8_t SPI_ESP::getSPIbus(void)
{
	return 0;//ESP has currently only one SPI bus
}

void SPI_ESP::enableDataStream(void)
{
	_changeMode(true);
}
		
void SPI_ESP::enableCommandStream(void)
{
	_changeMode(false);
}

void SPI_ESP::_changeMode(bool dataMode)
{
	if (_dc == 255) return;
	if (dataMode){
		#if defined(_ESP8266_STANDARDMODE)
			digitalWrite(_dc,HIGH);
		#else
			GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_dc));//H
		#endif
	} else {
		#if defined(_ESP8266_STANDARDMODE)
			digitalWrite(_dc,LOW);
		#else
			GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_dc));//L
		#endif
	}
}

void SPI_ESP::startTransaction(void)
{
	SPI.beginTransaction(_spiSettings);
	enableCS();
}

void SPI_ESP::endTransaction(void)
{
	SPI.endTransaction();
}

void SPI_ESP::writeByte_cont(uint8_t val,bool dataMode)
{
	_changeMode(dataMode);
	SPI.write(val);
}

void SPI_ESP::writeByte_last(uint8_t val,bool dataMode)
{
	writeByte_cont(val,dataMode);
	disableCS();
}

void SPI_ESP::writeWord_cont(uint16_t val,bool dataMode)
{
	_changeMode(dataMode);
	SPI.write16(val);
}

void SPI_ESP::writeWord_last(uint16_t val,bool dataMode)
{
	writeWord_cont(val,dataMode);
	disableCS();
}

uint8_t SPI_ESP::readByte_cont(bool dataMode)
{
	return SPI.transfer(0x00);
}

uint16_t SPI_ESP::readWord_cont(bool dataMode)
{
	//return SPI.transfer16(0x00);
	union {
		uint16_t val= 0;
		struct {
            uint8_t lsb;
			uint8_t msb;
		};
	} out;
	out.msb = SPI.transfer(0x00);
	out.lsb = SPI.transfer(0x00);
	return out.val;
}

int SPI_ESP::getInterruptNumber(uint8_t pin)
{
	//Interrupts may be attached to any GPIO pin, except GPIO16.
	if (pin == 16 || pin == D0) return 255;//TOFIX
	return pin;
}
