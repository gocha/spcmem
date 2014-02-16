/*
 * SPCAPI - Message Macros for SNES SPC700 Player
 */

#include <windows.h>
#include <stdint.h>

#include "spcapi.h"

/**
 * Get SPC700 Player window.
 */
HWND Spc_GetHostWindow(void)
{
	return FindWindow(TEXT("SSDLabo_SPCPLAY"), NULL);
}

/**
 * Address range check for APU RAM.
 */
bool Spc_IsAPURAMAddress(uint16_t address)
{
	return true; // 0000-ffff, always true
}

/**
 * Address range check for DSP regs.
 */
bool Spc_IsDSPRegAddress(uint16_t address)
{
	return (address >= 0 && address < 0x80);
}

/**
 * Index range check for SPC700 I/O port.
 */
bool Spc_IsSPC700Port(uint16_t port)
{
	return (port >= 0 && port < 4);
}

/**
 * Index range check for Script700 Work RAM.
 */
bool Spc_IsScript700WorkReg(uint16_t reg)
{
	return (reg >= 0 && reg < 8);
}

/**
 * Index range check for Script700 CmpParam.
 */
bool Spc_IsScript700CmpParam(uint16_t reg)
{
	return (reg >= 0 && reg < 1);
}

typedef uint32_t (*Spc_GetDWordFunc)(uint16_t);
typedef void (*Spc_SetDWordFunc)(uint16_t,uint32_t);

/**
 * Get byte by DWORD read function
 */
inline static uint8_t Spc_GetByteByDWordIO(Spc_GetDWordFunc GetDWord, uint16_t address)
{
	// address range check must be done by caller.

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	int bytePos = address % 4;
	int shift = bytePos * 8;
	uint16_t baseAddress = address - bytePos;
	return (uint8_t) (GetDWord(baseAddress) >> shift);
}

/**
 * Set byte by DWORD write function
 */
inline static void Spc_SetByteByDWordIO(Spc_GetDWordFunc GetDWord, Spc_SetDWordFunc SetDWord, uint16_t address, uint8_t value)
{
	// address range check must be done by caller.

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	int bytePos = address % 4;
	int shift = bytePos * 8;
	uint16_t baseAddress = address - bytePos;
	uint32_t value32 = GetDWord(baseAddress);
	value32 = value32 & ~(0xff << shift) | (value << shift);
	SetDWord(baseAddress, value32);
}

/**
 * Get WORD by DWORD read function
 */
inline static uint16_t Spc_GetWordByDWordIO(Spc_GetDWordFunc GetDWord, uint16_t address)
{
	// address range check must be done by caller.

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	int bytePos = address % 2;
	int shift = bytePos * 8;
	uint16_t baseAddress = address - bytePos;
	return (uint16_t) (GetDWord(baseAddress) >> shift);
}

/**
 * Set WORD by DWORD write function
 */
inline static void Spc_SetWordByDWordIO(Spc_GetDWordFunc GetDWord, Spc_SetDWordFunc SetDWord, uint16_t address, uint16_t value)
{
	// address range check must be done by caller.

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	int bytePos = address % 2;
	int shift = bytePos * 8;
	uint16_t baseAddress = address - bytePos;
	uint32_t value32 = GetDWord(baseAddress);
	value32 = value32 & ~(0xffff << shift) | (value << shift);
	SetDWord(baseAddress, value32);
}

/**
 * Get DWORD from APU RAM
 */
uint32_t Spc_GetAPURAMDWord(uint16_t address)
{
	if (!Spc_IsAPURAMAddress(address) || !Spc_IsAPURAMAddress(address + 3))
	{
		return 0;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	return (uint32_t) SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_GET_RAM | address), (LPARAM)0);
}

/**
 * Set DWORD to APU RAM
 */
void Spc_SetAPURAMDWord(uint16_t address, uint32_t value)
{
	if (!Spc_IsAPURAMAddress(address) || !Spc_IsAPURAMAddress(address + 3))
	{
		return;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_SET_RAM | address), (LPARAM)value);
}

