/*
 * Linux USB support
 *
 * Copyright (c) 2000-2003 Johannes Erdfelt <johannes@erdfelt.com>
 *
 * This library is covered by the LGPL, read LICENSE for details.
 */

#include <stdlib.h>	/* getenv, etc */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <dirent.h>

#include "linux.h"
#include "usbi.h"

static char usb_path[PATH_MAX + 1] = "";

int usb_os_open(usb_dev_handle *dev)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  char filename[PATH_MAX + 1];

  snprintf(filename, sizeof(filename) - 1, "%s/%s/%s",
    usb_path, dev->bus->dirname, dev->device->filename);

  dev->fd = open(filename, O_RDWR);
  if (dev->fd < 0) {
    dev->fd = open(filename, O_RDONLY);
    if (dev->fd < 0)
      USB_ERROR_STR(-errno, "failed to open %s: %s",
	filename, strerror(errno));
  }

  return 0;
}

int usb_os_close(usb_dev_handle *dev)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  if (dev->fd < 0)
    return 0;

  if (close(dev->fd) == -1)
    /* Failing trying to close a file really isn't an error, so return 0 */
    USB_ERROR_STR(0, "tried to close device fd %d: %s", dev->fd,
	strerror(errno));

  return 0;
}

int usb_set_configuration(usb_dev_handle *dev, int configuration)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_SETCONFIG, &configuration);
  if (ret < 0)
    USB_ERROR_STR(-errno, "could not set config %d: %s", configuration,
	strerror(errno));

  dev->config = configuration;

  return 0;
}

int usb_claim_interface(usb_dev_handle *dev, int interface)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_CLAIMINTF, &interface);
  if (ret < 0) {
    if (errno == EBUSY && usb_debug > 0)
      fprintf(stderr, "Check that you have permissions to write to %s/%s and, if you don't, that you set up hotplug (http://linux-hotplug.sourceforge.net/) correctly.\n", dev->bus->dirname, dev->device->filename);

    USB_ERROR_STR(-errno, "could not claim interface %d: %s", interface,
	strerror(errno));
  }

  dev->interface = interface;

  return 0;
}

int usb_release_interface(usb_dev_handle *dev, int interface)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_RELEASEINTF, &interface);
  if (ret < 0)
    USB_ERROR_STR(-errno, "could not release intf %d: %s", interface,
    	strerror(errno));

  dev->interface = -1;

  return 0;
}

int usb_set_altinterface(usb_dev_handle *dev, int alternate)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;
  struct usb_setinterface setintf;

  if (dev->interface < 0)
    USB_ERROR(-EINVAL);

  setintf.interface = dev->interface;
  setintf.altsetting = alternate;

  ret = ioctl(dev->fd, IOCTL_USB_SETINTF, &setintf);
  if (ret < 0)
    USB_ERROR_STR(-errno, "could not set alt intf %d/%d: %s",
	dev->interface, alternate, strerror(errno));

  dev->altsetting = alternate;

  return 0;
}

/* Linux usbdevfs has a limit of one page size per read/write. 4096 is */
/* the most portable maximum we can do for now */
#define MAX_READ_WRITE	4096

int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int length,
	int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_bulktransfer bulk;
  int ret, sent = 0;

  /* Ensure the endpoint address is correct */
  ep &= ~USB_ENDPOINT_IN;

  do {
    bulk.ep = ep;
    bulk.len = length - sent;
    if (bulk.len > MAX_READ_WRITE)
      bulk.len = MAX_READ_WRITE;
    bulk.timeout = timeout;
    bulk.data = (unsigned char *)bytes + sent;

    ret = ioctl(dev->fd, IOCTL_USB_BULK, &bulk);
    if (ret < 0)
      USB_ERROR_STR(-errno, "error writing to bulk endpoint %d: %s",
	ep, strerror(errno));

    sent += ret;
  } while (ret > 0 && sent < length);

  return sent;
}

