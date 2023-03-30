#ifndef SNAKE_H
#define SNAKE_H
#include "curses.h"
#include <vector>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

enum class itemType {
    Freeze = 1,
    Shrink = 2,
    Auto = 3,
    Award = 4,
};



enum stateAfterMove {NONE, FOOD, ITEM};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    SnakeBody(int x, int y, int trackIndex);
    bool operator == (const SnakeBody & snakeBody);

    int getY() const;
    int getX() const;

    // record the track direction
    int trackIndex;

private:
    int mX;
    int mY;
};

// Snake class should have no dependency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    void senseItem(SnakeBody item);
    bool touchFood();
    void reInitializeSnake();

    // Check if the snake is dead
    bool checkBound(int,int);
    bool hitWall();
    bool hitSelf();
    bool hitBarrier();
    bool hitPortal();
    bool checkCollision(bool checkDistance = false);
    SnakeBody getLastTail();


    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    std::vector<SnakeBody>  copySnake();
    int getLength();
    SnakeBody createNewHead();
    SnakeBody createNewTail();
    void cutTail();
    stateAfterMove moveForward();
    stateAfterMove ThroughWall();

    // AI function
    bool canFindBfsPath(SnakeBody end);
    bool canFindBfsPathToTail();
    void wander();
    bool moveByPath();
    void followSnakeTail();
    std::vector<SnakeBody> bfsPath; // store AI path
    struct bfsNode{
        int prev_x = 0; // store the information of the previous node
        int prev_y = 0;
        int price = 0;  // weight, stand for the length of the path
    };
    // ------------------------


    // Barriers
    bool isBarrier(int x, int y);
    void addBarrier(SnakeBody barrier);
    void breakBarrier(int x, int y);
    std::vector<SnakeBody> getBarriers();
    // ------------------------

    std::vector<SnakeBody> mSnake;

    // For track
    std::vector<SnakeBody> mTrack;
    std::vector<SnakeBody> mTrackCopy;
    int TrackAward = 0;
    void isOnTrack(bool atStart);
    // ------------------------

    // health bar
    int life_num = 3;
    // ------------------------


    void getDirection(SnakeBody head, SnakeBody body);

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    SnakeBody mItem; // in class Snake, treat item as SnakeBody

    SnakeBody lastTail;
    std::vector<SnakeBody> barriers;//store the barrier



// ------------------------
};




class Item
{
public:
    Item();
    Item(itemType type,int x, int y);
    void clearItem();
    itemType getType() const;
    const char getSymbol() const;
    int getY() const;
    int getX() const;

private:
    itemType type;
    int mX;
    int mY;
};



#endif
