# Obscure's Epsilon App Framework

This repo is to be Joey Sodergren's design space for creating a sort of base-level framework upon which games can be built for the Numworks calculators, running the Epsilon interface.

The system currently only has one headline feature, but it's an important one to have: support for drawing variably-sized sprites with transparent backgrounds. The transparency is binary, meaning that pixels can only be fully transparent or fully opaque, but it does work.

Joey is developing this sytem on Ubuntu 25.10, running inside of a virtual machine.

If you're using just such a system (or something similar), you can run this command to install the necessary dependencies to use the project:
```
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi npm openjdk-25-jdk -y
```

To build the project, Joey usually runs this:
```
make clean && make build
```
