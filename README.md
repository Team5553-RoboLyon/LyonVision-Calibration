# LyonVision Template

A simple vision tracking format to use, create and deploy other programs with.

This template has to be used with the LyonVision raspberry pi image located at https://github.com/Team5553-RoboLyon/LyonVision-pi-gen

**All Vision Tracking code is located in the folder [src/main/](src/main)**


# Getting started

## With your computer

### 1. Install [WPILib](https://github.com/wpilibsuite/allwpilib/releases)

### 2. Install [Java 11](https://www.oracle.com/technetwork/java/javase/downloads/jdk11-downloads-5066655.html)

### 3. Install a compiler
  - [Visual Studio](https://visualstudio.microsoft.com/fr/downloads/) on Windows (in the Visual Studio Installer program, choose "Desktop Developement in C++")
  - Gcc on Linux

### 4. Install the raspbian cross-compiler
  - Run the following command in the root directory of this project :
  ```bash
  .\gradlew installRaspbianToolchain
  ```


## With the Raspberry Pi

### 1. Grab the custom Raspberry Pi image from the [LyonVision-pi-gen repository](https://github.com/Team5553-RoboLyon/LyonVision-pi-gen/releases)
  - Latest release with name `LyonVision n°x - date`

### 2. Flash the image onto a microSD card using etcher: https://etcher.io/
  - You will need to grab the .img file from the zip. The .img file is what you give to etcher.
  - After complete, insert the microSD card and startup the coprocessor. It may take a minute or two.

### 3. Start your Raspberry Pi
  - Hostname: `lyonvision.local`
  - Username: `vision`
  - Password: `lyon`



# Usage

## Running your code locally on your computer :
```bash
.\gradlew runVision
```

## Deploying your code to the Raspberry Pi
```bash
.\gradlew deploy
```

## Build all
```bash
.\gradlew build
```

## Test code
```bash
.\gradlew check
```


# Git Submodule

The folder `src/lib/` is a git submodule. It is a link to the [Team5553-RoboLyon/LyonVision-Library](https://github.com/Team5553-RoboLyon/LyonVision-Library) repo where the pseudo-library files are located. These files are in a separated repository because there can be used by several projects.
