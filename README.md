# ESP32-S3 PlatformIO Template (Arduino Framework) <!-- omit in toc -->

This repository is a standardized starting point for ESP32-S3 embedded projects using the Arduino framework within PlatformIO. It is pre-configured for high-memory variants (32MB Flash, 8MB PSRAM) and includes essential boilerplate for networking, OTA updates, and hardware debugging.

## Table of Contents <!-- omit in toc -->

- [Hardware Target](#hardware-target)
- [Pre-Packaged Features](#pre-packaged-features)
- [Project Structure](#project-structure)
- [Quick Start (CLI)](#quick-start-cli)
- [Quick Start (VS Code)](#quick-start-vs-code)
- [Over-The-Air (OTA) Flashing](#over-the-air-ota-flashing)
- [Debugging Crashes](#debugging-crashes)


## Hardware Target
* **Microcontroller:** ESP32-S3 (Specifically variants with Octal SPI memory)
* **Flash:** 32MB (OPI)
* **PSRAM:** 8MB (OPI)
* **Connection:** Native USB CDC (not UART-to-USB)

## Pre-Packaged Features
* **Custom Partitioning:** Pre-configured `partitions.csv` allocating 8MB for the main app, 8MB for OTA staging, and 14MB for SPIFFS/LittleFS.
* **PSRAM Enablement:** PlatformIO environment flags set to initialize and utilize the 8MB Octal PSRAM via `ps_malloc()`.
* **Native USB Serial:** Configuration flags to ensure `Serial.print()` routes correctly through the native USB port without missing initial boot messages.
* **Over-The-Air (OTA) Updates:** Boilerplate C++ code and PlatformIO configurations to flash new firmware over local Wi-Fi.
* **Exception Decoder:** Automated translation of hexadecimal crash backtraces into exact file and line numbers for rapid debugging.
* **RGB LED Control:** Basic non-blocking heartbeat implementation utilizing the onboard WS2812 NeoPixel.

## Project Structure
* `.vscode/` - IDE specific settings.
* `include/` - C++ header files.
* `lib/` - Project-specific local dependencies.
* `src/main.cpp` - Main application entry point containing Wi-Fi and OTA logic.
* `partitions.csv` - Custom flash memory map.
* `platformio.ini` - Build environment and hardware configuration.
* `.gitignore` - Prevents tracking of `.pio/` build artifacts.

## Quick Start (CLI)
1. Clone this repository.
2. Open a terminal in the project root.
3. Compile the firmware:
   `pio run`
4. Connect the ESP32-S3 via USB and flash:
   `pio run --target upload`
5. Open the serial monitor:
   `pio device monitor`

## Quick Start (VS Code)
1. Install the **PlatformIO IDE** extension.
2. Open the cloned repository folder in VS Code.
3. Update the `ssid` and `password` variables in `src/main.cpp`.
4. Click the **Build (Checkmark)** icon in the bottom blue status bar.
5. Click the **Upload (Right Arrow)** icon to flash via USB.

## Over-The-Air (OTA) Flashing
Once the board is running the initial firmware and connected to your local network, you can push updates wirelessly.
1. Find the ESP32's IP address in the serial monitor.
2. Open `platformio.ini`.
3. Uncomment the OTA upload lines and insert the IP address:
   `upload_protocol = espota`
   `upload_port = 192.168.X.X`
4. Click **Upload**. PlatformIO will compile and transmit the binary over Wi-Fi.

## Debugging Crashes
The project is configured with `build_type = debug` and `monitor_filters = esp32_exception_decoder`. 
If the ESP32 crashes (e.g., panic, watchdog timeout), leave the serial monitor open. PlatformIO will automatically intercept the hex backtrace and print the exact file and line number where the code failed.
