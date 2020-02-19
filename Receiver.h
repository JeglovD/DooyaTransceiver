#pragma once

#include <Arduino.h>

namespace Dooya
{
	class ReceiverBuffer
	{
	private:
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

		class Data
		{
		public:
			Data();

		private:
			unsigned long mHighDuration[0x100];
			unsigned long mLowDuration[0x100];
			uint8_t mBegin;
			uint8_t mEnd;
		};

		SynchroWord mSynchroWord;
		Data mData;

	public:
		ReceiverBuffer();
		SynchroWord& SynchroWord() { return mSynchroWord; }
		Data& Data() { return mData; }
		void Check();
		bool IsSet();
		void Clear();
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
		void Check();
		void MicrosStore();

		ReceiverBuffer mBuffer1;
		ReceiverBuffer mBuffer2;
		ReceiverBuffer& mBufferWorking;
		unsigned long mMicrosPrevious;
		unsigned long mMicrosCurrent;
	};
}