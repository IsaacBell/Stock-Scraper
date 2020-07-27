# Stock Scraper (Working Title)

### Current Version 0.1

This is a project I'm using to learn C++.

Thanks to https://github.com/dani2442 for providing reference code which helped to get started.

Eventually, this is going to analyze stock data and perform market transactions. We're still at the first steps...

## Description

This application pulls in market data from the IEX Cloud API for analysis using various algorithms.

Initial target algorithms are Time-Weighted Average Price (TWAP) and Volume-Weighted Average Price (VWAP).

## Installation

Hm. I can tell you how I'm running this on my OSX machine (yes). The makefile and such will probably need some tweaking for cross-platform utility.

### Tools You Will Need (OSX)

* VCPkg (?)
* Visual Studio Code (optional)
* XCode (should be up to date)
* libcurl

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
