# CS161 - Final Project

This is our Minesweeper clone for the CS161 Final Assignment using only the C++ windows.h library.

![image](https://user-images.githubusercontent.com/47299149/147375809-d24436fe-e176-4f62-b4b3-7fbc90dd6372.png)

---

## Authors :wave:

- 21125063 - Phạm Văn Tấn Sang ([@sangpham2710](https://github.com/sangpham2710))
- 21125041 - Đặng Hoàng Nhật Hưng ([@doraemonidol](https://github.com/doraemonidol))

---

## How To Use

To clone and run this application, you'll need [Git](https://git-scm.com) and [Chocolatey](https://chocolatey.org).

```bash
# Install make command
$ choco install make

# Clone this repository
$ git clone https://github.com/sangpham2710/CS161-Project.git

# Go into the repository
$ cd CS161-Project/minesweeper_v1

# Build the executable
$ make

# Run the game
$ bin/Release/minesweeper_v1.exe
```

---

## Technology Used

- ["windows.h" library](https://en.wikipedia.org/wiki/Windows.h)

---

## Features

### Graphics

The graphics are simple since they are just characters printed to the console.

#### Backstory

We intended to renovate it using some graphics libraries. However, since this version is working flawlessly and we are also in love with its minimalistic look, we've come to the decision that this will be our final version :smile:.

### Basic Features

- Drawing the board (including cells, mines and flags).
- Changing a cell's state when it is selected.
- A timer.
- Game levels.
- Generating random mines positions.
- Receiving the new position of the pointer (to select a cell) from the console.
- Saving the current state of the playing board.
- Loading the previously saved board.
- A leaderboard.
- Message boxes to prompt the user.

#### There are 3 game levels:

- **Beginner:** 9x9 cells, 10 mines.
- **Intermediate:** 16x16 cells, 40 mines.
- **Expert:** 30x16 cells, 99 mines.

### Remarkable Features

- Changing the game's resolution corresponding to the current level to improve user experience.
- Guaranteeing that the first clicked cell is not a mine.
- Making our own library for manipulating the console based on the "windows.h" library.

---

## How to Play

- `W`, `A`, `S`, `D`: Move cursor.
- `J`, `Enter`: Select options | Reveal current cell.
- `K`: Reveal all adjacent cells of the current cell.
- `L`: Flag current cell.
- `O`: Save current board.
- `R`: Restart current game.
- `Esc`: Exit current game | Go back to menu.

---

## Screenshots

![image](https://user-images.githubusercontent.com/47299149/147375758-7fe57c0b-d3b6-4f88-b93d-2b21aa20122b.png)
![image](https://user-images.githubusercontent.com/47299149/147375766-3c3ac961-ff60-49f6-b62b-f81c267dc730.png)
![image](https://user-images.githubusercontent.com/47299149/147375809-d24436fe-e176-4f62-b4b3-7fbc90dd6372.png)
![image](https://user-images.githubusercontent.com/47299149/147375815-c62fef09-3902-43ae-83e4-00e1d629707e.png)

---

## Additional Information

### Project Requirements

#### Basic Features

- Game rules: [Minesweeper Online](https://minesweeper.online/)
- Drawing the board (including cells, mines and flags).
- Changing a cell's state when it is selected.
- A timer.
- Functions: New game, change board dimensions, select a function from console screen.
- Randomizing the positions of the mines.
- Receiving the new position of the pointer (to select a cell) from the console.
- Saving the current state of the playing board.
- Loading the previously saved board.
- A leaderboard.

#### Other Features

- Using the keyboard (arrow keys) to select a cell.
- Rendering images.
- Using structs.

---

## License

> You can check out the full license [here](https://github.com/sangpham2710/CS161-Project/blob/main/LICENSE)

This project is licensed under the terms of the **MIT** license.