int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
	int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_bulktransfer bulk;
  int ret, retrieved = 0, requested;

  /* Ensure the endpoint address is correct */
  ep |= USB_ENDPOINT_IN;

  do {
    bulk.ep = ep;
    requested = size - retrieved;
    if (requested > MAX_READ_WRITE)
      requested = MAX_READ_WRITE;
    bulk.len = requested;
    bulk.timeout = timeout;
    bulk.data = (unsigned char *)bytes + retrieved;

    ret = ioctl(dev->fd, IOCTL_USB_BULK, &bulk);
    if (ret < 0)
      USB_ERROR_STR(-errno, "error reading from bulk endpoint 0x%x: %s",
	ep, strerror(errno));

    retrieved += ret;
  } while (ret > 0 && retrieved < size && ret == requested);

  return retrieved;
}

int usb_control_msg(usb_dev_handle *dev, int requesttype, int request,
	int value, int index, char *bytes, int size, int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_ctrltransfer ctrl;
  int ret;

  ctrl.bRequestType = requesttype;
  ctrl.bRequest = request;
  ctrl.wValue = value;
  ctrl.wIndex = index;
  ctrl.wLength = size;

  ctrl.data = bytes;
  ctrl.timeout = timeout;

  ret = ioctl(dev->fd, IOCTL_USB_CONTROL, &ctrl);
  if (ret < 0)
    USB_ERROR_STR(-errno, "error sending control message: %s", strerror(errno));

  return ret;
}

/* Reading and writing are the same except for the endpoint */
static int usb_interrupt_transfer(usb_dev_handle *dev, int ep, char *bytes,
	int size, int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_urb int_urb;
  void *context;
  int ret;
  unsigned int bytesdone = 0, requested;
  struct timeval tv, tv_ref, tv_now;
  int waiting;

  /*
   * FIXME: Packetize large buffers here. 2.4 HCDs (atleast, haven't checked
   * 2.5 HCDs yet) don't handle multi-packet Interrupt transfers. So we need
   * to lookup the endpoint packet size and packetize appropriately here.
   */

  /* get actual time, and add the timeout value. The result is the absolute
   * time where we have to quit waiting for an interrupt message.
   */
  gettimeofday(&tv_ref, NULL);
  tv_ref.tv_sec = tv_ref.tv_sec + timeout / 1000;
  tv_ref.tv_usec = tv_ref.tv_usec + (timeout % 1000) * 1000;

  if (tv_ref.tv_usec > 1e6) {
    tv_ref.tv_usec -= 1e6;
    tv_ref.tv_sec++;
  }

  do {
    requested = size - bytesdone;
    if (requested > MAX_READ_WRITE)
      requested = MAX_READ_WRITE;

    int_urb.type = USB_URB_TYPE_INTERRUPT;
    int_urb.endpoint = ep;
    int_urb.flags = 0;
    int_urb.buffer = bytes + bytesdone;
    int_urb.buffer_length = requested;
    int_urb.usercontext = (void *)ep;
    int_urb.signr = 0;
    int_urb.actual_length = 0;

    ret = ioctl(dev->fd, IOCTL_USB_SUBMITURB, &int_urb);
    if (ret < 0) {
      USB_ERROR_STR(-errno, "error submitting URB: %s", strerror(errno));
      return ret;
    }

    waiting = 1;
    while (((ret = ioctl(dev->fd, IOCTL_USB_REAPURBNDELAY, &context)) == -1) && waiting) {
      tv.tv_sec = 0;
      tv.tv_usec = 1000; // 1 msec
      select(0, NULL, NULL, NULL, &tv); //sub second wait

      /* compare with actual time, as the select timeout is not that precise */
      gettimeofday(&tv_now, NULL);

      if ((tv_now.tv_sec >= tv_ref.tv_sec) && (tv_now.tv_usec >= tv_ref.tv_usec))
        waiting = 0;
    }

    /*
     * If there was an error, that wasn't EAGAIN (no completion), then
     * something happened during the reaping and we should return that
     * error now
     */
    if (ret < 0 && errno != EAGAIN)
      USB_ERROR_STR(-errno, "error reaping interrupt URB: %s", strerror(errno));

    bytesdone += int_urb.actual_length;
  } while (ret > 0 && bytesdone < size && ret == requested);

  /* If the URB didn't complete in success or error, then let's unlink it */
  if (ret < 0) {
    int rc;

    if (!waiting)
      rc = -ETIMEDOUT;
    else
      rc = int_urb.status;

    ret = ioctl(dev->fd, IOCTL_USB_DISCARDURB, &int_urb);
    if (ret < 0 && errno != EINVAL && usb_debug >= 1)
      fprintf(stderr, "error discarding interrupt URB: %s", strerror(errno));

    return rc;
  }

  return bytesdone;
}

