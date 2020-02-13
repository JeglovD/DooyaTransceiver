#include "Transmitter.h"

#include <Arduino.h>

namespace Dooya
{
	static const uint8_t PIN_TRANSMITTER_DATA{ 4 };

	Transmitter::Transmitter()
	{
		// --------------------------------------------------
		// Инициализация Timer2
		
		// Запрещаем прерывания
		cli();
		
		// Режим таймера: сброс при совпадении
		// TCCR2A - регистр управления A для Timer2
		// WGM21 - флаг( бит ), при установке которого таймер работает в режиме CTC (Clear Timer on Compare Match Mode)
		TCCR2A = 0x00;
		TCCR2A |= (1 << WGM21);
		
		// Устанавливаем делитель частоты на 32
		// TCCR2B - регистр управления B для Timer2
		TCCR2B = 0x00;
		TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);
			
		// Устанавливаем число, до которого нужно считать
		// OCR2A - регистр сравнения A, при совпадении с которым будет генериться прерывание, максимум 255 (0xFF)
		OCR2A = 83;

		// Разрешаем прерывание при совпадении с регистром A
		// TIMSK2 - регистр включения прерывания
		// OCIE2A - флаг( бит ) включения прерывания при совпадении со значением в OCR2A
		TIMSK2 = 0x00;
		TIMSK2 |= (1 << OCIE2A);

		// Разрешаем прерывания
		sei();

		// --------------------------------------------------
		// Конфигурируем пины
		pinMode(PIN_TRANSMITTER_DATA, OUTPUT);
	}

	void Transmitter::Interrupt()
	{
		digitalWrite(PIN_TRANSMITTER_DATA, HIGH - digitalRead(PIN_TRANSMITTER_DATA));
	}

	ISR(TIMER2_COMPA_vect)
	{
		Transmitter::Instance().Interrupt();
	}
}