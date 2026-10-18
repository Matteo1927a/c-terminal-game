# c-terminal-game PhalsoPhobia

**PhalsoPhobia** is a terminal-based horror game written in C, developed as a university project.

## Overview

A team of paranormal investigators enters a haunted location to collect evidence and identify the ghost lurking within. Players must explore the map, gather evidence using the appropriate equipment, manage their mental health, and survive paranormal encounters.

The goal is to collect all three types of evidence and bring them back to the caravan before the investigators lose their sanity.

## Features

- **Multiplayer gameplay:** Configure a game with 1–4 players.
- **Map management:** Create and modify a circular linked-list map of haunted rooms.
- **Random events:** Randomly generated items, evidence, player turns, and ghost encounters.
- **Inventory system:** Collect, store, and use items with different effects.
- **Sanity management:** Monitor players' mental health as paranormal events unfold.
- **Difficulty levels:** Choose between Amateur, Intermediate, and Nightmare.
- **Dynamic memory management:** Allocate and release memory for players and map locations.
- **Interactive terminal interface:** Configure the game and perform actions through text-based menus.

## Technologies

- C (C11)
- Standard C library
- GCC

## Project Structure

```text
PhalsoPhobia/
├── main.c
├── gamelib.c
├── gamelib.h
└── README.md
```

- `main.c` — Entry point and main menu.
- `gamelib.c` — Game logic, map management, player actions, and game state.
- `gamelib.h` — Public function declarations and shared data type definitions.

## Requirements

- GCC or another compatible C compiler
- A terminal

## Compilation

Compile the source files with:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c gamelib.c -o gioco
```

## Run

On Linux or macOS:

```bash
./gioco
```

On Windows, run the generated executable from the terminal:

```bash
gioco.exe
```

## Gameplay

1. Configure the game and select the number of players and difficulty.
2. Create the haunted map by adding rooms.
3. Equip the investigators with randomly assigned starting items.
4. Explore rooms, collect items, and search for paranormal evidence.
5. Manage sanity and use special items to survive ghost encounters.
6. Return to the caravan with the collected evidence.

The game can be configured and played again after a match ends.

## Educational Objectives

This project explores several fundamental concepts in C programming:

- Structures, enumerations, pointers, and arrays.
- Dynamic memory allocation and deallocation.
- Circular linked lists.
- Modular program design using header and source files.
- Random number generation.
- Input validation and turn-based game logic.

## Author

**Matteo Santoro**

GitHub: [@Matteo1927a](https://github.com/Matteo1927a)
