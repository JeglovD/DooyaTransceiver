#pragma once

#include <Arduino.h>

namespace Dooya
{
	static uint8_t PIN_TEST{ 9 };

	class SynchroWord
	{
	public:
		SynchroWord();
		void Clear();
		bool IsSet() { return mHighDuration && mLowDuration; }
		void SetHighDuration(const unsigned long& high_duration) { mHighDuration = high_duration; }
		void SetLowDuration(const unsigned long& low_duration) { mLowDuration = low_duration; }
		void Check();

	private:
		unsigned long mHighDuration;
		unsigned long mLowDuration;
	};

	//	class Data
	//	{
	//	public:
	//		Data();

	//	private:
	//		unsigned long mHighDuration[0x100];
	//		unsigned long mLowDuration[0x100];
	//		uint8_t mBegin;
	//		uint8_t mEnd;
	//	};

	class ReceiverBuffer
	{
	public:
		ReceiverBuffer();
		//	Data& Data() { return mData; }
		void Check();
		bool IsSet();
		void Clear();

	private:
		SynchroWord mSynchroWord;
	//	Data mData;

	public:
		SynchroWord& SynchroWord() { return mSynchroWord; }
	};
	
	class Receiver
	{
	public:
		static Receiver& Instance() { static Receiver receiver{}; return receiver; }
		static void InterruptRising();
		static void InterruptFalling();
		bool IsData();

	private:
		Receiver();
		Receiver(const Receiver&);
		Receiver& operator=(Receiver&);
		void MicrosStore();
		void Check();

		ReceiverBuffer mBuffer1;
		ReceiverBuffer mBuffer2;
		ReceiverBuffer& mBufferWorking;
		unsigned long mMicrosPrevious;
		unsigned long mMicrosCurrent;
	};
}