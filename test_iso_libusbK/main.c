#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <libusbk.h>

#define SILAB_VID 				0x10C4
#define SILAB_PID 				0x8846
#define EP_TRANSFER 			0x83
#define EP_PACKET_SIZE			0x10
#define ISO_PACKETS_PER_XFER 	1

#define ISO_CALC_CONTEXT_SIZE(mNumOfIsoPacket) (sizeof(KISO_CONTEXT) + (sizeof(KISO_PACKET)*mNumOfIsoPacket))
#define ISO_CALC_DATABUFFER_SIZE(mNumOfIsoPackets, mIsoPacketSize) (mNumOfIsoPackets*mIsoPacketSize)

static BOOL KUSB_API ShowDevicesCB(KLST_HANDLE DeviceList,
								   KLST_DEVINFO_HANDLE DeviceInfo,
								   PVOID MyContext)
{
	printf("%04X:%04X %s - %s\n",
		DeviceInfo->Common.Vid,
		DeviceInfo->Common.Pid,
		DeviceInfo->DeviceDesc,
		DeviceInfo->Mfg);
	return TRUE;
}

int main(int argc, char **argv)
{
	//For List Device
	KLST_HANDLE deviceList = NULL;
	KLST_DEVINFO_HANDLE  deviceInfo = NULL;
	DWORD errorCode = ERROR_SUCCESS;
	ULONG count = 0;
	
	//For USB Handle
	KUSB_HANDLE handle = NULL;
	PKISO_CONTEXT isoCtx = NULL;
	KOVL_HANDLE ovlkHandle = NULL;
	KOVL_POOL_HANDLE ovlPool = NULL;
	
	//For iso packet
	UCHAR pipeId = EP_TRANSFER;
	UCHAR dataBuffer[ISO_CALC_DATABUFFER_SIZE(ISO_PACKETS_PER_XFER, EP_PACKET_SIZE)];
	ULONG transferred = 0;
	LONG posPacket;
	
	//General Control
	BOOL success;
	
	/*
	Initialize a new LstK (device list) handle.
	The list is polulated with all usb devices libusbK can access.
	*/
	if (!LstK_Init(&deviceList, 0))
	{
		errorCode = GetLastError();
		printf("An error occured getting the device list. errorCode=%08Xh\n", errorCode);
		return errorCode;
	}
	
	// Get the number of devices contained in the device list.
	LstK_Count(deviceList, &count);
	if (!count)
	{
		printf("No devices connected.\n");
		LstK_Free(deviceList);
		return ERROR_DEVICE_NOT_CONNECTED;
	}
	
	// Show all devices using the enumerate function.
	LstK_Enumerate(deviceList, ShowDevicesCB, NULL);
	
	if (LstK_FindByVidPid(deviceList, SILAB_VID, SILAB_PID, &deviceInfo))
		printf("device connected!\n");
	else
		printf("device not found.\n");	
		
	//Initialize the device. this create the physical usb handle
	if (!UsbK_Init(&handle, deviceInfo))
	{
		errorCode = GetLastError();
		printf("UsbK_Init failed. Errorcode: %08Xh\n", errorCode);
		goto Done;
	}
	printf("device open successfully!\n");

	//initialize a new iso context handle
	IsoK_Init(&isoCtx, ISO_PACKETS_PER_XFER, 0);
	// set the iso pakcet offsets
	IsoK_SetPackets(isoCtx, EP_PACKET_SIZE);
	//initialize a new ovlK pool handle
	OvlK_Init(&ovlPool, handle, 4, 0);
	OvlK_Acquire(&ovlkHandle, ovlPool);
	
	UsbK_ResetPipe(handle, pipeId);
	
	UsbK_IsoReadPipe(handle, pipeId, dataBuffer, sizeof(dataBuffer), ovlkHandle, isoCtx);
	
	success = OvlK_WaitAndRelease(ovlkHandle, 1000, &transferred);
	if (!success)
	{
		errorCode = GetLastError();
		printf("IsoReadPipe faile. errorcode: %08Xh\n", errorCode);
		goto Done;
	}
	printf("ISO StartFrame:%08Xh ErrorCount=%u Transferred=%u\n",
		isoCtx->StartFrame, isoCtx->ErrorCount, transferred);
		
	for (posPacket = 0; posPacket < isoCtx->NumberOfPackets; posPacket++)
	{
		LONG posByte;
		PKISO_PACKET isoPacket = &isoCtx->IsoPackets[posPacket];
		
		printf("  IsoPacket[%d] Length=%u Status=%08Xh\n", posPacket, isoPacket->Length, isoPacket->Status);
		printf("  Data:");
		
		for (posByte = 0; posByte < isoPacket->Length; posByte++)
		{
			printf("%u", dataBuffer[isoPacket->Offset + posByte]);
			if ((posByte & 0xF) == 0xF) printf("\n    ");
		}
		printf("\n");
		if ((posByte & 0xF) != 0xF) printf("\n");
	}
Done:
	//free usb handle
	UsbK_Free(handle);
	
	//free device list
	LstK_Free(deviceList);
	IsoK_Free(isoCtx);
	OvlK_Free(ovlPool);
	
	return errorCode;
}
