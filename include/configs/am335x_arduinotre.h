/*
 * am335x_arduinotre.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Modified by Biagio montaruli <biagio-mkr@libero.it> in order to configure
 * U-Booot for Arduino Tre board https://www.arduino.cc/en/Main/ArduinoBoardTre
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_AM335X_ARDUINOTRE_H
#define __CONFIG_AM335X_ARDUINOTRE_H

#include <configs/ti_am335x_common.h>

/* Allow user to load environment variables in
   different way not only from the first fat
   partition of Sd card
 */
#define CONFIG_ENV_IS_NOWHERE
/*
#define CONFIG_ENV_IS_IN_FAT
#define FAT_ENV_INTERFACE          "mmc"
#define FAT_ENV_DEVICE_AND_PART    "0:1"
#define FAT_ENV_FILE             "uEnv.txt"
*/

#ifndef CONFIG_SPL_BUILD
# define CONFIG_TIMESTAMP
# define CONFIG_LZO
#endif

#define CONFIG_SYS_BOOTM_LEN		(16 << 20)

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* Always 128 KiB env size */
#define CONFIG_ENV_SIZE			(128 << 10)

/* This configuration option is included in order to add variables 
 * describing certain run-time determined information about the hardware 
 * to the environment.  These will be named board_name, board_rev
 */
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#ifdef CONFIG_USE_RAMDISK
#define RAMDISK_SETTINGS \
	"use_ramdisk=yes\0" \
	"import_ramdisk="\
	"setenv rdtypes ramdisk.img initramfs.img ramdisk.gz; " \
	"for rdfile in ${rdtypes} ; do " \
		"if test -e ${interface} ${bootpart} ${bootdir}/${rdfile}; then " \
			"echo Loading ramdisk image ${bootdir}/${rdfile} from ${interface}${mmcdev} partition ${mmcpart} ...; " \
			"run loadrd;" \
		"else " \
			"if test -e ${interface} ${mmcdev}:1 ${rdfile}; then " \
				"echo Loading ramdisk image ${rdfile} from ${interface}${mmcdev} partition 1 ...; " \
				"load ${interface} ${mmcdev}:1 ${rdaddr} ${rdfile}; setenv rdsize ${filesize}; " \
			"else " \
				"setenv rdfile none; " \
			"fi;" \
		"fi;" \
	"done;\0" \
	"ramdiskboot="\
	"run import_ramdisk;" \
	"if test ${rdfile} != none; then " \
		"if test ${rdfile} = ramdisk.gz; then " \
			"run ramargs;" \
		"fi;" \
		"if test ${bootfile} = zImage; then " \
			"echo Booting Linux Kernel ${bootfile} with ramdisk ${rdfile} ...; " \
			"bootz ${loadaddr} ${rdaddr} ${fdtaddr}; " \
		"else " \
			"if test ${bootfile} = uImage; then " \
				"echo Booting Linux Kernel ${bootfile} with ramdisk ${rdfile} ...; " \
				"bootm ${loadaddr} ${rdaddr} ${fdtaddr}; " \
			"else " \
				"echo WARN: Cannot load kernel image !; " \
			"fi;" \
		"fi;" \
	"else " \
		"echo WARN: Cannot load ramdisk image; " \
	"fi;\0"
#else
#define RAMDISK_SETTINGS "use_ramdisk=no\0"
#endif

