# Poseidon software compile

**Must read: This article is a guide to compiling and using software versions. Poseidon is divided into software and hardware versions. The software version is the open source code mentioned in this article. The hardware version is hardware acceleration of the operator, and the full function is based on this open source code, and it also requires a hardware acceleration card and corresponding drivers, incremental installation packages, etc.**
<br>

## Requirement

* Operating System: Ubuntu >= 18.04 LTS
* Tool chain: GNU g++ ( >= 8.0) , CMake ( >= 3.12 )
* Dependency library: gmp ( >= 6.3.0)
<br>

## Source Code Download

Download the software source code from the following link: [SourceCode](https://github.com/luhang-HPU/poseidon)
<br>

## Build Source Code

You can directly compile the source code with the following command:

```shell
# download the source code, use http or ssh
git clone https://github.com/luhang-HPU/poseidon.git #http
git clone git@github.com:luhang-HPU/poseidon.git #ssh

# go to the folder
cd poseidon
# create a compilation directory
mkdir -p build && cd build

# compile, the executable program will be in ./bin/
# ps: requires VPN!!!
cmake .. 
make -j

# install the library
# default install dir
# header files: /usr/local/include/poseidon/
# library file: /usr/local/lib/
sudo make install
sudo ldconfig
# If you install the library in a specified path
# Such as: make install DESTDIR=${path}
# will be install ${path}/usr/local/include/poseidon/ and ${path}/usr/local/lib/
# You need to add above path to PATH
```
<br>

## Execute the Examples

**Method one**

After Build Source Code.

```shell
# In build folder 
./bin/test_example_file_name 
```

**Method two**

If you only compiles single file, you can use the examples with the following command.

```shell
# Please replace the internal variables with the actual file
g++ ${filename}.cpp -o ${filename} -I${poseidon_header_path} -lposeidon_shared -std=c++17
```
<br>

## Cmake Variables

Poseidon provides a range of cmake variables for users to choose from. For the experience of use, try to keep the default options.

| CMake Variable          | Type | Usage                    | Default |
| ----------------------- | ---- | ------------------------ | ------- |
| POSEIDON_BUILD_EXAMPLES | bool | Build Poseidon examples  | ON      |
| POSEIDON_USE_ZSTD       | bool | Use zstd                 | ON      |
| POSEIDON_USE_ZLIB       | bool | Use zlib                 | ON      |
| POSEIDON_USE_MSGSL      | bool | Use msgsl                | ON      |
| POSEIDON_USE_HARDWARE   | bool | Whether hardware is used | OFF     |


POSEIDON_BUILD_EXAMPLES: An example is an application based on the Poseidon library, which can be turned off to speed up the compilation of parts of the poseidon library
POSEIDON_USE_ZSTD, POSEIDON_USE_ZLIB, POSEIDON_USE_MSGSL: Some additional dependencies. The advantage of **turning it off** is that you don't need a VPN during the compilation process, **but the disadvantage is that it will cause some applications to not work.**
POSEIDON_USE_HARDWARE: It is required in hardware notebooks, and the opening conditions are relatively harsh. See Hardware Compilation for details.

If you encounter any problems with compilation and installation, please feel free to contact us via email at luhang@ict.ac.cn
