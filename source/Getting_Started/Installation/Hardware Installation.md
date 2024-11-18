Poseidon(hardware version) provides the dynamic library installation package. It is recommended for users to download and install the software package if you are only going to call the API functions rather than modifying the source code.

The hardware version installation package contains all the software functions provided by software version install package. However, if users wish to 

# Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)
* Hardware: Poseidon HPU accelerator card
* Hardware driver (The hardware driver has been integrated into the hardware installation package)



# Hardware Installation Package

## Download

There are two ways to download the Poseidon installation package:

**Gitee (recommended)**

Here are all the installation packages.<br>
[The download address of the installation package](https://gitee.com/luhang-HPU/poseidon/tree/master/downloads)<br>

You can also download installation package with `wget` (check the version carefully)<br>

**CentOS**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0-1.el7.x86_64.rpm
```

**Ubuntu**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0_x86_64.deb
```


## Installation

After you’ve downloaded the installation package, navigate to the download directory and install it using the following command:

**Centos**

```shell
#install
sudo yum localinstall poseidon-1.0.0-1.el7.x86_64.rpm
```

**Ubuntu**

```shell
#install
sudo apt-get install ./poseidon-1.0.0_x86_64.deb
```


The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at /usr/poseidon. Meanwhile, it constructs a soft link in /usr/local/include for the headers.  



## Uninstallation

You can uninstall the software installation package with the following command:

**Centos**

```shell
#uninstall
sudo rpm -e poseidon-1.0.0-1.el7.x86_64
```

**Ubuntu**

```shell
#uninstall
sudo apt-get remove poseidon
```

 



## Test

After installation, you can test the library through the examples provided in ["Examples"](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples). 

If you encounter any installation problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn
