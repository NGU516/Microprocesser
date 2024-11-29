// serial.c

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "CGI_get_data.h"
#include "serial.h"

HANDLE openPort(char* port, char* msg) 
{
	HANDLE		hSerial;
	wchar_t		portNo[20], portBuf[20];
	DCB			dcbSerialParams = { 0, };	// DCB for Serial port
	COMMTIMEOUTS timeOuts = { 0 };

	MultiBytetoWideChar(CP_ACP, 0, port, 8, portBuf, 16);
	swprintf_s(portNo, 20, L"\\\.\\%s", portBuf);

	// Open the Serial COM port
	hSerial = CreateFile(portNo,
		GENERIC_READ | GENERIC_WRITE,		// Read/Write Access
		0,									// No Sharing, ports cant be shared
		NULL,								// No Security
		OPEN_EXISTING,						// Open existing port only
		0,									// Non Overlapped I/O
		NULL);								// NULL for Comm Devices

	if (hSerial == INVALID_HANDLE_VALUE) {
		sprintf_s(msg, 64, "\nError: Open the serial port(%s)...\n\n", port);
		return INVALID_HANDLE_VALUE;
	}

	// Configure the Serial port parameter(DCB structure)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == FALSE) {
		strcpy_s(msg, 64, "\nError: Get the COM state...\n\n");
		CloseHandle(hSerial);	return INVALID_HANDLE_VALUE;
	}

	dcbSerialParams.BaudRate = CBR_115200;		// BaudRate = 115200
	dcbSerialParams.ByteSize = 8;				// ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;		// StopBits = 1
	dcbSerialParams.Parity = NOPARITY;			// Parity = None
	if (SetCommState(hSerial, &dcbSerialParams) == FALSE) {
		strcpy_s(msg, 64, "\nError: Set the COM state(DCB Structure)...\n\n");
		CloseHandle(hSerial);	return INVALID_HANDLE_VALUE;
	}
	// Set timeout to the Serial port
	timeOuts.ReadIntervalTimeout = 10;
	timeOuts.ReadTotalTimeoutConstant = 10;
	timeOuts.ReadTotalTimeoutMultiplier = 0;
	timeOuts.WriteTotalTimeoutConstant = 10;
	timeOuts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hSerial, &timeOuts) == FALSE) {
		strcpy_s(msg, 32, "\nError: Set timeout to the Serial port...\n");
		CloseHandle(hSerial); return INVALID_HANDLE_VALUE;
	}
	return hSerial;

}	// openPort() 함수 끝

char* headLine[MAX_N_HEAD], * tailLine[MAX_N_TAIL];

DWORD readData(HANDLE hport, int N_HEAD, int N_TAIL, char* data, char* msg)
{
	char ch, * buf, i;
	DWORD nBytesRead;
	int nHead = 0, nTail = 0, cLen = 0;

	for (i = 0; i < N_HEAD; i++)
		headLine[i] = (char*)malloc(CHR_P_LINE);
	for (i = 0; i < N_TAIL; i++)
		tailLine[i] = (char*)malloc(CHR_P_LINE);

	for (buf = headLine[nHead++]; ; ) {
		if (ReadFile(hport, &ch, sizeof(ch), &nBytesRead, NULL) == FALSE) {
			strcpy_s(msg, 64, "\nError: Read from the Serial port...\n\n");
			return 0;
		}
		if (!nBytesRead) {
			buf[cLen] = '\0';
			break;
		}
		buf[cLen++] = ch;

		if (ch == '\n') {
			buf[cLen] = '\0';
			if (nHead < N_HEAD) buf = headLine[nHead++];
			else				buf = tailLine[nTail++ % N_TAIL];
			cLen = 0;
		}
	}

	for (i = 0, buf = data; i < nHead; i++) {
		sprintf_s(buf, 128, "%s", headLine[i]);
		buf += (int)strlen(buf);
	}

	sprintf_s(buf, 128, ":::\n");
	buf += (int)strlen(buf);

	if (nTail > N_TAIL) i = nTail % N_TAIL, nTail = N_TAIL;
	else				i = 0;

	for (int j = 0; j < nTail; j++) {
		sprintf_s(buf, 128, "%s", tailLine[i++ % N_TAIL]);
		buf += strlen(buf);
	}

	for (i = 0; i < N_HEAD; i++)
		free(headLine[i]);

	for (i = 0; i < N_TAIL; i++)
		free(tailLine[i]);

	return nHead + nTail;
}	// readData() 함수 끝

DWORD  writeCommand(HANDLE hPort, char* cmd, char* msg)
{
	DWORD	Status, nBytesWritten;
	char	buf[64];

	sprintf_s(buf, 64, "%c%s%n%c", '\33', cmd, '\33');		// ESC for no echo

	Status = WriteFile(hPort,
		buf,
		(DWORD)strlen(buf),
		&nBytesWritten,
		NULL
	);

	if (Status == FALSE) {
		strcpy_s(msg, 64, "\nError: Write to the Serial port ...\n\n");
		return 0;
	}
	return nBytesWritten;
}	// writeCommand() 함수 끝