# 🐍 Snake Game

A classic Snake game developed in **C++ using SDL3**, featuring real-time input handling, smooth grid-based rendering, and dynamic snake growth using a linked list. Includes collision detection and randomized apple spawning to replicate traditional gameplay.

---

## 🛠️ Setup Instructions

### 1. Install SDL3

Download the SDL3 development libraries for MinGW-w64 from the [official SDL repository](https://github.com/libsdl-org/SDL).

### 2. Folder Structure

Make sure your project is structured like this:

```
Snake-Game/
├── src/
│   ├── include/       # SDL3 headers
│   └── lib/           # SDL3 libraries
├── snake_game.cpp

```

Place the SDL `include/` and `lib/` folders inside a `src/` directory within your project.

### 3. Compile and Run

To compile the project, run:

```bash
g++ -Isrc/include -Lsrc/lib -o snake_game snake_game.cpp -lmingw32 -lSDL3
```

To run the game:

```bash
./snake_game
```

---

## 🕹️ How to Play

- Use **arrow keys** to control the snake:
  - `↑` Up
  - `↓` Down
  - `←` Left
  - `→` Right
- Collect red apples to grow longer.
- Avoid:
  - Running into the walls
  - Colliding with yourself
- The game ends upon collision, and your **score** (based on snake length) is printed in the **terminal**.

---

## 🎮 Demo

![Image](https://github.com/user-attachments/assets/c5793b26-9086-473d-a933-d6a495329ed9)

> A short looped gameplay demo of the project.

---

## 🚀 Features
- Real-time keyboard controls using arrow keys
- Linked list-based snake movement
- Random apple spawning without overlap  
- Collision Detection

---
