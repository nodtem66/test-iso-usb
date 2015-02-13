#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include <signal.h>
#include <errno.h>

//----------------------------------------------------------------------------
// Constant 
//----------------------------------------------------------------------------
#define ID_VENDOR		0x10C4
#define ID_PRODUCT		0x8846
#define EP_ISO_IN		0x83
#define EP_BULK_IN		0x83
#define NUM_ISO_PACKET	0x01
#define EP_PACKET_SIZE	0x10
//----------------------------------------------------------------------------
// Global variable
//----------------------------------------------------------------------------
static int do_exit = 0;
static libusb_device_handle * LIBUSB_CALL dev_handle = NULL;
static int counter = 0;
uint8_t buffer[NUM_ISO_PACKET * EP_PACKET_SIZE];
//----------------------------------------------------------------------------
void signal_handler (int param)
{
	printf("SIGNAL %d", param);
	do_exit = 1;
}

static void scan_devices()
{
	libusb_device **devs;
	libusb_device *dev;
	int i=0, cnt;
	
	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0) 
	{
		return;
	}
	
	while ((dev = devs[i++]) != NULL)
	{
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0)
		{
			printf("failed to get device descriptor\n");
			return;
		}
		printf("%04X:%04X (bus:%d, device: %d)\n", 
			desc.idVendor,
			desc.idProduct,
			libusb_get_bus_number(dev),
			libusb_get_device_address(dev));
		if (desc.idVendor == ID_VENDOR && desc.idProduct == ID_PRODUCT)
		{
			printf("    endpoint: %02X (%d %d)\n",
				EP_ISO_IN,
				libusb_get_max_packet_size(dev, EP_ISO_IN),
				libusb_get_max_iso_packet_size(dev, EP_ISO_IN));
		}
	}
	libusb_free_device_list(devs, 1);
}
static void LIBUSB_CALL cb_xfr(struct libusb_transfer *xfr)
{
		uint16_t i, len = 0;
		counter++;
		
		//Initalize buffer 16byte
		//uint8_t *recv = malloc(EP_PACKET_SIZE * NUM_ISO_PACKET);
		//uint8_t *recv_next = recv;
		
		if (xfr->status != LIBUSB_TRANSFER_COMPLETED)
		{
			printf("Error transfer status: %d\n", xfr->status);
			libusb_free_transfer(xfr);
			exit(3);
		}
		
		printf("[%d] XFR length:%u, actual_length:%u\n", counter, xfr->length, xfr->actual_length);
		if (xfr->type == LIBUSB_TRANSFER_TYPE_ISOCHRONOUS)
		{
			for (i=0; i < xfr->num_iso_packets; i++)
			{
				struct libusb_iso_packet_descriptor *pack = &xfr->iso_packet_desc[i];
				if (pack->status != LIBUSB_TRANSFER_COMPLETED)
				{
					printf("Error pack %u status %d\n",i, pack->status);
					//exit(5);
				} else {
					
					printf("pack%u length:%u, actual_length:%u\n",i, pack->length, pack->actual_length);
					//const uint8_t *data = libusb_get_iso_packet_buffer_simple(xfr, i);
					for (i=0; i< pack->actual_length; i++) 
					{
						printf("%02d ", xfr->buffer[i]);
					}
				printf("\n");
				}
			}
		}
		
		//Sanity check. If this is true, we've overflowed the recv buffer.
		if (len > EP_PACKET_SIZE * xfr->num_iso_packets)
		{
			printf("Error: incoming transfer had more data than we thought.\n");
			exit(1);
		}
		
		//printf("re-submit transfer\n");
		if (libusb_submit_transfer(xfr) < 0)
		{
			printf("Error re-submit transfer\n");
			exit(1);
		}
}

static int test_iso(uint8_t endpoint)
{
	static struct libusb_transfer *xfr;
	xfr = libusb_alloc_transfer(NUM_ISO_PACKET);
	if (!xfr)
		return LIBUSB_ERROR_NO_MEM;
	
	memset(buffer, 3, sizeof(buffer));
	
	libusb_fill_iso_transfer(xfr, dev_handle, endpoint, buffer, 
		sizeof (buffer), NUM_ISO_PACKET, cb_xfr, NULL, 0);
		
	libusb_set_iso_packet_lengths(xfr, EP_PACKET_SIZE);
	 
	//libusb_fill_bulk_transfer(xfr, dev_handle, endpoint, buffer, 64, cb_xfr, NULL, 0);

	return libusb_submit_transfer(xfr);
}

int main(void)
{
	int err_code;
	//Signal Handler
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	//Init LIBUSB
	err_code = libusb_init(NULL);
	if (err_code != 0) 
	{
		printf("Error initializing libusb %s\n", libusb_error_name(err_code));
		exit(1);
	}
	
	//Set debug level
	libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);
	
	//PRINT VERSION LIBUSB
	const struct libusb_version *version = libusb_get_version();
	printf("libusb version %d.%d.%d\n", version->major, version->minor, version->micro);
	
	//PRINT DEVICES LIST
	scan_devices();
	
	// GET USB DEVICE HANDLE
	dev_handle = libusb_open_device_with_vid_pid(NULL, ID_VENDOR, ID_PRODUCT);
	if (!dev_handle)
	{
		printf("Error finding USB device\n");
		goto out;
	}
	printf("found USB device %02X:%02X\n", ID_VENDOR, ID_PRODUCT);
	
	// Lock interface 1
	printf("claiming interface 0\n");
	err_code = libusb_claim_interface(dev_handle, 0);
	if (err_code < 0) 
	{
		printf("Error claiming interface 0\n");
		goto out;
	}
	err_code = test_iso(EP_ISO_IN);
	if (err_code < 0)
	{
		printf("Error test isochronous\n code:%d %s\n", err_code, libusb_error_name(err_code));
	}
	
	// handle pending event in blocking mode
	//*
	printf("start pending loop\n");
	while (!do_exit)
	{
		err_code = libusb_handle_events(NULL);
		if (err_code != LIBUSB_SUCCESS)
			break;
	}//*/
	
	// Release interface 1
	printf("release interface 1\n");
	libusb_release_interface(dev_handle, 0);
	//EXIT LIBUSB
out:
	if (dev_handle)
	{
			libusb_close(dev_handle);
	}
	libusb_exit(NULL);
	return err_code;
}