int usb_interrupt_write(usb_dev_handle *dev, int ep, char *bytes, int size,
	int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  /* Ensure the endpoint address is correct */
  return usb_interrupt_transfer(dev, ep & ~USB_ENDPOINT_IN, bytes, size,
		timeout);
}

int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size,
	int timeout)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  /* Ensure the endpoint address is correct */
  return usb_interrupt_transfer(dev, ep | USB_ENDPOINT_IN, bytes, size,
		timeout);
}

int usb_os_find_busses(struct usb_bus **busses)
{
  struct usb_bus *fbus = NULL;
  DIR *dir;
  struct dirent *entry;

  dir = opendir(usb_path);
  if (!dir)
    USB_ERROR_STR(-errno, "couldn't opendir(%s): %s", usb_path,
	strerror(errno));

  while ((entry = readdir(dir)) != NULL) {
    struct usb_bus *bus;

    /* Skip anything starting with a . */
    if (entry->d_name[0] == '.')
      continue;

    if (!strchr("0123456789", entry->d_name[strlen(entry->d_name) - 1])) {
      if (usb_debug >= 2)
        fprintf(stderr, "usb_os_find_busses: Skipping non bus directory %s\n",
		entry->d_name);
      continue;
    }

    bus = malloc(sizeof(*bus));
    if (!bus)
      USB_ERROR(-ENOMEM);

    memset((void *)bus, 0, sizeof(*bus));

    strncpy(bus->dirname, entry->d_name, sizeof(bus->dirname) - 1);
    bus->dirname[sizeof(bus->dirname) - 1] = 0;

    LIST_ADD(fbus, bus);

    if (usb_debug >= 2)
       fprintf(stderr, "usb_os_find_busses: Found %s\n", bus->dirname);
  }

  closedir(dir);

  *busses = fbus;

  return 0;
}

int usb_os_find_devices(struct usb_bus *bus, struct usb_device **devices)
{
  if (NULL == bus)
    USB_ERROR(-ENODEV);

  struct usb_device *fdev = NULL;
  DIR *dir;
  struct dirent *entry;
  char dirpath[PATH_MAX + 1];

  snprintf(dirpath, PATH_MAX, "%s/%s", usb_path, bus->dirname);

  dir = opendir(dirpath);
  if (!dir)
    USB_ERROR_STR(-errno, "couldn't opendir(%s): %s", dirpath,
	strerror(errno));

  while ((entry = readdir(dir)) != NULL) {
    char filename[PATH_MAX + 1];
    struct usb_device *dev;
    int i, fd, ret;

    /* Skip anything starting with a . */
    if (entry->d_name[0] == '.')
      continue;

    dev = malloc(sizeof(*dev));
    if (!dev)
      USB_ERROR(-ENOMEM);

    memset((void *)dev, 0, sizeof(*dev));

    dev->bus = bus;

    strncpy(dev->filename, entry->d_name, sizeof(dev->filename) - 1);
    dev->filename[sizeof(dev->filename) - 1] = 0;

    snprintf(filename, sizeof(filename) - 1, "%s/%s", dirpath, entry->d_name);
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
      if (usb_debug >= 2)
        fprintf(stderr, "usb_os_find_devices: Couldn't open %s\n",
		filename);

      free(dev);
      dev = NULL;
      continue;
    }

    ret = read(fd, (void *)&dev->descriptor, sizeof(dev->descriptor));
    if (ret < 0) {
      if (usb_debug)
        fprintf(stderr, "usb_os_find_devices: Couldn't read descriptor\n");

      free(dev);
      dev = NULL;

      goto err;
    }

    LIST_ADD(fdev, dev);

    if (usb_debug >= 2)
      fprintf(stderr, "usb_os_find_devices: Found %s on %s\n",
		dev->filename, bus->dirname);

    /* Now try to fetch the rest of the descriptors */
    if (dev->descriptor.bNumConfigurations > USB_MAXCONFIG)
      /* Silent since we'll try again later */
      goto err;

    if (dev->descriptor.bNumConfigurations < 1)
      /* Silent since we'll try again later */
      goto err;

    dev->config = (struct usb_config_descriptor *)malloc(dev->descriptor.bNumConfigurations * sizeof(struct usb_config_descriptor));
    if (!dev->config)
      /* Silent since we'll try again later */
      goto err;

    memset(dev->config, 0, dev->descriptor.bNumConfigurations *
          sizeof(struct usb_config_descriptor));

    for (i = 0; i < dev->descriptor.bNumConfigurations; i++) {
      char buffer[8], *bigbuffer;
      struct usb_config_descriptor *desc = (struct usb_config_descriptor *)buffer;

      /* Get the first 8 bytes so we can figure out what the total length is */
      ret = read(fd, (void *)buffer, 8);
      if (ret < 8) {
        if (usb_debug >= 1) {
          if (ret < 0)
            fprintf(stderr, "Unable to get descriptor (%d)\n", ret);
          else
            fprintf(stderr, "Config descriptor too short (expected %d, got %d)\n", 8, ret);
        }

        goto err;
      }

      USB_LE16_TO_CPU(desc->wTotalLength);

      bigbuffer = malloc(desc->wTotalLength);
      if (!bigbuffer) {
        if (usb_debug >= 1)
          fprintf(stderr, "Unable to allocate memory for descriptors\n");
        goto err;
      }

      /* Copy over the first 8 bytes we read */
      memcpy(bigbuffer, buffer, 8);

      ret = read(fd, (void *)(bigbuffer + 8), desc->wTotalLength - 8);
      if (ret < desc->wTotalLength - 8) {
        if (usb_debug >= 1) {
          if (ret < 0)
            fprintf(stderr, "Unable to get descriptor (%d)\n", ret);
          else
            fprintf(stderr, "Config descriptor too short (expected %d, got %d)\n", desc->wTotalLength, ret);
        }

        free(bigbuffer);
	bigbuffer = NULL;
        goto err;
      }

      ret = usb_parse_configuration(&dev->config[i], bigbuffer);
      if (usb_debug >= 2) {
        if (ret > 0)
          fprintf(stderr, "Descriptor data still left\n");
        else if (ret < 0)
          fprintf(stderr, "Unable to parse descriptors\n");
      }

      free(bigbuffer);
      bigbuffer = NULL;
    }

err:
    close(fd);
  }

  closedir(dir);

  *devices = fdev;

  return 0;
}

static int check_usb_vfs(const unsigned char *dirname)
{
  if (NULL == dirname)
    USB_ERROR(-ENODEV);

  DIR *dir;
  struct dirent *entry;
  int found = 0;

  dir = opendir(dirname);
  if (!dir)
    return 0;

  while ((entry = readdir(dir)) != NULL) {
    /* Skip anything starting with a . */
    if (entry->d_name[0] == '.')
      continue;

    /* We assume if we find any files that it must be the right place */
    found = 1;
    break;
  }

  closedir(dir);

  return found;
}

void usb_os_init(void)
{
  /* Find the path to the virtual filesystem */
  if (getenv("USB_DEVFS_PATH")) {
    if (check_usb_vfs(getenv("USB_DEVFS_PATH"))) {
      strncpy(usb_path, getenv("USB_DEVFS_PATH"), sizeof(usb_path) - 1);
      usb_path[sizeof(usb_path) - 1] = 0;
    } else if (usb_debug)
      fprintf(stderr, "usb_os_init: couldn't find USB VFS in USB_DEVFS_PATH\n");
  }

  if (!usb_path[0]) {
    if (check_usb_vfs("/dev/bus/usb")) {
      strncpy(usb_path, "/dev/bus/usb", sizeof(usb_path) - 1);
      usb_path[sizeof(usb_path) - 1] = 0;
    } else if (check_usb_vfs("/proc/bus/usb")) {
      strncpy(usb_path, "/proc/bus/usb", sizeof(usb_path) - 1);
      usb_path[sizeof(usb_path) - 1] = 0;
    } else if (check_usb_vfs("/sys/bus/usb")) { /* 2.6 Kernel with sysfs */
      strncpy(usb_path, "/sys/bus/usb", sizeof(usb_path) -1);
      usb_path[sizeof(usb_path) - 1] = 0;
    } else if (check_usb_vfs("/dev/usb")) {
      strncpy(usb_path, "/dev/usb", sizeof(usb_path) - 1);
      usb_path[sizeof(usb_path) - 1] = 0;
    } else
      usb_path[0] = 0;	/* No path, no USB support */
  }

  if (usb_debug) {
    if (usb_path[0])
      fprintf(stderr, "usb_os_init: Found USB VFS at %s\n", usb_path);
    else
      fprintf(stderr, "usb_os_init: No USB VFS found, is it mounted?\n");
  }
}

int usb_resetep(usb_dev_handle *dev, unsigned int ep)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_RESETEP, &ep);
  if (ret)
    USB_ERROR_STR(-errno, "could not reset ep %d: %s", ep,
    	strerror(errno));

  return 0;
}

int usb_clear_halt(usb_dev_handle *dev, unsigned int ep)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_CLEAR_HALT, &ep);
  if (ret)
    USB_ERROR_STR(-errno, "could not clear/halt ep %d: %s", ep,
    	strerror(errno));

  return 0;
}

int usb_reset(usb_dev_handle *dev)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  int ret;

  ret = ioctl(dev->fd, IOCTL_USB_RESET, NULL);
  if (ret)
     USB_ERROR_STR(-errno, "could not reset: %s", strerror(errno));

  return 0;
}

int usb_get_driver_np(usb_dev_handle *dev, int interface, char *name,
	unsigned int namelen)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_getdriver getdrv;
  int ret;

  getdrv.interface = interface;
  ret = ioctl(dev->fd, IOCTL_USB_GETDRIVER, &getdrv);
  if (ret)
    USB_ERROR_STR(-errno, "could not get bound driver: %s", strerror(errno));

  strncpy(name, getdrv.driver, namelen - 1);
  name[namelen - 1] = 0;

  return 0;
}

int usb_detach_kernel_driver_np(usb_dev_handle *dev, int interface)
{
  if (NULL == dev || -1 == dev)
    USB_ERROR(-ENODEV);

  struct usb_ioctl command;
  int ret;

  command.ifno = interface;
  command.ioctl_code = IOCTL_USB_DISCONNECT;
  command.data = NULL;

  ret = ioctl(dev->fd, IOCTL_USB_IOCTL, &command);
  if (ret)
    USB_ERROR_STR(-errno, "could not detach kernel driver from interface %d: %s",
        interface, strerror(errno));

  return 0;
}