#define BOOTENV_SETTINGS \
	"uenvaddr=0x880b0000\0" \
	"bootenv=uEnv.txt\0" \
	"script=boot.scr\0" \
	"loadbootenv="\
	"if test -e ${interface} ${bootpart} ${bootenv}; then " \
		"echo Loading boot environment data from ${interface}${mmcdev} partition 1 ...; " \
		"load ${interface} ${bootpart} ${uenvaddr} ${bootenv}; " \
	"else " \
		"setenv bootpart ${mmcdev}:2; " \
		"if test -e ${interface} ${bootpart} ${bootdir}/${bootenv}; then " \
			"echo Loading boot environment data from ${interface}${mmcdev} partition 2 ...; " \
			"load ${interface} ${bootpart} ${uenvaddr} ${bootdir}/${bootenv}; " \
		"else " \
			"setenv bootenv none; " \
		"fi;" \
	"fi;\0" \
	"importbootenv=echo Importing boot environment from ${bootenv}; " \
			"env import -t ${uenvaddr} ${filesize}\0" \
	"loadbootscript=if test -e ${interface} ${bootpart} ${script}; then " \
			"echo Loading boot script from ${interface}${mmcdev} partition ${mmcpart} ...; " \
			"load ${interface} ${bootpart} ${uenvaddr} ${script}; fi;\0" \
	"bootscript=echo Running boot scripts from ${script} ...; source ${uenvaddr}\0" \
	"envboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"setenv bootpart ${mmcdev}:1; " \
			"if run loadbootscript; then " \
				"run bootscript;" \
			"else " \
				"echo WARN: Cannot load ${script} !; " \
				"run loadbootenv;" \
				"if test ${bootenv} != none; then " \
					"run importbootenv;" \
					"if test -n ${uenvcmd}; then " \
						"echo Running uenvcmd ...;" \
						"run uenvcmd;" \
					"else " \
						"if test -n ${dtb}; then " \
							"echo Using dtb = ${dtb} ... ;" \
							"setenv fdtfile ${dtb};" \
						"fi;" \
						"echo Checking if uname_r is set in /boot/uEnv.txt ...;" \
						"if test -n ${uname_r}; then " \
							"gpio set 56; " \
							"setenv bootpart ${mmcdev}:2; " \
							"setenv fdtpart ${bootpart}; " \
							"setenv fdtfile am335x-arduino-tre.dtb; " \
							"setenv args_mmc_old ${mmcargs}; " \
							"echo Running uname_boot ...;" \
							"run uname_boot;" \
						"fi;" \
					"fi;" \
				"else " \
					"echo WARN: Cannot load ${bootenv} !; " \
				"fi;" \
			"fi;" \
		"fi;\0"

#define NETBOOT "netboot=echo Checking if client_ip is set ...;" \
		"if test -n ${client_ip}; then " \
			"gpio set 56; " \
			"if test -n ${uname_r}; then " \
				"echo Running nfsboot_uname_r ...;" \
				"run nfsboot_uname_r;" \
			"else " \
				"setenv bootfile zImage; " \
				"echo Running nfsboot ...;" \
				"run nfsboot;" \
			"fi;" \
		"else " \
			"echo WARN: Cannot boot over network because client_ip is not set !; " \
		"fi;\0"

#ifndef CONFIG_SPL_BUILD

