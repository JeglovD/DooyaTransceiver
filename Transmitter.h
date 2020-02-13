#pragma once

#include <Arduino.h>

namespace Dooya
{
	class Buffer
	{
	public:
		Buffer() {};
		void Put(const byte& data);
		bool Get(bool& bit);

	private:
		byte mData[0x100] = {};
		byte mBitCounter[0x100] = {};
		uint8_t mDataBegin{ 0 };
		uint8_t mDataEnd{ 0 };
	};

	class Transmitter
	{
	public:
		static Transmitter& Instance() { static Transmitter transmitter{}; return transmitter; }
		void Interrupt();
		void Put(const byte& data);
		void Transmit();

		bool Get(bool& bit) { return mBuffer.Get(bit); }

	private:
		Transmitter();

		Buffer mBuffer{};
		bool mTransmit{ false };
	};
}