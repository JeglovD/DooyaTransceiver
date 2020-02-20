#pragma once

#include <Arduino.h>

namespace Dooya
{
	static uint8_t PIN_TEST{ 10 };

	class SynchroWord
	{
	public:
		SynchroWord();
		void Clear();
		void SetHighDuration(const unsigned long& high_duration) { mHighDuration = high_duration; }
		void SetLowDuration(const unsigned long& low_duration) { mLowDuration = low_duration; }
		bool Check();
		const unsigned long& Duration() { return mLowDuration; }

	private:
		unsigned long mHighDuration;
		unsigned long mLowDuration;
	};

	class Data
	{
	public:
		Data();
		bool IsSet() { return mHighDuration[0] && mLowDuration[0]; }
		void SetHighDuration(const unsigned long& high_duration) { mHighDuration[0] = high_duration; }
		void SetLowDuration(const unsigned long& low_duration) { mLowDuration[0] = low_duration; }
		bool Check(const unsigned long& synchro_word_duration);

	private:
		unsigned long mHighDuration[0x100];
		unsigned long mLowDuration[0x100];
		uint8_t mBegin;
		uint8_t mEnd;
	};

	class ReceiverBuffer
	{
	public:
		ReceiverBuffer();
		bool Check();
		void Clear();

	private:
		SynchroWord mSynchroWord;
		Data mData;

	public:
		SynchroWord& SynchroWord() { return mSynchroWord; }
		Data& Data() { return mData; }
	};
	
	//class Receiver
	//{
	//public:
	//	static Receiver& Instance() { static Receiver receiver{}; digitalWrite(PIN_TEST, HIGH); return receiver; }

	//private:
	//	Receiver(const Receiver&);
	//	Receiver& operator=(Receiver&);

	//};

	class Receiver
	{
	public:
		static Receiver& Instance() { static Receiver receiver; digitalWrite(PIN_TEST, HIGH); return receiver; }
		bool Check();
		static void InterruptRising();
		static void InterruptFalling();

	private:
		Receiver();
		void MicrosStore();

		ReceiverBuffer mBuffer1;
		ReceiverBuffer mBuffer2;
		ReceiverBuffer& mBufferWorking;
		unsigned long mMicrosPrevious;
		unsigned long mMicrosCurrent;
	};
}