/* CONFIG_EXTRA_ENV_SETTING includes any number of null terminated
 * strings (variable = value pairs) that will be part of
 * the default environment compiled into the boot image.
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	RAMDISK_SETTINGS \
	"interface=mmc\0" \
	"mmcdev=0\0" \
	"mmcpart=2\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"fdtfile=am335x-arduino-tre.dtb\0" \
	"console=ttyO0,115200n8\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"ramroot=/dev/ram0 rw\0" \
	"ramrootfstype=ext2\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"findimage=if test -e ${interface} ${bootpart} ${bootdir}/${bootfile}; then " \
		"echo Loading linux kernel image ${bootdir}/${bootfile} from ${interface}${mmcdev} partition ${mmcpart} ...; fi;\0" \
	"loadimage=load ${interface} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"findfdt=if test -e ${interface} ${fdtpart} ${fdtdir}/${fdtfile}; then " \
			"echo Found dtb in ${interface}${mmcdev} partition ${mmcpart} ...; fi;\0" \
	"loadfdt=echo Loading Arduino Tre dtb : ${fdtfile}; " \
		"load ${interface} ${fdtpart} ${fdtaddr} ${fdtdir}/${fdtfile}\0" \
	"loadrd=load ${interface} ${bootpart} ${rdaddr} ${bootdir}/${rdfile}; setenv rdsize ${filesize}\0" \
	"failumsboot=echo; echo FAILSAFE: U-Boot UMS (USB Mass Storage) enabled, media now available over the usb slave port ...; " \
		"ums 0 ${interface} 1;\0" \
	"load_kernel="\
	"setenv bootpart ${mmcdev}:${mmcpart}; " \
	"if run findimage; then " \
		"run loadimage; " \
	"else " \
		"setenv newpart ${mmcdev}:1; " \
		"if test -e ${interface} ${newpart} ${bootfile}; then " \
			"echo Loading linux kernel image ${bootfile} from ${interface}${mmcdev} partition 1 ...; " \
        		"load ${interface} ${newpart} ${loadaddr} ${bootfile}; " \
        	"else " \
			"setenv bootfile uImage; " \
			"if run findimage; then " \
				"run loadimage; " \
			"else " \
				"setenv mmcpart 3; " \
				"setenv bootpart ${mmcdev}:${mmcpart}; " \
				"if run findimage; then " \
					"run loadimage;" \
					"setenv mmcroot /dev/mmcblk0p3 ro; " \
        			"else " \
					"setenv bootfile none; " \
				"fi;" \
			"fi;" \
		"fi;" \
	"fi;\0" \
	"import_dtb="\
	"setenv fdtdir ${bootdir}; " \
	"setenv fdtpart ${mmcdev}:2; " \
	"if test ${mmcpart} != 2; then " \
		"setenv mmcpart 2; " \
	"fi;" \
	"if run findfdt; then " \
		"run loadfdt; " \
	"else " \
		"setenv fdtdir ${bootdir}/dtbs; " \
		"if run findfdt; then " \
			"run loadfdt;" \
		"else " \
			"setenv fdtdir ${bootdir}/dtb; " \
			"if run findfdt; then " \
				"run loadfdt;" \
			"else " \
				"setenv fdtdir ${bootdir}; " \
				"setenv fdtfile arduino-tre.dtb; " \
				"if run findfdt; then " \
					"run loadfdt;" \
				"else " \
					"setenv mmcpart 1; " \
					"setenv fdtpart ${mmcdev}:${mmcpart}; " \
        				"if test -e ${interface} ${fdtpart} ${fdtfile}; then " \
						"echo Loading ${fdtfile} from ${interface}${mmcdev} part ${mmcpart} ...; " \
        					"load ${interface} ${fdtpart} ${fdtaddr} ${fdtfile}; " \
					"else " \
						"setenv fdtfile am335x-arduino-tre.dtb; " \
						"if test -e ${interface} ${fdtpart} ${fdtfile}; then " \
							"echo Loading ${fdtfile} from ${interface}${mmcdev} part ${mmcpart} ...; " \
        						"load ${interface} ${fdtpart} ${fdtaddr} ${fdtfile}; " \
						"else " \
							"setenv fdtfile arduino-tre.dtb; " \
							"setenv mmcpart 3; " \
							"setenv fdtpart ${mmcdev}:${mmcpart}; " \
							"setenv fdtdir ${bootdir}; " \
        						"if run findfdt; then " \
								"run loadfdt;" \
							"else " \
								"setenv fdtfile none; " \
							"fi;" \
						"fi;" \
					"fi;" \
				"fi;" \
			"fi;" \
		"fi;" \
	"fi;\0" \
	"mmcboot=${interface} dev ${mmcdev}; " \
		"if ${interface} rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"run load_kernel;" \
			"if test ${bootfile} != none; then " \
				"run import_dtb;" \
				"if test ${fdtfile} != none; then " \
					"run mmcargs;" \
					"if test ${use_ramdisk} = yes; then " \
						"run ramdiskboot;" \
					"else " \
						"if test ${bootfile} = zImage; then " \
							"echo Booting Linux Kernel image ${bootfile} ...; " \
							"bootz ${loadaddr} - ${fdtaddr}; " \
						"else " \
							"if test ${bootfile} = uImage; then " \
								"echo Booting Linux Kernel image ${bootfile} ...; " \
								"bootm ${loadaddr} - ${fdtaddr}; " \
							"else " \
								"echo WARN: Invalid Kernel image !; " \
							"fi;" \
						"fi;" \
					"fi;" \
				"else " \
					"echo WARN: Cannot load the Arduino Tre dtb !; " \
				"fi;" \
			"else " \
				"echo WARN: Cannot load the kernel image !; " \
			"fi;" \
		"fi;\0" \
	BOOTENV_SETTINGS \
	EEWIKI_NFS \
	EEWIKI_UNAME_BOOT \
	NETARGS \
	NETBOOT \
	DFUARGS
#endif

#define CONFIG_BOOTCOMMAND \
	"run mmcboot;" \
	"run envboot;" \
	"run netboot;"

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */
#define CONFIG_SYS_NS16550_COM2		0x48022000	/* UART1 */
#define CONFIG_SYS_NS16550_COM3		0x48024000	/* UART2 */
#define CONFIG_SYS_NS16550_COM4		0x481a6000	/* UART3 */
#define CONFIG_SYS_NS16550_COM5		0x481a8000	/* UART4 */
#define CONFIG_SYS_NS16550_COM6		0x481aa000	/* UART5 */
#define CONFIG_BAUDRATE			115200

/* PMIC support */
#define CONFIG_POWER_TPS65217
#define CONFIG_POWER_TPS65910

/* SPL */
#ifndef CONFIG_NOR_BOOT
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT

/* Bootcount using the RTC block */
#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_AM33XX
#define CONFIG_SYS_BOOTCOUNT_BE

/* USB gadget RNDIS */
#define CONFIG_SPL_MUSB_NEW_SUPPORT

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/am33xx/u-boot-spl.lds"
#endif

/*
 * USB configuration.  We enable MUSB support, both for host and for
 * gadget.  We set USB0 as peripheral and USB1 as host, based on the
 * board schematic and physical port wired to each.  Then for host we
 * add mass storage support and for gadget we add both RNDIS ethernet
 * and DFU.
 */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT
#define CONFIG_USB_MUSB_PIO_ONLY
#define CONFIG_USB_MUSB_DISABLE_BULK_COMBINE_SPLIT
#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_HOST
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_PERIPHERAL

#ifndef CONFIG_SPL_USBETH_SUPPORT
/* Fastboot */
#define CONFIG_USB_FUNCTION_FASTBOOT
#define CONFIG_CMD_FASTBOOT
#define CONFIG_ANDROID_BOOT_IMAGE
#define CONFIG_FASTBOOT_BUF_ADDR	CONFIG_SYS_LOAD_ADDR
#define CONFIG_FASTBOOT_BUF_SIZE	0x07000000

/* To support eMMC booting */
#define CONFIG_STORAGE_EMMC
#define CONFIG_FASTBOOT_FLASH_MMC_DEV   1
#endif

#ifdef CONFIG_USB_MUSB_HOST
#define CONFIG_USB_STORAGE
#endif

#ifdef CONFIG_USB_MUSB_GADGET
/* Removing USB gadget and can be enabled adter adding support usb DM */
#ifndef CONFIG_DM_ETH
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:af:00:00"
#endif /* CONFIG_DM_ETH */
#endif /* CONFIG_USB_MUSB_GADGET */

/*
 * Disable MMC DM for SPL build and can be re-enabled after adding
 * DM support in SPL
 */
#ifdef CONFIG_SPL_BUILD
#undef CONFIG_DM_MMC
#undef CONFIG_TIMER
#endif

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USBETH_SUPPORT)
/* Remove other SPL modes. */
#undef CONFIG_SPL_YMODEM_SUPPORT
#undef CONFIG_SPL_NAND_SUPPORT
#undef CONFIG_SPL_MMC_SUPPORT
#define CONFIG_ENV_IS_NOWHERE
#undef CONFIG_ENV_IS_IN_NAND
/* disable host part of MUSB in SPL */
/* disable EFI partitions and partition UUID support */
#undef CONFIG_PARTITION_UUIDS
#undef CONFIG_EFI_PARTITION
/* General network SPL  */
#define CONFIG_SPL_NET_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_NET_VCI_STRING	"AM335x U-Boot SPL"
#endif

/* USB Device Firmware Update support */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_USB_FUNCTION_DFU
#define CONFIG_DFU_MMC
#define DFU_ALT_INFO_MMC \
	"dfu_alt_info_mmc=" \
	"boot part 0 1;" \
	"rootfs part 0 2;" \
	"MLO fat 0 1;" \
	"MLO.raw raw 0x100 0x100;" \
	"u-boot.img.raw raw 0x300 0x400;" \
	"spl-os-args.raw raw 0x80 0x80;" \
	"spl-os-image.raw raw 0x900 0x2000;" \
	"spl-os-args fat 0 1;" \
	"spl-os-image fat 0 1;" \
	"u-boot.img fat 0 1;" \
	"uEnv.txt fat 0 1\0"
#ifdef CONFIG_NAND
#define CONFIG_DFU_NAND
#define DFU_ALT_INFO_NAND \
	"dfu_alt_info_nand=" \
	"SPL part 0 1;" \
	"SPL.backup1 part 0 2;" \
	"SPL.backup2 part 0 3;" \
	"SPL.backup3 part 0 4;" \
	"u-boot part 0 5;" \
	"u-boot-spl-os part 0 6;" \
	"kernel part 0 8;" \
	"rootfs part 0 9\0"
#else
#define DFU_ALT_INFO_NAND ""
#endif
#define CONFIG_DFU_RAM
#define DFU_ALT_INFO_RAM \
	"dfu_alt_info_ram=" \
	"kernel ram 0x80200000 0xD80000;" \
	"fdt ram 0x80F80000 0x80000;" \
	"ramdisk ram 0x81000000 0x4000000\0"
#define DFUARGS \
	"dfu_alt_info_emmc=rawemmc raw 0 3751936\0" \
	DFU_ALT_INFO_MMC \
	DFU_ALT_INFO_RAM \
	DFU_ALT_INFO_NAND
#endif

/* Network. */
#define CONFIG_PHY_GIGE
#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC
#define CONFIG_PHY_ATHEROS
#define CONFIG_PHY_VITESSE

#endif	/* ! __CONFIG_AM335X_ARDUINOTRE_H */
