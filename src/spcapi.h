/*
 * SPCAPI - Message Macros for SNES SPC700 Player
 */

#ifndef SPCAPI_H_INCLUDED
#define SPCAPI_H_INCLUDED

#include <windows.h>
#include <stdint.h>

// derived from spcplay.dpr
#define WM_APP_MESSAGE      0x8000              // notify message
#define WM_APP_GET_DSP      0xF1000000          // DSP read             ($F100??XX, XX:Pointer)
#define WM_APP_SET_DSP      0xF1010000          // DSP write            ($F101??XX, XX:Pointer, l:Value)
#define WM_APP_GET_PORT     0xF1100000          // I/O read             ($F110???X, X:Pointer)
#define WM_APP_SET_PORT     0xF1110000          // I/O write            ($F111???X, X:Pointer, l:Value)
#define WM_APP_GET_RAM      0xF1200000          // RAM read             ($F120XXXX, XXXX:Pointer)
#define WM_APP_SET_RAM      0xF1210000          // RAM write            ($F121XXXX, XXXX:Pointer, l:Value)
#define WM_APP_GET_WORK     0xF1300000          // Script700 work read  ($F130???X, X:Pointer)
#define WM_APP_SET_WORK     0xF1310000          // Script700 work write ($F131???X, X:Pointer, l:Value)
#define WM_APP_GET_CMP      0xF1400000          // Script700 cmp read   ($F140???X, X:Pointer)
#define WM_APP_SET_CMP      0xF1410000          // Script700 cmp write  ($F141???X, X:Pointer, l:Value)

HWND Spc_GetHostWindow(void);
bool Spc_IsAPURAMAddress(uint16_t address);
bool Spc_IsDSPRegAddress(uint16_t address);
bool Spc_IsSPC700Port(uint16_t port);
bool Spc_IsScript700WorkReg(uint16_t reg);
bool Spc_IsScript700CmpParam(uint16_t reg);
uint8_t Spc_GetAPURAMByte(uint16_t address);
void Spc_SetAPURAMByte(uint16_t address, uint8_t value);
uint16_t Spc_GetAPURAMWord(uint16_t address);
void Spc_SetAPURAMWord(uint16_t address, uint16_t value);
uint32_t Spc_GetAPURAMDWord(uint16_t address);
void Spc_SetAPURAMDWord(uint16_t address, uint32_t value);
uint8_t Spc_GetDSPRegByte(uint16_t address);
void Spc_SetDSPRegByte(uint16_t address, uint8_t value);
uint16_t Spc_GetDSPRegWord(uint16_t address);
void Spc_SetDSPRegWord(uint16_t address, uint16_t value);
uint8_t Spc_GetSPC700PortByte(uint16_t address);
void Spc_SetSPC700PortByte(uint16_t address, uint8_t value);
uint32_t Spc_GetScript700WorkReg(uint16_t reg);
void Spc_SetScript700WorkReg(uint16_t reg, uint32_t value);
uint32_t Spc_GetScript700CmpParam(uint16_t reg);
void Spc_SetScript700CmpParam(uint16_t reg, uint32_t value);

#endif // !SPCAPI_H_INCLUDED
