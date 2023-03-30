#ifndef GAME_H
#define GAME_H

#include "curses.h"
//#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include "queue"

#include "snake.h"

enum mainMenuState {START, SKIN, ITEMS, MUSIC, ACOUSTICS,QUIT};
enum gameMenuState { ENDLESS, STAGE, PRACTICE, AI, RETURNMAIN };
enum practiceMenuState { STA1, STA2, STA3, STA4, STA5, STA6, STAEX, RETURNPRACTICE };
enum levelMenuState { EASY, NORMAL, HARD, LUNATIC, RETURNLEVEL };

enum class trackType {
    Freeze = 1,
    Shrink = 2,
    Auto = 3,
    Award = 4,
};

// Track Direction
enum class trackDirection {
    Down = 0,
    Right = 1,
    Up = 2,
    Left = 3,
};

class Game
{
public:

    Game();
    ~Game();

    void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

    void renderBoards() const;

    void renderStageOrPracticeModeBoard(std::string introduction, int stage, int level, std::string title, int mode = 0) const;

    void initializeGame(int diffculty = 0, bool needHealthBar = false);
    void autoDrive(int);
    void runGame();
    void autoGame();
    void enterAwardTime(int time);
    void renderPoints() const;
    void erasePoints() const;
    void renderAwardPoints() const;
    void renderDifficulty() const;
    void eraseDifficulty() const;
    void adjustDelay(int diffculty = 5);

    // For energy track
    void createRandomTrack();
    void findStartPoint();
    const char getTrackSymbol(int index) const;
    void rotateTrack(int rotate_time);
    void renderTrack();
    // Track Design
    void createLTrack();
    void createSixTrack();
    void createTriangleTrack();
    void createSnakeTrack();
    void createHeartTrack();

    // ---------------------------


    // For food
    void createRandomFood();
    void renderFood() const;
    // ---------------------------

    // For terrain
    void createBarrier(int Y, int X);
    void renderBarriers() const;
    // ---------------------------

    // For snake
    void renderSnake() const;
    void renderAwardSnake() const;
    void controlSnake(int key, bool isReversed = false) const;
    void controlAwardSnake(int key) const;
    void eraseSnake () const;
    // ---------------------------

    //For pause and other windows.
    void startGame();
    bool renderRestartMenu() const;
    bool renderPauseMenu(bool stageMode = false);
    void renderCongratulationMenu()const;
    void renderCheatingMenu()const;
    // -----------------------------




    //从上到下依次为：展示开始界面、展示模式选择界面、展示练习模式选择界面、展示难度选择界面、展示禁止打开练习关卡界面
    mainMenuState renderStartMenu() const;
    gameMenuState renderGameMenu() const;
    practiceMenuState renderPracticeMenu() const;
    levelMenuState renderLevelMenu() const;
    void renderPracticeOpenFailedMenu(bool Extra = false) const;
    bool renderRestartWarningMenu() const;
    // -----------------------------


    // For tailoring the skin color
    void renderSkin();
    void changeHeadColor();
    void changeBodyColor();
    // -----------------------------

    // For items
    void renderItem();
    void effectItem();
    void createRandomItem();
    void renderItemInfo() const;
    void renderFreezeInfo() const;
    void renderShrinkInfo() const;
    void renderAutoInfo() const;
    void renderAwardInfo() const; // TODO
    void cutLength(int);
    void cutDifficulty(int);
    // -----------------------------

    //stages
    void stage1(int diffculty, bool* success, bool isStageMode = false);
    void stage2(int diffculty, bool* success, bool isStageMode = false);
    void stage3(int diffculty, bool* success, bool isStageMode = false);
    void stage4(int diffculty, bool* success, bool isStageMode = false);
    void stage5(int diffculty, bool* success, bool isStageMode = false);
    void stage6(int diffculty, bool* success, bool isStageMode = false);
    void stageEX(bool* success);
    void startStageMode(int diffculty);
    // -----------------------------

    bool isAuto() const;

    //for StageClearRecord
    bool readStageClearRecord(int diffculty = 0, int stage = 1);
    bool writeStageOpenRecord(int diffculty, int stage, bool state = true);

    // playing music and acoustics
    void playMusic(int index) const;
    void stopMusic(int index) const;
    void MusicOnOff(int index);
    void AcousticsOnOff();

    // For comics
    void renderStartComics() const;
    void renderEndComics() const;

    //For auxiliary functions that can draw pictures.
    //画斜线（输入起始点、终点、并输入正/反斜线即可）(默认斜线贯穿屏幕，正斜杠起始点为左下角，反斜杠为左上角)
    void printSlash(int beginingY, int beginingX, bool isBackslash = false, int endingY = -1, int endingX = -1);
    //画斜线 (使用外部迭代指针（在stage4中使用）)
    void printSlash(int* beginingY, int* beginingX, bool isBackslash = false, int endingY = -1, int endingX = -1);
    //以给定中心为矩形中心画矩形（按指定长宽比例画，需要给出矩形上边界中点坐标）
    void printSquare(int middlePointY, int middlePointX, int width, int height, int anchorPointY, int anchorPointX);
    //擦除矩形两对边中点的障碍以供蛇行动（用法与上一函数相同，多加一参数LR：true为擦除左右两边，false为上下两边）
    void deleteSquare(int middlePointY, int middlePointX, int width, int height, int anchorPointY, int anchorPointX, int LR = true);

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;

    int roundNum;
    int difficultyOffset = 0;
    int mPoints = 0;
    int aPoints = 0; // record the award point
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;

    bool escape = false;
    bool restartChoice = true;

    void renderA() const;
    void clearA() const;

    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    // -------------------------

    // blank char
    const char blank = ' ';
    std::unique_ptr<Snake> mPtrSnake;
    std::unique_ptr<Snake> mPtrAwardSnake;


    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';
    // ------------------------


    // Item information
    Item mItem;
    bool is_auto = false;
    bool is_award = false;
    // ------------------------


    // Skin Color
    int mBodyColor = 2;
    int mHeadColor = 3;
    bool isAutoColor = false;
    // ------------------------


    // for music and acoustics
    bool isMusicOn = true;
    bool isAcousticsOn = true;
    int musicIndex = 5;
    std::string menuSound;
    // ------------------------


    // for track
    int track_X;
    int track_Y;
    int prev_len;
    // ------------------------

    // for health bar
    void renderHealthBar();
    void cutHealthBar();




};
// use for conveniently print menu
static void print_menu(WINDOW* menu, std::vector<std::string> options, int offset, int index);
// use for conveniently print paragraph
static void print_paragraph(WINDOW* menu, int menu_width, int offset, std::string paragraph);
// use for conveniently print menu with instruction
static void print_menu_with_ins(WINDOW* menu, std::vector<std::string> options, int offset, int index, WINDOW* instruction_window, std::vector<std::string> paragraph, int ins_width, int ins_offset);



#endif
