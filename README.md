# Attacus :butterfly: :snake:

Flutter SDL Python

[Flutter](https://flutter.dev/)

[SDL](https://github.com/libsdl-org/SDL)

## Required

* [Poetry](https://python-poetry.org/)

## Optional

* [Ninja](https://ninja-build.org/)

## Clone

```bash
git clone --recursive https://github.com/kfields/attacus
cd attacus
```

## Extra Steps

You need to download the Flutter engine binaries from: [Custom-Flutter-Engine-Embedders](https://github.com/flutter/flutter/wiki/Custom-Flutter-Engine-Embedders)

Extract to: [project-root]/binaries/flutter

# Development

## Tool Chain

[scikit-build](https://github.com/scikit-build/scikit-build)

[pybind11](https://github.com/pybind/pybind11)

## Build
```bash
poetry shell
poetry install
python setup.py build [--build-type [Release|Debug]}
```

### Generate Bindings
```bash
aimgen gen
```

### Release
```bash
python setup.py build
```

### Debug
```bash
python setup.py build --build-type Debug
python setup.py build -G "Visual Studio 16 2019" --build-type Debug
```

## Quick Start
```bash
git clone --recursive https://github.com/kfields/attacus
cd attacus
mkdir build
cd build
cmake ..
```
### Or
```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
## Wayland
```bash
sudo apt install libwayland-dev
sudo apt install libxkbcommon-dev
sudo add-apt-repository ppa:christianrauch/libdecoration

cmake -DATT_WM_WAYLAND=ON -DCMAKE_BUILD_TYPE=Debug ..
```

## Glad
    v1
    glad --generator c --no-loader --out-path src
    
    v2
    glad --api gl:core --out-path src/glad c


## [Flutter Custom Devices](https://github.com/flutter/flutter/wiki/Using-custom-embedders-with-the-Flutter-CLI)
```
flutter channel master
flutter config --enable-custom-devices
flutter custom-devices
```
C:\Users\kurti\AppData\Roaming\.flutter_custom_devices.json

``` json
    {
      "id": "attacus",
      "label": "Attacus",
      "sdkNameAndVersion": "Attacus 0.1",
      "platform": null,
      "enabled": true,
      "ping": [
        "ping",
        "-n",
        "1",
        "-w",
        "500",
        "localhost"
      ],
      "pingSuccessRegex": "[<=]\\d+ms",
      "postBuild": null,
      "install": [
        "python",
        "--version"
      ],
      "uninstall": [
        "python",
        "--version"
      ],
      "runDebug": [
        "python",
        "main.py"
      ],
      "forwardPort": null,
      "forwardPortSuccessRegex": null,
      "screenshot": null
    }
```

## [Flutter Embedder](https://github.com/flutter/flutter/wiki/Custom-Flutter-Engine-Embedders)
Get the SHA of the Flutter engine you wish to use
```
cd flutter/bin/internal
code engine.version
```