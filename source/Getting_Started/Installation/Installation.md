Installing Poseidon is very simple. The installation package has integrated the HPU driver for the operating system. We have tested it on ubuntu 18.04. Higher version (i.e. 20.04 and 22.04) might also work well. 

# Download the Installation Package[](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/Installation/Installation.html#download-the-installation-package)

There are three ways to download the Poseidon installation package:

**GitHub**

```
wget https://github.com/luhang-CCL/Poseidon/raw/main/downloads/poseidon-xdma-v2.6.deb
```

**Gitee (recommended)**

```
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-xdma-v2.6.deb
```

**Baidu Netdisk**

```
https://pan.baidu.com/s/1BOdGCspseTXt2Pllf9DhLA?pwd=3q7x
```

# Install the Package[](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/Installation/Installation.html#install-the-package)

After you’ve downloaded the installation package, navigate to the download directory and install it using the following command:

```
sudo dpkg -i poseidon-xdma-v2.6.deb
```

The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at /usr/poseidon. Meanwhile, it constructs a soft link in /usr/local/include for the headers.  

**Done!**

After installation, you can test the library through the examples provided in ["Examples"](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples). 

If you encounter any installation problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn