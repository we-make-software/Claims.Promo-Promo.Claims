###############################################################################
#                                                                             #
#                     Claims.Promo & Promo.Claims                              #
#                     Kernel Playground Framework                              #
#                                                                             #
###############################################################################

License: GPL
Author : Claims.Promo & Promo.Claims

Overview:
  This framework is a sandbox for kernel module development.
  Each module is isolated, cloned in RAM, and can be safely loaded/unloaded.
  Tiny bugs may crash the system, but that’s the price of fun. 😎

Features:
  - Safe init/exit ordering
  - Module cloning in RAM
  - Developer-friendly macros
  - Dynamic streaming of "apps"
  - Humor included for free

Performance Notes:
  Modern humans react in roughly ~250 ms.
  This framework aims to stay far ahead of that.

  Incoming (RX) and outgoing (TX) processing are designed
  to stay under half of the human reaction window (~120 ms),
  ensuring that the system responds faster than a human can notice.

  When load increases, traffic can be redirected to other servers
  to preserve this reaction-time budget. The goal is simple:
  keep everything running so fast that the user never feels delay.

Getting Started:
  1. Set up your development environment:
     - Download and install VS Code: https://code.visualstudio.com/
     - Install Linux headers and development tools (build-essential)
  
  2. Kernel sources:
     - Get the latest stable Linux kernel from https://www.kernel.org/
       or clone from Linus Torvalds' GitHub: https://github.com/torvalds/linux
     - Ensure you have headers for your running kernel or the kernel you plan to build modules against.

  3. Compiler:
     - Use the latest GCC for your system: https://gcc.gnu.org/
     - Ensure you have `make` installed.

  4. Build the framework:
     - Open a terminal inside your project folder.
     - Run `make` to compile the menu.


Warnings:
  ⚠️ Kernel-level sandbox / one typo can freeze your machine.
  ⚠️ Always test in a VM first if possible.
  ⚠️ Half-baked modules or crashes are expected — debugging is part of the fun.

“Here be dragons” 🐉

