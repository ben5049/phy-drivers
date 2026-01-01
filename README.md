# Ethernet PHY Drivers Library

This repository contains platform agnostic bare metal C drivers for configuring and managing several Ethernet PHY chips:
- 88Q211x (100/1000BASE-T1) - Working
- LAN867x (10BASE-T1S) - WIP
- DP83867 (10/100/1000BASE-T) - WIP

They have been tested on my [Switch-V4](https://github.com/ben5049/switch-v4-hardware) project which uses an STM32H573. Bugs may exist, and features may not be implemented, but it has been enough to get the PHYs transmitting and receiving data.

## Integrations Examples

Examples of callback functions and initialisation can be found in here:
- [phy_callbacks.c](https://github.com/ben5049/switch-v4-firmware/blob/main/primary/NonSecure/Application/Src/phy/phy_callbacks.c)
- [phy_init.c](https://github.com/ben5049/switch-v4-firmware/blob/main/primary/NonSecure/Application/Src/phy/phy_init.c)
