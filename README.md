# Ethernet PHY Drivers Library

This repository contains platform agnostic bare metal C drivers for configuring and managing several Ethernet PHY chips:
- 88Q211x (100/1000BASE-T1)
- LAN867x (10BASE-T1S)
- DP83867 (10/100/1000BASE-T)

They have been tested on my [Switch-V4](https://github.com/ben5049/switch-v4-hardware) and [Switch-V5](https://github.com/ben5049/switch-v5-hardware) projects which use an STM32H573. Bugs may exist, and not all PHY functions are implemented, but it has been enough to get the PHYs transmitting and receiving data, with sleep & wake and some diagnostic features.

## Integration Examples

Examples of callback functions and initialisation can be found in here:
- [phy_callbacks.c](https://github.com/ben5049/unified-switch-firmware/blob/main/primary/NonSecure/application/src/phy/phy_callbacks.c)
- [phy_init.c](https://github.com/ben5049/unified-switch-firmware/blob/main/primary/NonSecure/application/src/phy/phy_init.c)
