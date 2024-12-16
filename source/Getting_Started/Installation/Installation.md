# Whole Package Installation

Poseidon provides the dynamic library installation package. It is recommended for users to download and install the software package if you are only going to call the API functions rather than modifying the source code. 

The Poseidon dynamic library supports both software and acceleration cards. On the premise that you have an accelerator card, you can choose to switch between the software or the accelerator card. In the absence of an acceleration card, you can just use the software.



## Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)
* Hardware: Poseidon HPU accelerator card
* Hardware driver (The hardware driver has been integrated into the hardware installation package)



## Download

There are two ways to download the Poseidon installation package:

1. Here are all the installation packages.<br>
[The download address of the installation package](https://gitee.com/luhang-HPU/poseidon/tree/master/downloads)<br>

2. You can also download installation package with `wget` (check the version carefully)<br>
```shell
# Ubuntu
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0_x86_64.deb

# CentOS
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0-1.el7.x86_64.rpm
```



## Installation Package

After you’ve downloaded the installation package, navigate to the download directory and install it using the following command:
```shell
# Ubuntu
sudo apt-get install ./poseidon-1.0.0_x86_64.deb

# Centos
sudo yum localinstall poseidon-1.0.0-1.el7.x86_64.rpm
```

The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at /usr/poseidon. Meanwhile, it constructs a soft link in /usr/local/include for the headers.  



## Uninstallation Package

You can uninstall the software installation package with the following command:
```shell
# Ubuntu
sudo apt-get remove poseidon
# Centos
sudo rpm -e poseidon-1.0.0-1.el7.x86_64
```



## Run the Examples

Enable `POSEIDON_BUILD_EXAMPLES` when building.

```shell
cd build/bin
./test_[example name]

# Please upgrade to sudo privilege when executing with hardware
sudo ./test_[example name]
```



If you only compiles single file, you can use the examples provided in [“Examples”](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples) with the following command.

```shell
g++ ${filename}.cpp -o ${filename} -I${poseidon_header_path} -lposeidon_shared -std=c++17
```



If you encounter any compile problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn
