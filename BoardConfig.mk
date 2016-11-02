# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8996
TARGET_NO_BOOTLOADER := true

# Platform
TARGET_BOARD_PLATFORM := msm8996
TARGET_BOARD_PLATFORM_GPU := qcom-adreno530

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := kryo
TARGET_CPU_SMP := true

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic

# Kernel
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 user_debug=31 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 cma=16M@0-0xffffffff androidboot.hardware=htc_pme androidkey.dummy=1 androidboot.selinux=permissive

BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 4096
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x00008000 --ramdisk_offset 0x01000000 --tags_offset 0x00000100 --board recovery:0
TARGET_PREBUILT_KERNEL := device/htc/pme/kernel

BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3791650816
BOARD_USERDATAIMAGE_PARTITION_SIZE := 469762048
BOARD_FLASH_BLOCK_SIZE := 262144 # (BOARD_KERNEL_PAGESIZE * 64)
BOARD_VOLD_EMMC_SHARES_DEV_MAJOR := true

TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_HAS_NO_SELECT_BUTTON := true
BOARD_SUPPRESS_SECURE_ERASE := true
#TARGET_USES_LOGD := true

# Keymaster
TARGET_HW_DISK_ENCRYPTION := true

# TWRP Build Flags
TW_THEME := portrait_hdpi
TW_EXCLUDE_DEFAULT_USB_INIT := true
TW_HAS_DOWNLOAD_MODE := true
TW_INCLUDE_CRYPTO := true
TW_CRYPTO_USE_SYSTEM_VOLD := qseecomd
TW_INCLUDE_NTFS_3G := true
#TW_INPUT_BLACKLIST := "hbtp_vm"
TW_NO_EXFAT_FUSE := true
TARGET_RECOVERY_QCOM_RTC_FIX := true
TARGET_RECOVERY_DEVICE_MODULES := chargeled # strace twrpdec
#TW_RECOVERY_ADDITIONAL_RELINK_FILES := $(OUT)/system/xbin/strace $(OUT)/recovery/root/sbin/twrpdec
#TWRP_INCLUDE_LOGCAT := true

# Vendor Init
TARGET_INIT_VENDOR_LIB := libinit_pme
TARGET_UNIFIED_DEVICE := true


##==================================
##  RECOVERY_VARIANT := multirom
TARGET_RECOVERY_IS_MULTIROM := true

#MR_DEVICE_SPECIFIC_VERSION := b ... let the build script deal with this!

include device/common/version-info/MR_REC_VERSION.mk

ifeq ($(MR_REC_VERSION),)
MR_REC_VERSION := $(shell date -u +%Y%m%d)-01
endif

BOARD_MKBOOTIMG_ARGS += --board mrom$(MR_REC_VERSION)

# Still needed by MultiROM Boot Menu
#   from TWRP: setting GGL_PIXEL_FORMAT_RGBA_8888, double buffered, framebuffer: 0 (1440 x 2560)
MR_PIXEL_FORMAT := "RGBA_8888"
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TW_BRIGHTNESS_PATH := /sys/class/leds/lcd-backlight/brightness

# MultiROM config
MR_DEVICE_VARIANTS := htc_pmeul      #HTC 10 - GSM (PME_UL)
MR_DEVICE_VARIANTS += htc_pmeuhl     #HTC 10 - Europe (PME_UHL)
MR_DEVICE_VARIANTS += htc_pmewhl     #HTC 10 - Sprint (PME_WHL)
MR_DEVICE_VARIANTS += htc_pmewl      #HTC 10 - AT&T/T-Mobile/Verizon (PME_WL)
MR_INPUT_TYPE := type_b
MR_INIT_DEVICES := device/htc/pme/multirom/mr_init_devices.c
MR_DPI := xhdpi
MR_DPI_MUL := 2.0
MR_DPI_FONT := 435
#MR_CONTINUOUS_FB_UPDATE := true

MR_DEVICE_HOOKS := device/htc/pme/multirom/mr_hooks.c
MR_DEVICE_HOOKS_VER := 6

MR_FSTAB := device/htc/pme/multirom/mrom.fstab
MR_USE_MROM_FSTAB := true

MR_ENCRYPTION := true
MR_ENCRYPTION_SETUP_SCRIPT := device/htc/pme/multirom/mr_cp_crypto.sh
MR_ENCRYPTION_FAKE_PROPERTIES := true
MR_ENCRYPTION_FAKE_PROPERTIES_EXTRAS := device/htc/pme/multirom/mr_fake_properties.c

# not just yet :(
MR_KEXEC_MEM_MIN := 0x03200000
MR_KEXEC_DTB := true

MR_NO_KEXEC := enabled
# possible options:
#       1 true allowed      # NO_KEXEC_DISABLED =  0x00,   // no-kexec is disabled (ie it is built, but needs to be manually enabled)
#       2 enabled           # NO_KEXEC_ALLOWED  =  0x01,   // "Use no-kexec only when needed"
#       3 ui_confirm        # NO_KEXEC_CONFIRM  =  0x02,   // "..... but also ask for confirmation"
#       4 ui_choice         # NO_KEXEC_CHOICE   =  0x04,   // "Ask whether to kexec or use no-kexec"
#       5 forced            # NO_KEXEC_FORCED   =  0x08,   // "Always force using no-kexec workaround"
# any other setting won't build it at all
