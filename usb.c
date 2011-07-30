#include "usb.h"

static void
libusb_perror(int error_code)
{
	fprintf(stderr, "%s(): libusb error (%d)\n", __func__, error_code);
}

int 
usb_initialize()
{
	int r;

	/* This will only work if libusb is compiled with debug support. Also, this value will be
	 * owerriden with LIBUSB_DEBUG environment variable */
	if (debug)
		libusb_set_debug(NULL, 3);

	r = libusb_init(NULL);
	if (r != 0) {
		libusb_perror(r);
		return r;
	}

	return 0;
}

int
find_usb_device(struct UsbIdent ident, libusb_device *dev_found)
{
	int i;
	ssize_t cnt;
	libusb_device **devs;
	libusb_device *dev;

	cnt = libusb_get_device_list(NULL, &devs);
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return 1;
		}

		if (desc.idVendor == ident.idVendor && desc.idProduct == ident.idProduct) {
			printf("device found (bus %d, device %d)\n",
				libusb_get_bus_number(dev), libusb_get_device_address(dev));
			break;
		}
	}

	dev_found = dev;

	return 0;
}

