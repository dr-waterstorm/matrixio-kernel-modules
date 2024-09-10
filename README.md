# matrixio-kernel-modules

MATRIXIO Kernel Modules is the kernel drivers for MATRIX Creator and MATRIX Voice.

## Note / Warning:
I'm no actual C dev, I'm just trying to get this working somehow.

## Fork purpose
As matrixio does not seem to support their hardware anymore, the purpose of this fork is to make the matrixio Kernel for MATRIX Voice usable on a modern Raspberry Pi OS (bookworm) with the 6.6 kernel.

## Warning: Kernel Version
This drivers **only** works with current stock raspbian kernel. To reverting back to current stock Raspbian kernel use:  

```
sudo apt-get install --reinstall raspberrypi-bootloader raspberrypi-kernel
```

## Cloning & compiling from sources

### Install Dependencies

```
# Update packages and install
sudo apt-get update
sudo apt-get upgrade
```

```
# Installation Kernel Packages
sudo apt install raspberrypi-kernel-headers raspberrypi-kernel git 
```

```
# Install deps
sudo apt install cmake g++ git libfftw3-dev libgflags-dev gpiod gpiod-dev libgpiod-dev
```

```
# Reboot
sudo reboot
```

### SPI
Enable SPI via `sudo raspi-config`

```
sudo sed -i '$s/$/ spidev.bufsiz=12288/' /boot/firmware/cmdline.txt
```

```
# Reboot
sudo reboot
```

### Cloning & Compiling
```
git clone https://github.com/dr-waterstorm/matrixio-kernel-modules
cd matrixio-kernel-modules/src
make && make install
```
### Overlay Setup

Add in `/boot/firmware/config.txt`

```
dtoverlay=matrixio
```

Finally, load the remaining required modules
```
sudo cp ~/matrixio-kernel-modules/misc/matrixio.conf /etc/modules-load.d/
sudo cp ~/matrixio-kernel-modules/misc/asound.conf /etc/
sudo reboot
```

The reboot in the last step allows and activates the MATRIXIO Kernel modules. 