/**
 * Get byte from APU RAM
 */
uint8_t Spc_GetAPURAMByte(uint16_t address)
{
	return Spc_GetByteByDWordIO(Spc_GetAPURAMDWord, address);
}

/**
 * Set byte to APU RAM
 */
void Spc_SetAPURAMByte(uint16_t address, uint8_t value)
{
	Spc_SetByteByDWordIO(Spc_GetAPURAMDWord, Spc_SetAPURAMDWord, address, value);
}

/**
 * Get WORD from APU RAM
 */
uint16_t Spc_GetAPURAMWord(uint16_t address)
{
	return Spc_GetWordByDWordIO(Spc_GetAPURAMDWord, address);
}

/**
 * Set WORD to APU RAM
 */
void Spc_SetAPURAMWord(uint16_t address, uint16_t value)
{
	Spc_SetWordByDWordIO(Spc_GetAPURAMDWord, Spc_SetAPURAMDWord, address, value);
}

/**
 * Get byte from DSP reg
 */
uint8_t Spc_GetDSPRegByte(uint16_t address)
{
	if (!Spc_IsDSPRegAddress(address))
	{
		return 0;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	return (uint32_t) SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_GET_DSP | address), (LPARAM)0);
}

/**
 * Set byte to DSP reg
 */
void Spc_SetDSPRegByte(uint16_t address, uint8_t value)
{
	if (!Spc_IsDSPRegAddress(address))
	{
		return;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_SET_DSP | address), (LPARAM)value);
}

/**
 * Get WORD from DSP reg
 */
uint16_t Spc_GetDSPRegWord(uint16_t address)
{
	return Spc_GetDSPRegByte(address) | (Spc_GetDSPRegByte(address + 1) << 8);
}

/**
 * Set WORD to DSP reg
 */
void Spc_SetDSPRegWord(uint16_t address, uint16_t value)
{
	Spc_SetDSPRegByte(address, (uint8_t) value);
	Spc_SetDSPRegByte(address + 1, (uint8_t) (value >> 8));
}

/**
 * Get byte from SPC700 I/O port
 */
uint8_t Spc_GetSPC700PortByte(uint16_t port)
{
	if (!Spc_IsSPC700Port(port))
	{
		return 0;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	return (uint32_t) SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_GET_PORT | port), (LPARAM)0);
}

/**
 * Set byte to SPC700 I/O port
 */
void Spc_SetSPC700PortByte(uint16_t port, uint8_t value)
{
	if (!Spc_IsSPC700Port(port))
	{
		return;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_SET_PORT | port), (LPARAM)value);
}

/**
 * Get Script700 work register
 */
uint32_t Spc_GetScript700WorkReg(uint16_t reg)
{
	if (!Spc_IsScript700WorkReg(reg))
	{
		return 0;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	return (uint32_t) SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_GET_WORK | reg), (LPARAM)0);
}

/**
 * Set Script700 work register
 */
void Spc_SetScript700WorkReg(uint16_t reg, uint32_t value)
{
	if (!Spc_IsScript700WorkReg(reg))
	{
		return;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_SET_WORK | reg), (LPARAM)value);
}

/**
 * Get Script700 CmpParam
 */
uint32_t Spc_GetScript700CmpParam(uint16_t reg)
{
	if (!Spc_IsScript700CmpParam(reg))
	{
		return 0;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return 0;
	}

	return (uint32_t) SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_GET_CMP | reg), (LPARAM)0);
}

/**
 * Set Script700 CmpParam
 */
void Spc_SetScript700CmpParam(uint16_t reg, uint32_t value)
{
	if (!Spc_IsScript700CmpParam(reg))
	{
		return;
	}

	HWND hSpcHost = Spc_GetHostWindow();
	if (hSpcHost == NULL)
	{
		return;
	}

	SendMessage(hSpcHost, WM_APP_MESSAGE, (WPARAM)(WM_APP_SET_CMP | reg), (LPARAM)value);
}
