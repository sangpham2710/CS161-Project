
# CS161 - Final Project

This is our project on building a game called Minesweeper by using C++.


## Authors

- [@sangpham2710](https://github.com/sangpham2710)
- [@doraemonidol](https://github.com/doraemonidol)

## Documentation

[Documentation](https://linktodocumentation)


## Screenshots

![App Screenshot](https://via.placeholder.com/468x300?text=App+Screenshot+Here)


## Acknowledgements

 - [Awesome Readme Templates](https://awesomeopensource.com/project/elangosundar/awesome-README-templates)
 - [Awesome README](https://github.com/matiassingers/awesome-readme)
 - [How to write a Good readme](https://bulldogjob.com/news/449-how-to-write-a-good-readme-for-your-github-project)


## Function Reference

### Game Display

#### Set-up Display 

```c++
  void setupDisplay(const int &_boardWidth, const int &_boardHeight,
                    const int &currentLevel);
```

|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `_boardWidth` | `int` | Board's width |
| `_boardHeight` | `int` | Board's height |
| `currentLevel` | `int` | Keep track of level to adjust the window's size and font properties |


---
#### Next, we divide the play screen into different parts
- Main Board
- Number of Flags Left
- Timer
- Status Bar.

#### Main Board

```c++
  void displayBoard(int playerBoard[][MAX_BOARD_SIZE], const int &cursorRow,
                    const int &cursorCol, const bool &firstCall = false);
```

|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `playerBoard[][MAX_BOARD_SIZE]` | `int` | Board's width |
| `cursorRow` `and` `cursorCol` | `int` | Cursor's Position |
| `firstCall` | `bool` | Keep track of level to adjust the window's size and font properties |

This function is used to display the board with opened cells, flagged cells or questioned cells. In short, it displays the information of the board.

#### Number of Flags Left

```c++
  void displayNumFlags(const int &numFlags, const bool &firstCall = false);
```

|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `numFlags` | `int` | Number of flags left |
| `firstCall` | `bool` | Keep track of level to adjust the window's size and font properties |

This function is used to display how many flags do player have at the time he plays.

#### Timer

```c++
  void displayTimer(const long long &elapsedTime, const bool &firstCall = false);
```

|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `elapsedTime` | `long long` | Total time elapsed |
| `firstCall` | `bool` | Keep track of level to adjust the window's size and font properties |

This function is used to display how many flags do player have at the time he plays.

#### Board Status

```c++
  void displayBoardStatus(const std::string &boardStatus,
                          const bool &firstCall = false);
```
|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `boardStatus` | `string` | The message to show on the screen |
| `firstCall` | `bool` | Keep track of level to adjust the window's size and font properties |

This function takes a string as input and displays that string to the screen in some form of format. This acts as a message to the players when they do an invalid action in the game. This message is shown under the board.

---
#### Win & Lose Message

```c++
  void displayEndGame(const bool &win, const int &rank);
```

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `win`      | `bool` | **True** when player wins. **False** when player loses. |
|`rank`|`int`| Rank of player to display when he wins.

When the game finishes, there are 2 states whether the player wins or loses. This function will show a small message box in the middle of the screen to inform win/lose message and rank if he wins.
