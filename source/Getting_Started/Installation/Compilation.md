# Compile

Poseidon is comprised of open-source software code and non-open-source hardware code, which results in different ways to compile and install.

If you are going to have a try on FHE or Poseidon library, both compiling source code only or whole package installation is recommended. 

If you are equipped with HPU card and not to modify the software source code, whole package installation is recommended.

If you are equipped with HPU card and want to modify the software code, compiling source code with hardware library is recommended.



Poseidon supports two ways to compile the open-source software code.

1. Compile software source code only
2. Compile software source code with hardware package



## Requirement 

* Operating System: Ubuntu >= 18.04 LTS , CentOS >= 7.0
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)



## Source Code Download

Download the software source code from the following link:
[SourceCode]([luhang-HPU/poseidon](https://github.com/luhang-HPU/poseidon))



## Build Software Source Code Only

You can directly compile the source code with the following command:

```shell
mkdir build && cd build
cmake ..
make -j
```



## Build Software Source Code With Hardware Package (Optional)

*** This requires the hardware library to be installed before compiling!!!***



**Install dependency**
You can download installation package with `wget` (check the version carefully)<br>

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
