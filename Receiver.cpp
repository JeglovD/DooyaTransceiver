#include "Receiver.h"

#include <Arduino.h>

namespace Dooya
{
	static uint8_t PIN_TEST{ 9 };

	ReceiverBuffer::ReceiverBuffer()
	{
	}

	ReceiverBuffer::SynchroWord::SynchroWord()
	{
		Clear();
	}

	ReceiverBuffer::Data::Data()
	{
	}

	ReceiverBuffer::SynchroWord::Clear()
	{
		mHighDuration = 0;
		mLowDuration = 0;
	}
	
	Receiver::Receiver():
		mBuffer1{},
		mBuffer2{},
		mBufferWorking{mBuffer1}
	{
		// --------------------------------------------------
		// Инициализация выводов
		pinMode(PIN2, INPUT);
		pinMode(PIN3, INPUT);
		pinMode(PIN_TEST, OUTPUT);
		
		// --------------------------------------------------
		// Инициализация прерываний
		// Прерывание на PIN2
		attachInterrupt(0, InterruptRising, RISING);
		// Прерывание на PIN3
		attachInterrupt(1, InterruptFalling, FALLING);
	}

	void Receiver::InterruptRising()
	{
		digitalWrite(PIN_TEST, HIGH);
	}

	void Receiver::InterruptFalling()
	{
		digitalWrite(PIN_TEST, LOW);
	}
}