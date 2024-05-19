# CSI sensors project 

This is embedded application for material detection via WIFI CSI.
Project is focused on analyzing data from both ESP32 microcontrollers working as receiver and sender.
The main goal of this code is to make two ESPs working as spectrocope.

## Installation 

1. Install all liblaries (follow ESP-IDF documentation)
2. Clone this repository and navigate to it:
3. You can choose between two app types *receiver* and *sender*, to configure project run, and navigate to *CSI config* menu:
```bash idf.py menuconfig```
4. To build and flash app run:
```bash idf.py build flash```
