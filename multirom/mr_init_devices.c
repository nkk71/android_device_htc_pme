#include <stdlib.h>

// These are paths to folders in /sys which contain "uevent" file
// need to init this device.
// MultiROM needs to init framebuffer, mmc blocks, input devices,
// some ADB-related stuff and USB drives, if OTG is supported
// You can use * at the end to init this folder and all its subfolders
const char *mr_init_devices[] =
{
    "/sys/devices/virtual/mem/null",
    "/sys/devices/virtual/misc/fuse",

    "/sys/class/graphics/fb0",


    // mmcblk0 -> ../devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0
    "/sys/block/mmcblk0",
    "/sys/devices/soc/7464900.sdhci",
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0",
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001",
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0",
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p38", //misc
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p44", //extra
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p42", //devlog
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p41", //fataldevlog
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p48", //persist

    // on HTC 10 1.80.709.1 there's an extra partition:
    //   58          890774          890805       16384   0700  control
    //   so everything on/after 58 needs to be increased by 1
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p59", //boot
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p60", //recovery
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p61", //cache
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p62", //system
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p63", //userdata
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p64", //userdata on 1.80.709.1


    // sdcard
    // mmcblk1 -> ../devices/soc/74a4900.sdhci/mmc_host/mmc1/mmc1:e624/block/mmcblk1
    "/sys/block/mmcblk1",
    "/sys/block/mmcblk1/mmcblk1p1",
    "/sys/block/mmcblk1/mmcblk1p2",


    "/sys/bus/mmc",
    "/sys/bus/mmc/drivers/mmcblk",
    "/sys/bus/sdio/drivers/bcmsdh_sdmmc",
    "/sys/module/mmc_core",
    "/sys/module/mmcblk",

    // OTG
    // "/sys/devices/soc.0/f9200000.ssusb/f9200000.dwc3/xhci-hcd*",
    // "/sys/devices/platform/xhci-hcd*",
    // /devices/soc/74a4900.sdhci/mmc_host/mmc*    auto               auto    defaults                                                                                          voldmanaged=sdcard1:auto,noemulatedsd,encryptable=userdata
    // /devices/platform/msm_hsusb*                auto               auto    defaults                                                                                          voldmanaged=usbdisk:auto

    // input devices
    "/sys/devices/soc/soc:gpio_keys/input*",
    "/sys/devices/virtual/input*",
    "/sys/devices/virtual/misc/uinput",
    "/sys/devices/soc/757a000.i2c/i2c-6/6-0020/input*",

    // adb
    "/sys/devices/virtual/tty/ptmx",
    "/sys/devices/virtual/misc/android_adb",
    "/sys/devices/virtual/android_usb/android0/f_adb",
    "/sys/devices/platform/android_usb/usb_function_switch",
    "/sys/bus/usb",


    // Encryption
    "/sys/block/mmcblk0rpmb",
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p47",  //ssd
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0p55",  //keymaster
    "/sys/devices/soc/7464900.sdhci/mmc_host/mmc0/mmc0:0001/block/mmcblk0/mmcblk0rpmb", //rpmb

    "/sys/devices/virtual/ecryptfs",
    "/sys/devices/virtual/misc/ecryptfs",
    "/sys/devices/virtual/icesdcc/icesdcc",
    "/sys/devices/virtual/misc/device-mapper",
    "/sys/devices/virtual/misc/ion",
    "/sys/devices/virtual/qseecom/qseecom",


    // Logging
    "/sys/devices/virtual/pmsg/pmsg0",
    "/sys/devices/virtual/mem/kmsg",

    NULL
};
