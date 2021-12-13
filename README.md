
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


##### Next, we divide the play screen into different parts: Main Board, Number of Flags Left, Timer and Status Bar.

#### Main Board

```c++
  void displayBoard(int playerBoard[][MAX_BOARD_SIZE], const int &cursorRow,
                  const int &cursorCol, const bool &firstCall = false);
```

|   Variable    | Type     | Description                |
| :------------ | :------- | :------------------------- |
| `playerBoard[][MAX_BOARD_SIZE]` | `int` | Board's width |
| `cursorRow` | `int` | Board's height |
| `cursorCol` | `int` | Board's height |
| `firstCall` | `bool` | Keep track of level to adjust the window's size and font properties |


#### Get item

```http
  GET /api/items/${id}
```

| Parameter | Type     | Description                       |
| :-------- | :------- | :-------------------------------- |
| `id`      | `string` | **Required**. Id of item to fetch |

#### add(num1, num2)

Takes two numbers and returns the sum.
