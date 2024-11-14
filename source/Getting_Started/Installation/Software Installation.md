Poseidon(software version) provides the dynamic library installation package as well as the source code. It is recommended for users to download and install the software package if you are only going to call the API functions rather than modifying the source code.



# Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)



# Software Installation Package



## Download

There are two ways to download the software installation package.

The software installation package can be downloaded from the following link:

**GitHub** link(TODO)

**Gitee** link(TODO)



Also you can download the software installation package with the following command (check the version carefully):

**CentOS**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-4.0.0-1.el7.x86_64.rpm (TODO)
```

**Ubuntu**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-4.0.0_x86_64.deb (TODO)
```



## Installation

You can install the software installation package with the following command:

**Centos**

```shell
#install
sudo yum localinstall poseidon-4.0.0-1.el7.x86_64.rpm
```

**Ubuntu**

```shell
#install
sudo apt-get install ./poseidon-4.0.0_x86_64.deb
```

The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at /usr/poseidon. Meanwhile, it constructs a soft link in /usr/local/include for the headers.  



## Uninstallation

You can uninstall the software installation package with the following command:

**Centos**

```shell
#uninstall
sudo rpm -e poseidon-4.0.0-1.el7.x86_64
```

**Ubuntu**

```shell
#uninstall
sudo apt-get remove poseidon
```

 

## Compile

We recommand to use CMake for project management. 

If you compiles single file, you could run the following command:

```shell
g++ filename.cpp -o filename -I/usr/local/include/poseidon_software/ -lposeidon_shared -std=c++17
```



# Software Source Code 



## Download

You can download the software source code from the following link:

**GitHub** link(TODO)

**Gitee** link(TODO)



## Compile

You can compile the source code with the following command:

```shell
mkdir build && cd build
cmake ..
make -j
```



Poseidon provides a bunch of cmake variables for users to decide what kind of source code to build.

| CMake Variable          | Variable Type | Usage                                |
| ----------------------- | ------------- | ------------------------------------ |
| POSEIDON_BUILD_EXAMPLES | bool          | Build examples for Poseidon          |
| POSEIDON_BUILD_APP      | bool          | Build applications for Poseidon      |
| POSEIDON_BUILD_TEST     | bool          | Build test examples for Poseidon     |
| POSEIDON_USE_HARDWARE   | bool          | Build software code or hardware code |

If you are going to modify the source code, you should modify the the cmake files as well.





## Test

After installation, you can test the library through the examples provided in [“Examples”](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples).

If you encounter any installation problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn
