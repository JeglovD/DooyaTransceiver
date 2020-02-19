#include "Receiver.h"

namespace Dooya
{
	static uint8_t PIN_TEST{ 9 };

	ReceiverBuffer::ReceiverBuffer()
	{
	}

	void ReceiverBuffer::Check()
	{
		mSynchroWord.Check();
	}

	bool ReceiverBuffer::IsSet()
	{
		return mSynchroWord.IsSet();
	}

	void ReceiverBuffer::Clear()
	{
		mSynchroWord.Clear();
	}

	ReceiverBuffer::SynchroWord::SynchroWord()
	{
		Clear();
	}

	void ReceiverBuffer::SynchroWord::Clear()
	{
		mHighDuration = 0;
		mLowDuration = 0;
	}

	void ReceiverBuffer::SynchroWord::Check()
	{
		if (!mLowDuration)
			return;
		if (mHighDuration > mLowDuration * 2 &&
			mHighDuration < mLowDuration * 4)
			return;
		Clear();
	}
	
	ReceiverBuffer::Data::Data() :
		mBegin{ 0 },
		mEnd{ 0 }
	{
	}

	Receiver::Receiver():
		mBuffer1{},
		mBuffer2{},
		mBufferWorking{mBuffer1},
		mMicrosCurrent{micros()},
		mMicrosPrevious{mMicrosCurrent}
	{
		// --------------------------------------------------
		// Инициализация выводов
		pinMode(PIN2, INPUT);
		pinMode(PIN3, INPUT);
		pinMode(PIN_TEST, OUTPUT);
		
		// --------------------------------------------------
		// Инициализация прерываний
		attachInterrupt(digitalPinToInterrupt(PIN2), InterruptRising, RISING);
		attachInterrupt(digitalPinToInterrupt(PIN3), InterruptFalling, FALLING);
	}

	void Receiver::InterruptRising()
	{
		Receiver& receiver{ Receiver::Instance() };
		receiver.MicrosStore();
		if (receiver.mBufferWorking.SynchroWord().IsSet())
		{
		}
		else
		{
			receiver.mBufferWorking.SynchroWord().SetLowDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
			receiver.Check();
		}
	}

	void Receiver::InterruptFalling()
	{
		Receiver& receiver{ Receiver::Instance() };
		receiver.MicrosStore();
		if (receiver.mBufferWorking.SynchroWord().IsSet())
		{
		}
		else
		{
			receiver.mBufferWorking.SynchroWord().SetHighDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
			receiver.Check();
		}
	}

	void Receiver::MicrosStore()
	{
		mMicrosPrevious = mMicrosCurrent;
		mMicrosCurrent = micros();
	}

	void Receiver::Check()
	{
		mBufferWorking.Check();

		if (mBufferWorking.IsSet())
		{
			if (&mBufferWorking == &mBuffer1)
			{
				mBuffer2.Clear();
				mBufferWorking = mBuffer2;
			}
			else
			{
				mBuffer1.Clear();
				mBufferWorking = mBuffer1;
			}
		}
	}

	bool Receiver::IsData()
	{
		bool result;
		if (&mBufferWorking == &mBuffer1)
		{
			result = mBuffer2.IsSet();
			digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));
			mBuffer2.Clear();
		}
		else
		{
			result = mBuffer1.IsSet();
			digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));
			mBuffer1.Clear();
		}
	}
}