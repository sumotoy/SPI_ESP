/* 
SPI_ESP. An SPI library method for easy tranfer data using SPI

This is a test to create an opimized standard method for fast transfer SPI between libraries.
Use is simple:

	1) startTransaction
		2) writeByte_cont or writeWord_cont
		...
		...
		n) writeByte_last or writeWord_last
	x) endTransaction
	
 This library works with: Any 8bit arduino using LAST IDE core!
---------------------------------------------------------------------------------------------------------------------
Version history:
0.1: Early alpha.
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2016, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
		Coded by: Max MC Costa
---------------------------------------------------------------------------------------------------------------------
Licence:
	Licensed as GNU General Public License.
    SPI_ESP Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SPI_ESP Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
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