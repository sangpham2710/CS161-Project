# CS161 - Final Project

This is our Minesweeper clone for the CS161 Final Assignment using only C++ windows.h library.

## Authors :wave:

- 21125063 - Phạm Văn Tấn Sang ([@sangpham2710](https://github.com/sangpham2710))
- 21125041 - Đặng Hoàng Nhật Hưng ([@doraemonidol](https://github.com/doraemonidol))

## Features

### Graphics

The game graphics are simple since they are just the characters printed to the console. We intended to renovate it using some graphic libraries, but since this version is working flawlessly and we are in love with its minimalistic look, we had decided to

### Basic Features

- Drawing the board (including cells, mines and flags).
- Changing the cell's state when it is selected.
- Timer.
- Functions: `new game`, `change board dimensions`, `select a function from console screen`.
- Generate random mines' positions.
- Receiving new position of the pointer (to select a cell) from console screen.
- Save the current playing board
- Leaderboard.

#### There are 3 game levels:

- **Beginner:** 9x9 cells, 10 mines
- **Intermediate:** 16x16 cells, 40 mines
- **Expert:** 30x16 cells, 99 mines

### Remarkable Features

- Change game's resolution corresponding to the current level to improve user experience.
- Guarantee that first cell clicked is not a mine.

## Hotkeys

- `W`, `A`, `S`, `D`: Move cursor.
- `J`, `Enter`: Select options | Reveal current cell.
- `K`: Reveal all adjacent cells of the current cell.
- `L`: Flag current cell.
- `O`: Save current board.
- `R`: Restart your game.
- `Esc`: Exit current playing game | Go back to menu.

## Documentation

- Rules of the game are based on: [Minesweeper Online](https://minesweeper.online/)

## Screenshots

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)

## License

> You can check out the full license [here](https://github.com/sangpham2710/CS161-Project/blob/main/LICENSE)

This project is licensed under the terms of the **MIT** license.
