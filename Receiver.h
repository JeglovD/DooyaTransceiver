#pragma once

namespace Dooya
{
	class ReceiverBuffer
	{
	public:
		ReceiverBuffer();

	private:
		class SynchroWord
		{
		public:
			SynchroWord();
			Clear();

		private:
			unsigned long mHighDuration;
			unsigned long mLowDuration;
		};

		class Data
		{
		public:
			Data();

		private:
		};

		SynchroWord mSynchroWord;
		Data mData;
	};
	
	class Receiver
	{
	public:
		static Receiver& Instance() { static Receiver receiver{}; return receiver; }
		static void InterruptRising();
		static void InterruptFalling();

	private:
		Receiver();

		ReceiverBuffer mBuffer1;
		ReceiverBuffer mBuffer2;
		ReceiverBuffer& mBufferWorking;
	};
}