Poseidon is comprised of open-source software code and non-open-source hardware code, which results in different ways to compile and install.

Here are our recommendation for different users:

1. If you are going to have a try on FHE or Poseidon library, both **compiling source code only** or **whole package installation** is recommended. 
2. If you are equipped with HPU card and not to modify the software source code, **whole package installation** is recommended.
3. If you are equipped with HPU card and want to modify the software code, **compiling source code with hardware library** is recommended.

<br>

# Compile

Poseidon supports two ways to compile the open-source software code.

1. Compile software source code only
2. Compile software source code with hardware package

<br>

## Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)

<br>

## Source Code Download

Download the software source code from the following link: [SourceCode](https://github.com/luhang-HPU/poseidon)

<br>

## Build Software Source Code Only
You can directly compile the source code with the following command:

```shell
mkdir build && cd build
cmake .. # You can open CMake Variable
make -j

# If you need to install the library
sudo make install
# If you install the library in a specified path, you need to add the corresponding environment variables
```

<br>

## Install Hardware Library

***If you don't use hardware, don't use this method!!!***
***This requires the hardware library to be installed before compiling!!!***

<br>

**Install dependency**

You can download installation package with *wget* (check the version carefully)

```shell
# Ubuntu download
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-hardware-1.0.0_x86_64.deb
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/dpdk.deb
# Ubuntu install
sudo apt-get install ./poseidon-1.0.0_x86_64.deb
sudo apt-get install dpdk.deb

# CentOS download
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-hardware-1.0.0-1.el7.x86_64.rpm
# CentoOS install
sudo yum localinstall poseidon-1.0.0-1.el7.x86_64.rpm
```

<br>

**Build**

After the hardware library is installed, you can compile the source code with the following command:

```shell
mkdir build && cd build
cmake -DPOSEIDON_USE_HARDWARE=ON ..
make -j
```



Poseidon provides a bunch of cmake variables for users to decide what kind of source code to build. If you are going to modify the source code, do not forget to modify corresponding cmake file.

| CMake Variable          | Variable Type | Usage                                     |
| ----------------------- | ------------- | ----------------------------------------- |
| POSEIDON_BUILD_EXAMPLES | bool          | Build Poseidon examples                   |
| POSEIDON_BUILD_APP      | bool          | Build Poseidon applications               |
| POSEIDON_BUILD_TEST     | bool          | Build gtest cases                         |
| POSEIDON_USE_HARDWARE   | bool          | Build software only or with hardware code |

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
