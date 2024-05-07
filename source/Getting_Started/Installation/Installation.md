Installing Poseidon is very simple. The installation package has integrated the HPU driver for the operating system. We have tested it on ubuntu 18.04. Higher version (i.e. 20.04 and 22.04) also might also work well. 

# Download and Install Poseidon Hardware Library

<table>
<tbody>
<tr>
        <th>System</th>
        <th>Toolchain</th>
    </tr>
    <tr>
        <td>Linux</td>
        <td>GNU G++ (>= 8.0), CMake (>= 3.12)</td>
    </tr>
</tr>
</tbody>
</table>


# Download the Installation Package

There are three ways to download the Poseidon installation package:

**Gitee (recommended)**

Here are all the installation packages.
[downloads list](https://gitee.com/luhang-HPU/poseidon/tree/master/downloads)

You can also download it this way (check the version carefully)
Centos
```
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-4.0.0-1.el7.x86_64.rpm
```

Ubuntu 
```
wget https://gitee.com/luhang-CCL/poseidon/raw/master/downloads/poseidon-4.0.0_x86_64.deb
```

<!--
**Baidu Netdisk**

```
https://pan.baidu.com/s/12PJr49DyHtVIQJYrBXVDLQ?pwd=awf8

```
-->
# Install the Package

After you’ve downloaded the installation package, navigate to the download directory and install it using the following command:

**Centos**

```
#install
sudo yum localinstall poseidon-4.0.0-1.el7.x86_64.rpm

#uninstall
sudo rpm -e poseidon-4.0.0-1.el7.x86_64
```

**Ubuntu**
```
#install
sudo apt-get install ./poseidon-4.0.0_x86_64.deb

#uninstall
sudo apt-get remove poseidon
```


The installation process will require the system reboot before it wraps up. The target installation folder for the headers and dynamic library locates at /usr/poseidon. Meanwhile, it constructs a soft link in /usr/local/include for the headers.  

### Example Compile

```
g++ xxx.cpp -o xxx -lposeidon_shared -std=c++17
```

**Done!**

After installation, you can test the library through the examples provided in ["Examples"](https://poseidon-hpu.readthedocs.io/en/latest/Getting_Started/index.html#examples). 

If you encounter any installation problems regarding the OS compatibility, please feel free to contact us via email: luhang@ict.ac.cn
