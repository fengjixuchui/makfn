#pragma once

__int64 (*DetermineTraceChannelOriginal)(__int64, __int64, char);
__int64 DetermineTraceChannel(__int64 a1, __int64 a2, char a3)
{
	__int64 ReturnValue = reinterpret_cast<__int64(__cdecl*)(__int64, __int64, char, uintptr_t, void*)>(DoSpoofCall)(a1, a2, a3, SpoofCode, DetermineTraceChannelOriginal);

	if (Settings::Exploits::MagicWeapon)
	{
		return 16;
	}

	return ReturnValue;
}