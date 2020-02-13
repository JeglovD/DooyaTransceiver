#include "Transmitter.h"

#include <Arduino.h>

namespace Dooya
{
	static const uint8_t PIN_TRANSMITTER_DATA{ 4 };

	Transmitter::Transmitter()
	{
		// --------------------------------------------------
		// ������������� Timer2
		
		// ��������� ����������
		cli();
		
		// ����� �������: ����� ��� ����������
		// TCCR2A - ������� ���������� A ��� Timer2
		// WGM21 - ����( ��� ), ��� ��������� �������� ������ �������� � ������ CTC (Clear Timer on Compare Match Mode)
		TCCR2A = 0x00;
		TCCR2A |= (1 << WGM21);
		
		// ������������� �������� ������� �� 32
		// TCCR2B - ������� ���������� B ��� Timer2
		TCCR2B = 0x00;
		TCCR2B |= (0 << CS22) | (1 << CS21) | (1 << CS20);
			
		// ������������� �����, �� �������� ����� �������
		// OCR2A - ������� ��������� A, ��� ���������� � ������� ����� ���������� ����������, �������� 255 (0xFF)
		OCR2A = 83;

		// ��������� ���������� ��� ���������� � ��������� A
		// TIMSK2 - ������� ��������� ����������
		// OCIE2A - ����( ��� ) ��������� ���������� ��� ���������� �� ��������� � OCR2A
		TIMSK2 = 0x00;
		TIMSK2 |= (1 << OCIE2A);

		// ��������� ����������
		sei();

		// --------------------------------------------------
		// ������������� ����
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