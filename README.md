# My First Awesome Kernel Module

Simple kernel module for embedded Linux distros based on Poky.

Tested on Yocto 4.2 (mickledore) with kernel version : 6.1.38-yocto-standard

## Installing

1. Install Yocto. You can read how to install [here](https://docs.yoctoproject.org/2.4.2/yocto-project-qs/yocto-project-qs.html) .
2. Clone this repo.
3. ```bash
   cp MyFirstAwesomeKernelModule <poky directory>
   ```
4. ```bash
   source oe-init-bulid-env
   ```
5. Add layer
    ```bash
    bitbake-layers add-layer ../MyFirstAwesomeKernelModule/
    ```
6. Add to end of your local.conf
``IMAGE_INSTALL:append = " awesome-mod"``

7. Build image with ``bitbake <image name>``
## Usage
1. Install module with
    ```bash
    su
    insmod /lib/modules/$(uname -r)/extra/awesome_module.ko address=<breakpoint address>
   ```
2. For change breakpoint address
   ```bash
   su
   echo <address> > /sys/kernel/address/address 
   ```
   For read
   ```bash
   cat /sys/kernel/address/address
   ```
### Used materials 

https://community.nxp.com/t5/i-MX-Processors-Knowledge-Base/Incorporating-Out-of-Tree-Modules-in-YOCTO/ta-p/1373825

https://martin.uy/blog/hardware-breakpoints-in-the-linux-kernel-through-perf_events/

https://stackoverflow.com/questions/19725900/watch-a-variable-memory-address-change-in-linux-kernel-and-print-stack-trace

https://embetronicx.com/tutorials/linux/device-drivers/sysfs-in-linux-kernel/

https://devarea.com/linux-kernel-development-kernel-module-parameters/
