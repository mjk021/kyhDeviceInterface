#include <stdio.h>
#include <errno.h>
#include <usb.h>

int main(void)
{
  int rv;
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *udev;

  usb_init();

  usb_find_busses();
  usb_find_devices();

  printf("bus/device  idVendor/idProduct\n");
  for (bus = usb_busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
	if(dev->descriptor.idVendor != 0x096e) continue;

	errno = 0;
      udev = usb_open(dev);
      printf("udev:%p errno:%d\n",udev,errno);
	rv = usb_claim_interface(udev,0);
      printf("rv:%d errno:%d\n",rv,errno);
	usb_close (udev);
    }
  }

  return 0;
}

