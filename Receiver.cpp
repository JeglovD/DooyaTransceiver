#include "Receiver.h"

namespace Dooya
{
	SynchroWord::SynchroWord()
	{
		Clear();
	}

	void SynchroWord::Clear()
	{
		mHighDuration = 0;
		mLowDuration = 0;
	}

	void SynchroWord::Check()
	{
		bool check{ true };
		if (!mHighDuration && mLowDuration)
			check = false;
		if (mHighDuration && mLowDuration)
		{
			if (mHighDuration < mLowDuration * 2)
				check = false;
			if (mHighDuration > mLowDuration * 4)
				check = false;
		}
		if (!check)
			Clear();
	}

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

	//ReceiverBuffer::Data::Data() :
	//	mBegin{ 0 },
	//	mEnd{ 0 }
	//{
	//}

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
		attachInterrupt(digitalPinToInterrupt(PIN2), Receiver::InterruptRising, RISING);
		attachInterrupt(digitalPinToInterrupt(PIN3), Receiver::InterruptFalling, FALLING);
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
			digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));

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
	//	bool result;
	//	if (&mBufferWorking == &mBuffer1)
	//	{
	//		result = mBuffer2.IsSet();
	//		//digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));
	//		mBuffer2.Clear();
	//	}
	//	else
	//	{
	//		result = mBuffer1.IsSet();
	//		//digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));
	//		mBuffer1.Clear();
	//	}
	}
}