This is a command-line tool.

The `main.cpp` file analyzes stock data in real time, and gives a full print-out of current and historical trade data. For now, in the remote repository this code is set to sandbox mode.

What you're looking at is a very early version. In the future I plan to build this into a 1.0 with a UI.

Thanks to https://github.com/dani2442 for providing reference code which helped to get started.

## Description

This application pulls in market data from the IEX Cloud API for analysis using various algorithms.

Initial target algorithms are Time-Weighted Average Price (TWAP) and Volume-Weighted Average Price (VWAP).

## Installation

Hm. I can tell you how I'm running this on my OSX machine (yes). Cross-compatibility is forthcoming.

### Tools You Will Need

* VCPkg (?)
* libcurl

### Mac Installation

```
git clone https://github.com/IsaacBell/Stock-Scraper.git
cd stock-scraper

# OSX
cd ~/Documents
xcode-select --install # This can take a long time
brew install curl-openssl
vcpkg install rapidjson
vcpkg install curl
export LDFLAGS="-L/usr/local/opt/curl/lib"
export CPPFLAGS="-I/usr/local/opt/curl/include"
export PKG_CONFIG_PATH="/usr/local/opt/curl/lib/pkgconfig"

export PKG_CONFIG_PATH="/usr/local/opt/curl-openssl/lib/pkgconfig"
echo 'export PATH="/usr/local/opt/curl-openssl/bin:$PATH"' >> /Users/$(whoami)/.bash_profile
export LDFLAGS="-L/usr/local/opt/curl-openssl/lib"
```

## Building the Application

In VSCode, open the main.cpp file. Then, either click Terminal -> Run Build Task, or press Cmd+Shift+B.

## Running the Application

In VSCode, with the application built, open a new Terminal window in VSCode. (You can click the + Button in the terminal after the build finishes).

Find the `main` executable file which was built/compiled from your `main.cpp` file, and run it from the terminal.

```
# from the root directory
./main
```
