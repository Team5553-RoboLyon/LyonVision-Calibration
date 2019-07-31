# Camera Calibration

Programme de calibration de camera sur PC ou sur Raspberry par ssh.

La calibration s'effectue avec une feuille imprimée d'un quadrillage noir et blanc.


# Getting started

Follow instructions of https://github.com/Team5553-RoboLyon/LyonVision-Template


# Usage

## Running your code locally on your computer :
```bash
.\gradlew runVision
```

## Deploying your code to the Raspberry Pi
```bash
.\gradlew deploy
```
* SSH into the Raspberry : user `vision` et password `lyon`

* Run `./visionProgram` on the Raspberry

* Open the mjpeg stream in any web browser : `lyonvision.local:1181/?action=stream`

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
