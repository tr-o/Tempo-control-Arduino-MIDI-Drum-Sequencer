
# Arduino MIDI Drum Sequencer

A versatile and customizable MIDI drum sequencer for Arduino, designed to control drum machines, virtual instruments, or hardware synths via MIDI.

![image](https://user-images.githubusercontent.com/98264095/236213322-418d5a74-1b8f-427f-88ee-cc4905032238.png)

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Hardware Setup](#hardware-setup)
- [Usage](#usage)
- [License](#license)

## Introduction

This project aims to provide a simple and customizable MIDI drum sequencer using an Arduino board. It is designed to work with drum machines, virtual instruments, or hardware synths, providing an easy and intuitive way to create rhythm patterns and sequences.

## Features

- Tempo control with customizable BPM
- Channel switching for different drum instruments
- Support for various MIDI note values
- Intuitive user interface with buttons and an LCD screen

## Overview

The Arduino-based MIDI drum sequencer sends MIDI messages to a host PC running a digital audio workstation (DAW) such as Waveform Free. The sequencer uses an I2C liquid crystal display (LCD) and buttons for control. The LiquidCrystal_I2C and MIDI libraries are included, and an LCD object is created with address 0x27 and 16 columns and 2 rows. Drum instruments and their corresponding MIDI notes are defined using constants and arrays, while rhythm patterns for the sequencer are defined using multi-dimensional arrays.

## Prerequisites

### Hardware

- Arduino board (e.g., Uno, Mega, etc.)
- MIDI interface (e.g., MIDI shield, breakout board, or DIY circuit)
- Push buttons, LEDs, and resistors for the user interface
- 16x2 LCD screen with I2C interface

### Software

- Arduino IDE
- MIDI library for Arduino
- LiquidCrystal_I2C library
- Digital Audio Workstation (DAW) with MIDI support, such as Waveform Free

## Hardware Setup

Follow these steps to connect the hardware components:

1. Connect the MIDI interface to your Arduino board as per the manufacturer's instructions or the MIDI library documentation.
2. Wire the push buttons, LEDs, and resistors to the appropriate digital pins on the Arduino board.
3. Connect the 16x2 LCD screen to the I2C interface on the Arduino board.
4. Refer to the code for specific pin assignments and adjust as needed.

## Usage

After setting up the hardware and uploading the code, follow these steps to use the drum sequencer:

1. Power on the Arduino board and connect it to your MIDI device or DAW via the MIDI interface.
2. Use the buttons to create and modify rhythm patterns, select drum instruments, and adjust the tempo.
3. Refer to the LCD screen for real-time feedback on your settings and sequences.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
