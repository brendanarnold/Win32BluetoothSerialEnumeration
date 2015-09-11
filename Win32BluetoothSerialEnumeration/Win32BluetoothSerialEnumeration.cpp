

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include "Objbase.h"

int main(int argc, char *argv[], char *envp[])
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;
	CLSID guid;
	//Class = Ports
	//ClassGuid = { 4d36e978 - e325 - 11ce - bfc1 - 08002be10318}


	LPCOLESTR  guidString = L"{86E0D1E0-8089-11D0-9CE4-08003E301F73}";
	HRESULT a = CLSIDFromString(guidString, &guid);
	// Create a HDEVINFO with all present devices.
	hDevInfo = SetupDiGetClassDevs(&guid,
		0, // Enumerator
		0,
		DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		// Insert error handling here.
		return 1;
	}

	// Enumerate through all devices in Set.

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i = 0;SetupDiEnumDeviceInfo(hDevInfo, i,
		&DeviceInfoData);i++)
	{
		DWORD DataT;
		LPTSTR buffer = (LPTSTR)LocalAlloc(LPTR, 4096 * 2);
		DWORD buffersize = 4096;

		LPBYTE buffer2 = (LPBYTE)LocalAlloc(LPTR, 4096 * 2);



		SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_FRIENDLYNAME,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize);





		if (wcsstr(buffer, L"Standard Serial over Bluetooth link"))
		{
			SetupDiGetDeviceRegistryProperty(
				hDevInfo,
				&DeviceInfoData,
				SPDRP_CAPABILITIES,
				&DataT,
				(PBYTE)buffer2,
				buffersize,
				&buffersize);

			wprintf(L"Result:%s - ", buffer);
			int a = *buffer2;
			if (a == 0xC0)
			{
				wprintf(L"Incoming");
			}
			else if (a == 0xC4)
			{
				wprintf(L"Outgoing");
			}

			wprintf(L"\n", buffer);

		}

		if (buffer) LocalFree(buffer);
	}


	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		// Insert error handling here.
		return 1;
	}

	//  Cleanup
	SetupDiDestroyDeviceInfoList(hDevInfo);
	getchar();
	return 0;
}
