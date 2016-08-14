/* 
This library was developed for fast prototype code that use SPI
calls, it's pretty fast and can use legacy mode.
Nothing special here....
 */
 
#ifndef _SPI_ESPLIB_H_INCLUDED
#define _SPI_ESPLIB_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>

#if defined(ESP8266)
	#if !defined(_ESP8266_STANDARDMODE)
		#include <eagle_soc.h>
	#endif
#else
	#error "ONLY for ESP8266 ! ! !"
#endif

static SPISettings 	_spiSettings;

class SPI_ESP {
	public:
		SPI_ESP();//used for include in other libraries
		SPI_ESP(const uint8_t csPin,const uint8_t dcPin=255);
		//this must be called before begin inside other libraries
		void		postInstance(const uint8_t csPin,const uint8_t dcPin);
		bool		begin(SPISettings settings=SPISettings(30000000, MSBFIRST, SPI_MODE0),bool avoidInit=false);
		void		setSpiSettings(SPISettings settings);
		uint8_t 	getSPIbus(void);//return always 0
		void		startTransaction(void);
		void 		endTransaction(void);
		void		writeByte_cont(uint8_t val,bool dataMode=false);
		void		writeByte_last(uint8_t val,bool dataMode=false);
		void		writeWord_cont(uint16_t val,bool dataMode=false);
		void		writeWord_last(uint16_t val,bool dataMode=false);
		uint8_t		readByte_cont(bool dataMode __attribute__((unused))=false);
		uint16_t	readWord_cont(bool dataMode __attribute__((unused))=false);
		int 		getInterruptNumber(uint8_t pin);//return pin if legal
		void 		enableDataStream(void);
		void 		enableCommandStream(void);
		void 		enableCS(void);
		void 		disableCS(void);
	private:
		#if defined(_ESP8266_STANDARDMODE)
			uint8_t 			_cs;
			uint8_t				_dc;
		#else
			uint32_t 			_cs;
			uint32_t			_dc;

			uint32_t _pinRegister(uint8_t pin)
			__attribute__((always_inline)) {
				return _BV(pin);
			}
		#endif
		void		_changeMode(bool dataMode);
		uint8_t		_initError;
};

#endif