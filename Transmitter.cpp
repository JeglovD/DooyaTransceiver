#include "Transmitter.h"

namespace Dooya
{
	static const uint8_t PIN_TRANSMITTER_DATA{ 4 };

	void Buffer::Put(const byte& data)
	{
		mData[mDataEnd] = data;
		mBitCounter[mDataEnd] = 0xFF;
		mDataEnd++;
	}

	bool Buffer::Get(bool& bit)
	{
		if (mDataBegin == mDataEnd)
			return false;
		bit = mData[mDataBegin] & 0x80;
		mData[mDataBegin] = mData[mDataBegin] << 1;
		mBitCounter[mDataBegin] = mBitCounter[mDataBegin] << 1;
		if (!mBitCounter[mDataBegin])
			mDataBegin++;
		return true;
	}

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
		OCR2A = 69;

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
		digitalWrite(PIN_TRANSMITTER_DATA, LOW);
	}

	void Transmitter::Interrupt()
	{
		if (mTransmit)
		{
			bool bit{ false };
			mTransmit = mBuffer.Get(bit);
			digitalWrite(PIN_TRANSMITTER_DATA, bit ? HIGH : LOW);
		}
	}

	void Transmitter::Put(const byte& data)
	{
		mBuffer.Put(data);
	}

	void Transmitter::Transmit()
	{
		mTransmit = true;
	}

	ISR(TIMER2_COMPA_vect)
	{
		Transmitter::Instance().Interrupt();
	}
}