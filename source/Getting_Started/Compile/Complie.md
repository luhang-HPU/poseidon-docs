# Complie

Poseidon is divided into software part and accelerator card part.Only the source code of the software part is provided.Poseidon supports two situations.

Situation one: Only use the software library.

Situation two: The Poseidon dynamic library supports both software and acceleration cards. On the premise that you have an accelerator card, you can choose to switch between the software or the accelerator card. In the absence of an acceleration card, you can just use the software.

Both of the above situations support compilation. The purpose of compilation is to link other libraries provided by us to use our accelerator card part when the source code has been modified to a certain extent.It is recommended to directly use the installation package to install the poseidon library [INSTALL](installation.md). The following is the compilation guide for the two situations.



## Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)



### Download

You can download the software source code from the following link:

[SourceCode]([luhang-HPU/poseidon](https://github.com/luhang-HPU/poseidon))



### Build

#### Situation one:

You can directly compile the source code with the following command:

```shell
mkdir build && cd build
cmake ..
make -j
```



#### Situation two:

Situation two requires the hardware library to be installed before compiling.

***Install depend***

You can also download installation package with `wget` (check the version carefully)<br>

```shell
# Ubuntu download
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-hardware-1.0.0_x86_64.deb
# Ubuntu install
sudo apt-get install ./poseidon-1.0.0_x86_64.deb

# CentOS download
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-hardware-1.0.0-1.el7.x86_64.rpm
# CentoOS install
sudo yum localinstall poseidon-1.0.0-1.el7.x86_64.rpm
```

##### ***Build***

After the hardware library are installed, you can compile the source code with the following command:

```shell
mkdir build && cd build
cmake -DPOSEIDON_USE_HARDWARE=ON ..
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



### Test

After installation, you can test the library through the examples provided in [“Examples”](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples).

If you encounter any installation problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn



## Software Installation Package



### Download

There are two ways to download the software installation package.

The software installation package can be downloaded from the following link:

[GitHub]([luhang-HPU/poseidon](https://github.com/luhang-HPU/poseidon))





Also you can download the software installation package with the following command (check the version carefully):

**CentOS**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0-1.el7.x86_64.rpm
```

**Ubuntu**

```shell
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-1.0.0_x86_64.deb
```



### Installation

You can install the software installation package with the following command:

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



### Uninstallation

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


### Compile

We recommand to use CMake for project management. 

If you compiles single file, you could run the following command:

```shell
g++ filename.cpp -o filename -I/usr/local/include/poseidon_software/ -lposeidon_shared -std=c++17
```
