# CS161 - Final Project

This is our Minesweeper clone for the CS161 Final Assignment using only C++ windows.h library.

## Authors :wave:

- 21125063 - Phạm Văn Tấn Sang ([@sangpham2710](https://github.com/sangpham2710))
- 21125041 - Đặng Hoàng Nhật Hưng ([@doraemonidol](https://github.com/doraemonidol))

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

## Technologies Used

- ["windows.h" library](https://en.wikipedia.org/wiki/Windows.h)

## Features

### Graphics

The game's graphics are simple since they are just the characters printed onto the console. We intended to renovate it using some graphic libraries, but since this version is working flawlessly and we are in love with its minimalistic look, we had decided to

### Basic Features

- Drawing the board (including cells, mines and flags).
- Changing the cell's state when it is selected.
- Timer.
- Game levels.
- Generate random mines positions.
- Receiving new position of the pointer (to select a cell) from console screen.
- Save the current state of the playing board.
- Load the previously saved board.
- Leaderboard.
- Message box to prompt the user.

#### There are 3 game levels:

- **Beginner:** 9x9 cells, 10 mines
- **Intermediate:** 16x16 cells, 40 mines
- **Expert:** 30x16 cells, 99 mines

### Remarkable Features

- Change game's resolution corresponding to the current level to improve user experience.
- Guarantee that first cell clicked is not a mine.
- Make our own library for manipulating the console based on the "windows.h" library

## How to play

- `W`, `A`, `S`, `D`: Move cursor.
- `J`, `Enter`: Select options | Reveal current cell.
- `K`: Reveal all adjacent cells of the current cell.
- `L`: Flag current cell.
- `O`: Save current board.
- `R`: Restart your game.
- `Esc`: Exit current playing game | Go back to menu.

## Additional Information

### Requirements

#### Basic features

- Game rules: [Minesweeper Online](https://minesweeper.online/)
- Drawing the board (including cells, mines and flags).
- Changing the state of each cell when it is selected.
- Timer.
- Functions: new game, change board dimensions, select a function from console screen.
- Randomize positions of the mines.
- Receiving new position of the pointer (to select a cell) from console screen.
- Saving the current state of the board and loading in the next playing session.
- Saving high scores.

#### Other features

- Using keyboard (arrow keys) to select a cell.
- Rendering images.
- Using structs.

## License

> You can check out the full license [here](https://github.com/sangpham2710/CS161-Project/blob/main/LICENSE)

This project is licensed under the terms of the **MIT** license.
