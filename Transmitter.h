#pragma once

namespace Dooya
{
	class Transmitter
	{
	public:
		static Transmitter& Instance() { static Transmitter transmitter{}; return transmitter; }
		void Interrupt();

	private:
		Transmitter();
	};
}