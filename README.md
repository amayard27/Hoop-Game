# Hoop‑Game

**Hoop‑Game** is a simple implementation of a hoop and ball game developed as part of the Fall 2024 Fundamentals of Computing course.  
This project uses **X11 graphics** via `gfx2` and to my knowledge **only works when launched over PuTTY with Xming (or another X server)**.

---

## Table of Contents

1. [Features](#features)  
2. [Prerequisites](#prerequisites)  
3. [Installation](#installation)  
4. [Running the Game](#running-the-game)  
5. [Controls](#controls)  
6. [Screenshot (Example Output)](#screenshot-example-output)  
7. [Project Structure](#project-structure)  
8. [License](#license)

---

## Features

- Basic interactive hoop and ball game  
- Graphic output using X11 (Xming)  
- Compiles with a standard C toolchain via the provided `Makefile`

---

## Prerequisites

To run the game you must have:

1. **PuTTY** — for SSH access to the remote machine  
2. **Xming** (or another X server) — to render the graphics locally  
3. A working **GCC/Make** toolchain on the remote machine

**Important:** Ensure that X11 forwarding is enabled in your PuTTY session:

- In PuTTY → *Connection → SSH → X11* → check *Enable X11 forwarding*
- `Xming` must be running on your local machine before connecting

---

## Installation

Clone the repository:

```sh
git clone https://github.com/amayard27/Hoop-Game.git
cd Hoop-Game

Running the Game

Once connected through PuTTY with X11 forwarding and with Xming running:

    Build the project:

make

Launch the game:

    ./game

    (Or use the provided target in the Makefile if different.)

If graphics do not appear, confirm that:

    Xming is running locally

    PuTTY X11 forwarding is enabled

    The $DISPLAY environment variable is set on the remote machine

Controls

(Add control instructions here — e.g., keys to move, shoot, exit)

Example (edit as needed):

Arrow keys  — move
Space       — shoot
Esc / q     — quit

Screenshot (Example Output)

Below is a sample screenshot of what the game should look like when running with Xming/Putty:

![Hoop-Game Screenshot](./images/example.png)

This uses standard GitHub Markdown image syntax. GitHub displays the image when viewing the README.

Project Structure

.
├── Makefile
├── README.md
├── gfx2.h
├── gfx2.o
├── project
└── project.c

    Makefile: Build instructions

    gfx2.*: X11 graphics support

    project.c: Main game source

    images/: Screenshots or assets

License

MIT License

Copyright (c) 2026 Andre Mayard
