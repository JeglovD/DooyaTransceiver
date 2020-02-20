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

	bool SynchroWord::Check()
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
		return mHighDuration && mLowDuration;
	}

	ReceiverBuffer::ReceiverBuffer()
	{
	}

	bool ReceiverBuffer::Check()
	{
		if (mSynchroWord.Check())
			return mData.Check(mSynchroWord.Duration());
		return false;
	}

	void ReceiverBuffer::Clear()
	{
		mSynchroWord.Clear();
	}

	Data::Data() :
		mBegin{ 0 },
		mEnd{ 0 }
	{}

	bool Data::Check(const unsigned long& synchro_word_duration)
	{
		//bool check{ true };
		//if (mHighDuration[0] && mLowDuration[0])
		//	check = false;
		//if (mHighDuration[0] && mLowDuration[0])
		//{
		//	if (synchro_word_duration < mHighDuration[0] + mLowDuration[0])
		//		check = false;
		//	if (synchro_word_duration > (mHighDuration[0] + mLowDuration[0]) * 2)
		//		check = false;
		//}
		//if (!check)
		//{
		//	mHighDuration[0] = 0;
		//	mLowDuration[0] = 0;
		//}
		//return mHighDuration[0] && mLowDuration[0];
		return true;
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
		attachInterrupt(digitalPinToInterrupt(PIN2), Receiver::InterruptRising, RISING);
		attachInterrupt(digitalPinToInterrupt(PIN3), Receiver::InterruptFalling, FALLING);
	}

	void Receiver::InterruptRising()
	{
		Receiver& receiver = Receiver::Instance();
		receiver.MicrosStore();
		if (receiver.mBufferWorking.SynchroWord().Check())
			receiver.mBufferWorking.Data().SetLowDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
		else
			receiver.mBufferWorking.SynchroWord().SetLowDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
		receiver.Check();
	}

	void Receiver::InterruptFalling()
	{
		Receiver& receiver = Receiver::Instance();
		receiver.MicrosStore();
		if (receiver.mBufferWorking.SynchroWord().Check())
			receiver.mBufferWorking.Data().SetHighDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
		else
			receiver.mBufferWorking.SynchroWord().SetHighDuration(receiver.mMicrosCurrent - receiver.mMicrosPrevious);
		receiver.Check();
	}

	void Receiver::MicrosStore()
	{
		mMicrosPrevious = mMicrosCurrent;
		mMicrosCurrent = micros();
	}

	bool Receiver::Check()
	{
		//digitalWrite(PIN_TEST, HIGH - digitalRead(PIN_TEST));

		//if (mBufferWorking.Check())
		{
			//if (&mBufferWorking == &mBuffer1)
			//{
			//	mBuffer2.Clear();
			//	mBufferWorking = mBuffer2;
			//}
			//else
			//{
			//	mBuffer1.Clear();
			//	mBufferWorking = mBuffer1;
			//}
		}
	}
}