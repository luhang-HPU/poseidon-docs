# Whole Package Installation (Updating)

Poseidon installation package includes both software code and hardware code. 

On the premise that you are equipped with the HPU card, you can switch the execution environment between the CPU and the HPU.

<br>

## Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)
* Hardware: Poseidon HPU accelerator card
* Hardware driver (The hardware driver has been integrated into the hardware installation package)

<br>

## Download

There are two ways to download the Poseidon installation package:

1. Download Poseidon package with the link: [Poseidon installation package](https://gitee.com/luhang-HPU/poseidon/tree/master/downloads)
2. Download Poseidon package with *wget* (check the version carefully)

```shell
# Ubuntu
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0_x86_64.deb

# CentOS
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0-1.el7.x86_64.rpm
```

<br>

## Installation

Navigate to the download directory and install it with the following command:
```shell
# Ubuntu
sudo apt-get install poseidon-1.0.0_x86_64.deb

# Centos
sudo yum localinstall poseidon-1.0.0-1.el7.x86_64.rpm
```

The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at */usr/poseidon*. Meanwhile, it constructs a soft link in */usr/local/include* for the headers.  

<br>

## Uninstallation

You can uninstall Poseidon package with the following command:
```shell
# Ubuntu
sudo apt-get remove poseidon

# Centos
sudo rpm -e poseidon-1.0.0-1.el7.x86_64
```

<br>

## Execute the Examples

Enable `POSEIDON_BUILD_EXAMPLES` when building.

```shell
cmake -DPOSEIDON_BUILD_EXAMPLES=ON ..
make -j

cd build/bin
./test_example_file_name

# Please upgrade to sudo privilege when executing with hardware
sudo ./test_example_file_name
```



If you only compiles single file, you can use the examples provided in [“Examples”](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples) with the following command.

```shell
g++ ${filename}.cpp -o ${filename} -I${poseidon_header_path} -lposeidon_shared -std=c++17
```



If you encounter any compile problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn
