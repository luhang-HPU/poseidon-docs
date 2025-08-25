# Trindent compile

Trident contains multiple applications: PIR, LR_TRAIN, KNN, PSI and Framingham Heart Study.

# Compile Steps:

## Step 1: Install poseidon
Please install the poseidon library before use it.
poseidon is divided into two versions **[Install Software Library](../Getting_Started/Compile and Installation/Software(poseidon).md)**, **[Install Hardware Library](../Getting_Started/Compile and Installation/Hardware(poseidon with HPU).md)**, poseidon with HPU version requires a corresponding hardware acceleration card, and it is generally recommended to install the normal version.

## Step 2: Install dependencies

### Cmake Variables
Apps in Trindent are independent and depend on Poseidon and cmake provides an option for each application to compile or not. As follows:

| CMake Variable | Type | Usage             | Default |
| -------------- | ---- | ----------------- | ------- |
| PIR            | bool | Build PIR         | ON      |
| LR_TRAIN       | bool | Build LR_TRAIN    | ON      |
| HEARTSTUDY     | bool | Build HEARTSTUDY  | ON      |
| KNN            | bool | Build KNN         | ON      |
| APSI           | bool | Build APSI        | ON      |

***Only APSI needs to install dependencies. If you don't compile apsi, you can skip this step and use ```cmake .. -DAPSI=OFF``` when generating makefile.***
```shell
# install flatbuffers
sudo apt install libflatbuffers-dev
# install json
sudo apt install libjsoncpp-dev
# install log4
sudo apt install liblog4cplus-dev
```

## Step 3: Compile Trident
```shell
# download the source code, use http or ssh
git clone https://github.com/luhang-HPU/Trident.git #http
git clone git@github.com:luhang-HPU/Trident.git #ssh

# go to the folder
cd Trident
# create a compilation directory
mkdir -p build && cd build

# compile
cmake ..
make -j
```
