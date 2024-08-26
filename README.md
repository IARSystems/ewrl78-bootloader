# A simple XMODEM serial boot loader<br>for the Renesas RL78 Microcontroller

## Overview
This repository hosts a workspace featuring a single project: __rl78-boot__. This project includes both the bootloader, which utilizes the Renesas Code Flash Self-Programming Library for RL78, and the application itself. The flashing process is performed via a serial port using the [XMODEM](https://en.wikipedia.org/wiki/XMODEM) protocol to ensure file transfer integrity.

## Requirements
The bootloader project in this repo used the following software and hardware components
- [IAR Embedded Workbench for Renesas RL78](https://iar.com/ewrl78) (V5.10.3)
- [Renesas Code Flash Self-Programming Library for RL78](https://www.renesas.com/us/en/software-tool/code-flash-libraries-flash-self-programming-libraries) (T01, Ver.4.00)
- [Renesas Applilet3 for RL78](https://renesas.com/applilet) (V1.22.0)
- [Tera Term](https://github.com/TeraTermProject/teraterm/releases/latest) (V5.2)
- 2x mini-B USB cables
- [Renesas E1 Emulator (or similar)](https://renesas.com/e1)
- Renesas Promotion Board for RL78 ([YRPBRL78G13](https://renesas.com/yrpbrl78g13))

>[!NOTE]
>- Newer versions might work with little or no modifications.
>- Renesas Applilet3 for RL78 was used for setting up the involved peripherals. The configuration file [rl78-boot.cgp](rl78-boot/rl78-boot.cgp) is available, for reference.

## Setup the YRPBRL78G13 board
The Renesas Promotion Board for RL78 conveniently comes with a built-in Serial-to-USB adapter which simplifies the initial setup.

1) Make sure the __Virtual UART__ is selected.

| Jumper | Configuration |
| -      | -             |
| `J6`   | `2-3`         |
| `J7`   | `2-3`         |
| `J8`   | `2-3`         |
| `J9`   | `2-3`         |

2) Use the E1 Emulator 14-pin flat cable to connect to the board `J5` header.
3) Use one mini-B USB cable to connect the Renesas E1 emulator to a PC USB port.
4) Use another mini-B USB cable to connect the `USB1` connector to another PC USB port.
5) Use the Windows __Device Manager__ to find out if the device was properly recognized and, if so, which communications port was assigned to it (e.g. `COMx`).


## Get the code
The __serial_bootloader__ workspace contains the __rl78-boot__ project and can be cloned directly (or downloaded):
```
$ git clone https://github.com/iarsystems/ewrl78-bootloader
```

## Connect to the serial port with Tera Term
1) Switch to the Tera Term.
2) Open a new serial connection using the COMx.
3) Set the serial port parameters to [115200-8-N-1](https://en.wikipedia.org/wiki/8-N-1).


## Build with IAR Embedded Workbench
To build the project, perform the following steps:

1) Install the RL78 FSL library (for IAR compiler version 2.10+):
   
![RENESAS_RL78_FSL_T01_4V00_b7YSmzOmeE](https://github.com/user-attachments/assets/c68600ef-fd21-4b89-811f-2ba4edf86c73)

2) Install it in the `<path-to>/ewrl78-bootloader/rl78-boot/FSL`. For example:

![image](https://github.com/user-attachments/assets/6b32c5c1-9bda-4138-9474-2d23ff4cbb5c)

3) In the IAR Embedded Workbench, load the __serial_bootloader.eww__ workspace.
4) Build the project (<kbd>F7</kbd>).
5) Choose __Project__ → __Download__ → __Download active application__.
6) __Disconnect__ the E1 Emulator from the YRPBRL78G13 `J5` header.

In Tera Term, you should see the application `V1` running, with the LED blinking on the board:

![image](https://github.com/user-attachments/assets/2399bb22-a1dd-4039-b179-c7f14cc6c7c3)


## Updating the firmware
1) In the project, modify the welcome string from `V1` to `V2` in the [main-app.c](rl78-boot/main-app.c#L37) source file and rebuild the project (<kbd>F7</kbd>).
2) The application can enter into its "bootloader mode". Press `b`:

![image](https://github.com/user-attachments/assets/349db917-154f-4f02-a563-f671c627694c)

3) The application is now in "bootloader mode" from where it is possible to download a new firmware. Press `1`:

![image](https://github.com/user-attachments/assets/e28a5790-e1c7-43a6-9e68-87b378f1c75e)

4) Choose __File__ → __Transfer__ → __XMODEM__ → __Send...__, navigate to the file `Debug\Exe\boot+app.bin` and click __Send__:

![image](https://github.com/user-attachments/assets/c0125767-d31e-49b8-95b8-04e7e7828e2f)

Congratulations! The bootloader updated the application:

![image](https://github.com/user-attachments/assets/1dba0444-95d2-47c5-b69a-c1b4206554e9)


## Memory layout
The memory layout was defined for the `R5F100LE` device:

| Region        | Start     | End       | Description                |
| -             | -         | -         | -                          | 
| `BOOT0_ROM`   | `0x00000` | `0x00FFF` | The bootloader region      |
| `BOOT1_ROM`   | `0x01000` | `0x01FFF` | The bootloader update slot |
| `APPLICATION` | `0x02000` | `0x0FFFF` | The main application       |

>[!NOTE]
>- The linker configuration file ([`boot_lnkr5f100le.icf`](rl78-boot/boot_lnkr5f100le.icf)) serves as an example for other RL78 compatible parts.
>- The `BOOT1_ROM` region is unmapped, solely used as temporary storage space for a new bootloader(, interrupt vectors, etc.).


## Post build
The following command is performed during the post-build stage (__Project__ → __Options__ → __Build Actions__):
```
ielftool --bin-multi="0x00000-0x00FFF;0x02000-0x0FFFF" "$TARGET_PATH$" "$EXE_DIR$\binfile" & copy /b/v "$EXE_DIR$\binfile-0x0"+"$EXE_DIR$\binfile-0x2000" "$EXE_DIR$\boot+app.bin"
```
This build action will:
1) Use the `IAR ELF Tool` to generate 2 binary files: one for `BOOT0_ROM` and another one for `APPLICATION`.
2) Use the `copy /b` command to concatenate the generated binary files into a single `boot+app.bin` binary file.


## Issues
For technical support contact [IAR Customer Support][url-iar-customer-support].

For questions or suggestions related to this repository: try the [wiki][url-repo-wiki] or check [earlier issues][url-repo-issue-old]. If those don't help, create a [new issue][url-repo-issue-new] with detailed information.

<!-- Links -->
[url-iar-customer-support]: https://iar.my.site.com/mypages/s/contactsupport

[url-repo-wiki]:           https://github.com/iarsystems/ewrl78-bootloader/wiki
[url-repo-issue-new]:      https://github.com/iarsystems/ewrl78-bootloader/issues/new
[url-repo-issue-old]:      https://github.com/iarsystems/ewrl78-bootloader/issues?q=is%3Aissue+is%3Aopen%7Cclosed
