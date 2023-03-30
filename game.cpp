#include <string>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "curses.h"

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>
#include "game.h"
#include <sstream>

// For music playing
#include <Windows.h>
#include <mmsystem.h>
#include <unistd.h>

#pragma comment (lib, "winmm.lib")



Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);

    initscr();
    // If there wasn't any key pressed don't wait for keypress

    nodelay(stdscr, true); // true for fluent experience, false for debug

    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);

    start_color();
    init_color(10, 1000, 500, 0);
    init_color(11, 1000, 1000, 0);
    init_color(12, 500,1000, 0);
    init_color(13, 0,1000, 0);
    init_color(14, 0,1000, 500);
    init_color(15, 0, 1000,1000);
    init_color(16, 0, 500,1000);
    init_color(17, 0, 0, 1000);
    init_color(18, 500, 0, 1000);
    init_color(19, 1000, 0, 1000);
    init_color(20, 1000, 0, 500);
    init_color(21, 1000, 0, 0);
    init_color(22, 525, 904, 1000); // for Freeze
    init_color(23, 0, 340, 1000);
    init_color(24, 644, 444, 900); // for Shrink
    init_color(25, 280, 0, 340);
    init_color(26, 200, 200, 200); // for starting
    init_color(27, 400, 400, 400);
    init_color(28, 600, 600, 600);
    init_color(29, 800, 800, 800);
    init_color(30, 992, 900, 348);
    init_color(31, 1000, 500, 30);
    init_color(32, 1000, 850, 200); // for the auto mode
    init_color(33, 428, 788, 212); // for easy mode
    init_color(34, 176, 400, 900); // for normal mode
    init_color(35, 600, 0,0);// for hard mode
    init_color(36, 444, 200, 516); // for lunatic mode
    init_color(37, 106, 360, 200);
    init_color(38, 620, 800, 350);
    init_color(39, 1000, 636, 100); // for award trial
    init_pair(1, 21, 21); // Food's default color
    init_pair(2,COLOR_WHITE,COLOR_WHITE); // Body's default color
    init_pair(3,10, 10); // Head's default color
    init_pair(4, 11, 11);
    init_pair(5, 12, 12);
    init_pair(6, 13, 13);
    init_pair(7, 14, 14);
    init_pair(8, 15, 15);
    init_pair(9, 16, 16);
    init_pair(10, 17, 17);
    init_pair(11, 18, 18);
    init_pair(12, 19, 19);
    init_pair(13, 20, 20);
    init_pair(14, COLOR_BLACK, COLOR_WHITE);
    init_pair(15, 23, 22);
    init_pair(16, 25, 24);
    init_pair(17, 22, COLOR_BLACK);
    init_pair(18, 24, COLOR_BLACK);
    init_pair(19, 26, 26); // displaying starting
    init_pair(20, 27, 27);
    init_pair(21, 28, 28);
    init_pair(22, 29, 29);
    init_pair(23, COLOR_WHITE, COLOR_WHITE);
    init_pair(24, 31, 30);
    init_pair(25, 30, COLOR_BLACK);
    init_pair(26, 32, 32);
    init_pair(27, 33, COLOR_BLACK);
    init_pair(28, 34, COLOR_BLACK);
    init_pair(29, 35, COLOR_BLACK);
    init_pair(30, 36, COLOR_BLACK);
    init_pair(31, 14, COLOR_BLACK);
    init_pair(32, 26, COLOR_BLACK);
    init_pair(33, 27, COLOR_BLACK);
    init_pair(34, 28, COLOR_BLACK);
    init_pair(35, 29, COLOR_BLACK);
    init_pair(36, COLOR_WHITE, COLOR_BLACK);
    init_pair(37, 37, 38);
    init_pair(38, 38, COLOR_BLACK);
    init_pair(39, 39, 39);

    // Music file
    mciSendString("open ./music/smile.mp3 alias EasyMusic", NULL, 0,NULL);
    mciSendString("open ./music/buddy.mp3 alias NormalMusic", NULL, 0, NULL);
    mciSendString("open ./music/goinghigher.mp3 alias HardMusic", NULL, 0, NULL);
    mciSendString("open ./music/epic.mp3 alias LunaticMusic", NULL, 0, NULL);
    mciSendString("open ./music/menumusic.mp3 alias Menu", NULL, 0, NULL);
    mciSendString("open ./music/auto.mp3 alias auto" ,NULL, 0, NULL);
    mciSendString("open ./music/dreams.mp3 alias end", NULL, 0, NULL);
    mciSendString("open ./music/track.mp3 alias track", NULL, 0, NULL);
    mciSendString("open ./music/cheatingOpenWarning.wav alias cheating", NULL, 0, NULL);
    mciSendString("open ./music/undoCheating.wav alias undoCheating", NULL, 0, NULL);
    mciSendString("open ./music/dead.wav alias dead", NULL, 0, NULL);

}

Game::~Game()
{
    // close music file
    mciSendString("close EasyMusic", NULL, 0, NULL);
    mciSendString("close NormalMusic", NULL, 0, NULL);
    mciSendString("close HardMusic", NULL, 0, NULL);
    mciSendString("close LunaticMusic", NULL, 0, NULL);
    mciSendString("close Menu", NULL, 0, NULL);
    mciSendString("close auto", NULL, 0, NULL);
    mciSendString("close end", NULL, 0, NULL);
    mciSendString("close track", NULL, 0, NULL);
    mciSendString("close cheating", NULL, 0, NULL);
    mciSendString("close undoCheating", NULL, 0, NULL);
    mciSendString("close dead", NULL, 0, NULL);


    for (int i = 0; i < this->mWindows.size(); i++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    //    wattrset(mWindows[0],COLOR_PAIR(30));
    mvwprintw(this->mWindows[0], 1, 2, "Welcome to The Snake Game!                                ");
    mvwprintw(this->mWindows[0], 2, 2, "Try your best to get the highest score                   ");
    mvwprintw(this->mWindows[0], 3, 2, "Hope this help kill your boring time                    ");
    mvwprintw(this->mWindows[0], 4, 2, "Implemented using C++ and libncurses  ");
    //    wattroff(mWindows[0],COLOR_PAIR(30));
    wrefresh(this->mWindows[0]);
}

void Game::renderA() const
{
    mvwprintw(this->mWindows[0],3,59,"Auto Mode");
    mvwprintw(this->mWindows[0],3,58, "|");
    mvwprintw(this->mWindows[0],3,68, "|");


//    mvwprintw(this->mWindows[0], 3, 42, "/");
//    mvwprintw(this->mWindows[0], 2, 43, "/");
//    mvwprintw(this->mWindows[0], 1, 44, "+");
//    mvwprintw(this->mWindows[0], 3, 43, "-");
//    mvwprintw(this->mWindows[0], 3, 44, "-");
//    mvwprintw(this->mWindows[0], 3, 45, "-");
//    mvwprintw(this->mWindows[0], 2, 45, "\\");
//    mvwprintw(this->mWindows[0], 3, 46, "\\");
//    mvwprintw(this->mWindows[0], 4, 47, "\\");
    wrefresh(this->mWindows[0]);
}

void Game::clearA() const
{
//    mvwprintw(this->mWindows[0], 4, 41, " ");
//    mvwprintw(this->mWindows[0], 3, 42, " ");
//    mvwprintw(this->mWindows[0], 2, 43, " ");
//    mvwprintw(this->mWindows[0], 1, 44, " ");
//    mvwprintw(this->mWindows[0], 3, 43, " ");
//    mvwprintw(this->mWindows[0], 3, 44, " ");
//    mvwprintw(this->mWindows[0], 3, 45, " ");
//    mvwprintw(this->mWindows[0], 2, 45, " ");
//    mvwprintw(this->mWindows[0], 3, 46, " ");
//    mvwprintw(this->mWindows[0], 4, 47, " ");
    mvwprintw(this->mWindows[0],3,58, " ");
    mvwprintw(this->mWindows[0],3,68, " ");
    mvwprintw(this->mWindows[0],3,59,"         ");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, " Manual");

    mvwprintw(this->mWindows[2], 3, 1, " Up: W");
    mvwprintw(this->mWindows[2], 4, 1, " Down: S");
    mvwprintw(this->mWindows[2], 5, 1, " Left: A");
    mvwprintw(this->mWindows[2], 6, 1, " Right: D");
    mvwprintw(this->mWindows[2], 8, 1, " Pause: Esc");

    mvwprintw(this->mWindows[2], 10, 1, " Difficulty");
    mvwprintw(this->mWindows[2], 13, 1, " Points");
    wrefresh(this->mWindows[2]);
}

void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 16 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 16, 1, " Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 16 - 2); i++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = " #" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 16 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 16 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

mainMenuState Game::renderStartMenu() const
{
    for (int i = 0; i < this->mWindows.size(); i++) {
        werase(this->mWindows[i]);
        wrefresh(this->mWindows[i]);
    }
    WINDOW* menu;
    WINDOW* opt_instruction;
    int menu_width = this->mScreenWidth * 2 / 3 / 3;
    int ins_width = this->mScreenWidth * 2 / 3 / 3 * 2;
    int height = this->mScreenHeight * 2 / 3;
    int menu_startX = mScreenWidth / 6;
    int ins_startX = mScreenWidth / 6 + menu_width;
    int startY = mScreenHeight / 6;

    menu = newwin(height, menu_width, startY, menu_startX);
    box(menu, 0, 0);
    opt_instruction = newwin(height, ins_width, startY, ins_startX);
    std::vector<std::string> menuItems = { "Start", "Skin", "Items","Music On/Off", "Acoustics On/Off", "Quit" };
    std::vector<std::string> ins_paragraph = { "Start your game.","Modify the skin of the snake as you wish.",
                                               "Game guide", "If the music is on, turn if off. And vice versa.",
                                               "If the acoustics is on, turn it off. And vice versa.", "Quit the game." };
    int index = 0;
    int offset = height / 2 - menuItems.size() / 2;
    print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
    mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
    mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
    wrefresh(opt_instruction);


    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }

            //作弊相关
            case'H':
            case'h':
            {
                std::fstream record("stageRecord.dat", record.binary | record.trunc | record.out);
                bool auxBoolVar = true;
                for (int i = 0; i < 25; i++) {
                    record.write((char*)&auxBoolVar, sizeof(auxBoolVar));
                }
                record.close();
                if (isAcousticsOn){
                    stopMusic(musicIndex);
                    mciSendString("play cheating from 0", NULL, 0, NULL);
                }
                renderCheatingMenu();
                mciSendString("stop cheating", NULL, 0, NULL);
                if (isMusicOn)
                    playMusic(musicIndex);
                return renderStartMenu();
                break;
            }
            case'X':
            case'x':
            {
                nodelay(stdscr, false);
                if (isAcousticsOn){
                    stopMusic(musicIndex);
                    mciSendString("play cheating from 0", NULL, 0, NULL);
                }
                renderCheatingMenu();
                mciSendString("stop cheating", NULL, 0, NULL);
                if (isMusicOn)
                    playMusic(musicIndex);
                return renderStartMenu();
                break;
            }
            case'Z':
            case'z':
            {
                nodelay(stdscr, true);
                if (isAcousticsOn){
                    stopMusic(musicIndex);
                    mciSendString("play undoCheating from 0", NULL, 0, NULL);
                }
                renderCheatingMenu();
                mciSendString("stop undoCheating", NULL, 0, NULL);
                if (isMusicOn)
                    playMusic(musicIndex);
                return renderStartMenu();
                break;
            }
        }

        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    werase(menu);
    werase(opt_instruction);
    wrefresh(menu);
    wrefresh(opt_instruction);
    delwin(menu);
    delwin(opt_instruction);

    if (index == 0)
        return mainMenuState::START;
    else if (index == 1)
        return mainMenuState::SKIN;
    else if (index == 2)
        return mainMenuState::ITEMS;
    else if (index == 3)
        return mainMenuState::MUSIC;
    else if (index == 4)
        return mainMenuState::ACOUSTICS;
    else
        return mainMenuState::QUIT;
}

gameMenuState Game::renderGameMenu() const
{
    for (int i = 0; i < this->mWindows.size(); i++) {
        werase(this->mWindows[i]);
        wrefresh(this->mWindows[i]);
    }
    WINDOW* menu;
    WINDOW* opt_instruction;
    int menu_width = this->mScreenWidth * 2 / 3 / 3;
    int ins_width = this->mScreenWidth * 2 / 3 / 3 * 2;
    int height = this->mScreenHeight * 2 / 3;
    int menu_startX = mScreenWidth / 6;
    int ins_startX = mScreenWidth / 6 + menu_width;
    int startY = mScreenHeight / 6;

    menu = newwin(height, menu_width, startY, menu_startX);
    box(menu, 0, 0);
    opt_instruction = newwin(height, ins_width, startY, ins_startX);
    std::vector<std::string> menuItems = { "Endless Mode", "Stage Mode", "Practice Mode", "Auto Mode", "Back" };
    std::vector<std::string> ins_paragraph = { "Try your best to get a score as high as you can and participate in the ranking.","Try to pass 6 stages without failing.When you first complete the game with a level higher than easy,you can challenge the \"stage EX\" in the Practice Mode.","Practice a single stage.","Appreciate the operation of the computer","Back to previous menu." };
    int index = 0;
    int offset = height / 2 - menuItems.size() / 2;
    print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
    mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
    mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
    wrefresh(opt_instruction);


    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }
        }

        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
        return gameMenuState::ENDLESS;
    else if (index == 1)
        return gameMenuState::STAGE;
    else if (index == 2)
        return gameMenuState::PRACTICE;
    else if (index == 3)
        return gameMenuState::AI;
    else
        return gameMenuState::RETURNMAIN;
}

practiceMenuState Game::renderPracticeMenu() const
{
    for (int i = 0; i < this->mWindows.size(); i++) {
        werase(this->mWindows[i]);
        wrefresh(this->mWindows[i]);
    }
    WINDOW* menu;
    int width = this->mScreenWidth * 2 / 3;
    int height = this->mScreenHeight * 2 / 3;
    int startX = mScreenWidth / 6;
    int startY = mScreenHeight / 6;
    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);

    std::vector<std::string> menuItems = { "Stage 1", "Stage 2", "Stage 3","Stage 4","Stage 5","Stage 6","Stage EX","Back" };
    int index = 0;
    int offset = height / 2 - menuItems.size() / 2;
    print_menu(menu, menuItems, offset, index);


    int key;

    while (true) {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu(menu, menuItems, offset, index);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu(menu, menuItems, offset, index);
                break;
            }
        }
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
        return practiceMenuState::STA1;
    else if (index == 1)
        return practiceMenuState::STA2;
    else if (index == 2)
        return practiceMenuState::STA3;
    else if (index == 3)
        return practiceMenuState::STA4;
    else if (index == 4)
        return practiceMenuState::STA5;
    else if (index == 5)
        return practiceMenuState::STA6;
    else if (index == 6)
        return practiceMenuState::STAEX;
    else
        return practiceMenuState::RETURNPRACTICE;
}

levelMenuState Game::renderLevelMenu() const
{
    for (int i = 0; i < this->mWindows.size(); i++) {
        werase(this->mWindows[i]);
        wrefresh(this->mWindows[i]);
    }
    WINDOW* menu;
    WINDOW* opt_instruction;
    int menu_width = this->mScreenWidth * 2 / 3 / 3;
    int ins_width = this->mScreenWidth * 2 / 3 / 3 * 2;
    int height = this->mScreenHeight * 2 / 3;
    int menu_startX = mScreenWidth / 6;
    int ins_startX = mScreenWidth / 6 + menu_width;
    int startY = mScreenHeight / 6;

    menu = newwin(height, menu_width, startY, menu_startX);
    box(menu, 0, 0);
    opt_instruction = newwin(height, ins_width, startY, ins_startX);
    std::vector<std::string> menuItems = { "Easy", "Normal", "Hard", "Lunatic", "Back" };
    std::vector<std::string> ins_paragraph = { "For people who never had played Gluttonous Snake before.","For people who are confident.","For master-hand.","Not for anyone.","Back to previous menu." };
    int index = 0;
    int offset = height / 2 - menuItems.size() / 2;
    print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
    mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
    mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
    wrefresh(opt_instruction);


    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu_with_ins(menu, menuItems, offset, index, opt_instruction, ins_paragraph, ins_width, 3);
                mvwprintw(opt_instruction, 1, ins_startX / 2 - 5, "* Explanation *");
                mvwprintw(opt_instruction, 2, ins_startX / 2 - 6, "-----------------");
                wrefresh(opt_instruction);
                break;
            }
        }

        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
        return levelMenuState::EASY;
    else if (index == 1)
        return levelMenuState::NORMAL;
    else if (index == 2)
        return levelMenuState::HARD;
    else if (index == 3)
        return levelMenuState::LUNATIC;
    else
        return levelMenuState::RETURNLEVEL;
}

void Game::renderPracticeOpenFailedMenu(bool Extra) const{
    if (isAcousticsOn)
        PlaySound("./music/warning.wav", NULL, SND_FILENAME|SND_ASYNC);
    WINDOW* warning = newwin(mGameBoardHeight * 0.5, mGameBoardWidth * 0.5, mGameBoardHeight * 0.25 + mInformationHeight, mGameBoardWidth * 0.25);
    box(warning, 0, 0);
    if(!Extra)
        print_paragraph(warning, mGameBoardWidth * 0.5, 3, "You can't practice this stage until you firstly come across this stage in the Stage Mode.");
    else
        print_paragraph(warning, mGameBoardWidth * 0.5, 3, "You can't play Extra stage until you firstly complete the game in the Stage Mode with the level higher than EASY");
    wattron(warning, COLOR_PAIR(14));
    mvwprintw(warning, mGameBoardHeight * 0.5 - 4, mGameBoardWidth / 4 - 1, "OK");
    wattroff(warning, COLOR_PAIR(14));
    wrefresh(warning);
    int key;
    while (true) {
        key = getch();
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(warning);
    return;
}

bool Game::renderRestartWarningMenu() const{
    if (isAcousticsOn)
        PlaySound("./music/warning.wav", NULL, SND_FILENAME|SND_ASYNC);
    WINDOW* menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = { "Yes , I'm sure what I'm doing.", "Let me think twice." };

    int index = 0;
    print_paragraph(menu, this->mGameBoardWidth * 0.5, 2, "Warning:When you choose to restart in the Stage Mode , you can only start from stage 1");
    print_menu(menu, menuItems, 6, index);
    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu(menu, menuItems, 6, index);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu(menu, menuItems, 6, index);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Game::isAuto() const {
    return this->is_auto;
}

bool Game::renderRestartMenu() const
{
    stopMusic(musicIndex);
    WINDOW* menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = { "Restart", "Return to menu" };

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 2, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 2, pointString.c_str());
    wattron(menu, COLOR_PAIR(14));
    mvwprintw(menu, 0 + offset, 2, menuItems[0].c_str());
    wattroff(menu, COLOR_PAIR(14));
    mvwprintw(menu, 1 + offset, 2, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, COLOR_PAIR(14));
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                wattroff(menu, COLOR_PAIR(14));
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, COLOR_PAIR(14));
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                wattroff(menu, COLOR_PAIR(14));
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0) {
        return true;
    }
    else {
        return false;
    }

}

// Menu for pause
bool Game::renderPauseMenu(bool stageMode)
{
    stopMusic(musicIndex);
    if (isAcousticsOn)
        PlaySound("./music/pause.wav", NULL, SND_FILENAME|SND_ASYNC);
    if (is_auto)
        mciSendString("stop auto", NULL, 0, NULL);
    WINDOW* menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = { "Continue", "Restart", "Music On/Off", "Acoustics On/Off", "Return to Menu" };

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 2, "Your Current Score: %d", mPoints);
    wattron(menu, COLOR_PAIR(14));
    mvwprintw(menu, 0 + offset, 2, menuItems[0].c_str());
    wattroff(menu, COLOR_PAIR(14));
    for (int i = 1; i < menuItems.size(); i++)
        mvwprintw(menu, i + offset, 2, menuItems[i].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, COLOR_PAIR(14));
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                wattroff(menu, COLOR_PAIR(14));
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, COLOR_PAIR(14));
                mvwprintw(menu, index + offset, 2, menuItems[index].c_str());
                wattroff(menu, COLOR_PAIR(14));
                break;
            }
        }

        wrefresh(menu);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            if (index == 2){
                MusicOnOff(musicIndex);
            } else if (index == 3){
                AcousticsOnOff();
            } else{
                wclear(menu);
                wrefresh(menu);
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    // refreshing the windows behind
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            mvwprintw(mWindows[1], height/2 + j, width/2+i, " ");
    wrefresh(mWindows[1]);
    delwin(menu);

    if (index == 0)     // continue
    {
        this->escape = false;
        return true;
    }
    else if (index == 1) { // restart
        if (!stageMode) {                   // endless mode
            this->escape = false;
            return false;
        }
        else{                               // stage mode
            bool confirm = renderRestartWarningMenu();
            if (confirm) {
                this->escape = false;
                return false;
            }
            else
                return renderPauseMenu(true);
        }
    }
    else {                // escape
        this->escape = true;
        return false;
    }

}

void Game::renderCongratulationMenu()const {
    WINDOW* congratulation = newwin(mGameBoardHeight * 0.5, mGameBoardWidth * 0.5, mGameBoardHeight * 0.25 + mInformationHeight, mGameBoardWidth * 0.25);
    box(congratulation, 0, 0);
    print_paragraph(congratulation, mGameBoardWidth * 0.5, 3, "Congratulations! Let's start the next stage!");
    wattron(congratulation, COLOR_PAIR(14));
    mvwprintw(congratulation, mGameBoardHeight * 0.5 - 4, mGameBoardWidth/4-1, "OK");
    wattroff(congratulation, COLOR_PAIR(14));
    wrefresh(congratulation);
    int key;
    while (true) {
        key = getch();
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(congratulation);
    return;
}

void Game::renderCheatingMenu()const {
    WINDOW* menu;
    int width = this->mScreenWidth * 0.5;
    int height = this->mScreenHeight * 0.5;
    int startX = this->mScreenWidth * 0.25;
    int startY = this->mScreenHeight * 0.25;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = { "My friend told me about that.", "I tried it by myself." };
    int index = 0;
    print_paragraph(menu, mScreenWidth * 0.5, 2, "You cheater! Where did you learn the cheating code?");
    print_menu(menu, menuItems, 6, index);
    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            if (isAcousticsOn)
                PlaySound("./music/menu.wav", NULL, SND_FILENAME | SND_ASYNC);
            index--;
            index = (index < 0) ? menuItems.size() - 1 : index;
            print_menu(menu, menuItems, 6, index);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            if (isAcousticsOn)
                PlaySound("./music/menu.wav", NULL, SND_FILENAME | SND_ASYNC);
            index++;
            index = (index > menuItems.size() - 1) ? 0 : index;
            print_menu(menu, menuItems, 6, index);
            break;
        }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);
    if (index == 0) {
        WINDOW* menu;
        int width = this->mScreenWidth * 0.5;
        int height = this->mScreenHeight * 0.5;
        int startX = this->mScreenWidth * 0.25;
        int startY = this->mScreenHeight * 0.25;

        menu = newwin(height, width, startY, startX);
        box(menu, 0, 0);
        std::vector<std::string> menuItems = { "Hyx.", "Hbh", "Lz", "Mind your own business"};
        int index = 0;
        print_paragraph(menu, mScreenWidth * 0.5, 2, "Which of the following was the mole who leaked the cheating code?");
        print_menu(menu, menuItems, 5, index);
        wrefresh(menu);
        while (true)
        {
            key = getch();
            switch (key)
            {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                index--;
                index = (index < 0) ? menuItems.size() - 1 : index;
                print_menu(menu, menuItems, 5, index);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME | SND_ASYNC);
                index++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                print_menu(menu, menuItems, 5, index);
                break;
            }
            }
            wrefresh(menu);
            if (key == ' ' || key == 10) // enter or blankspace
            {
                if (isAcousticsOn)
                    PlaySound("./music/pick.wav", NULL, SND_FILENAME | SND_ASYNC);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        delwin(menu);
        if (index == 3) {
            WINDOW* menu = newwin(mScreenHeight * 0.5, mScreenWidth * 0.5, mScreenHeight * 0.25, mScreenWidth * 0.25);
            box(menu, 0, 0);
            print_paragraph(menu, mScreenWidth * 0.5, 3, "Maybe I'll let you off this time...");
            wattron(menu, COLOR_PAIR(14));
            mvwprintw(menu, mScreenHeight*3/8-1, mScreenWidth / 4 - 1, "OK");
            wattroff(menu, COLOR_PAIR(14));
            wrefresh(menu);
            while (true) {
                key = getch();
                if (key == ' ' || key == 10) // enter or blankspace
                {
                    if (isAcousticsOn)
                        PlaySound("./music/pick.wav", NULL, SND_FILENAME | SND_ASYNC);
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
//            werase(menu);
//            wrefresh(menu);
            delwin(menu);
            return;
        }
        else{
            WINDOW* menu = newwin(mScreenHeight * 0.5, mScreenWidth * 0.5, mScreenHeight * 0.25, mScreenWidth * 0.25);
            box(menu, 0, 0);
            print_paragraph(menu, mScreenWidth * 0.5, 3, "We'll get rid of him/her properly, thank you.");
            wattron(menu, COLOR_PAIR(14));
            mvwprintw(menu, mScreenHeight*3/8-1, mScreenWidth / 4 - 1, "OK");
            wattroff(menu, COLOR_PAIR(14));
            wrefresh(menu);
            int key;
            while (true) {
                key = getch();
                if (key == ' ' || key == 10) // enter or blankspace
                {
                    if (isAcousticsOn)
                        PlaySound("./music/pick.wav", NULL, SND_FILENAME | SND_ASYNC);
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
//            werase(menu);
//            wrefresh(menu);
            delwin(menu);
            return;
        }
    }
    else {
        WINDOW* menu = newwin(mScreenHeight * 0.5, mScreenWidth * 0.5, mScreenHeight * 0.25, mScreenWidth * 0.25);
        box(menu, 0, 0);
        print_paragraph(menu, mScreenWidth * 0.5, 3, "Well, I'll trust you this time, but only this time");
        wattron(menu, COLOR_PAIR(14));
        mvwprintw(menu, mScreenHeight*3/8-1, mScreenWidth / 4 - 1, "OK");
        wattroff(menu, COLOR_PAIR(14));
        wrefresh(menu);
        while (true) {
            key = getch();
            if (key == ' ' || key == 10) // enter or blankspace
            {
                if (isAcousticsOn)
                    PlaySound("./music/pick.wav", NULL, SND_FILENAME | SND_ASYNC);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
//        werase(menu);
//        wrefresh(menu);
        delwin(menu);
        return;
    }
}



// Skin System -----------------------------
void Game::renderSkin() {
    WINDOW* menu;
    int width = this->mScreenWidth / 2;
    int height = this->mScreenHeight / 2;
    int startX = (mScreenWidth - width) / 2;
    int startY = (mScreenHeight - height) / 2;

    std::vector<std::string> menuItems = { "Head", "Body", "Back" };

    int index = 0;
    int offset = height / 2;

    int key;

    while (index != 2) {
        menu = newwin(height, width, startY, startX);
        index = 0;
        werase(menu);
        box(menu, 0, 0);
        wattron(menu, COLOR_PAIR(14));
        mvwprintw(menu, 0 + offset, width / 2 - 2, menuItems[0].c_str());
        wattroff(menu, COLOR_PAIR(14));
        mvwprintw(menu, 1 + offset, width / 2 - 2, menuItems[1].c_str());
        mvwprintw(menu, 2 + offset, width / 2 - 2, menuItems[2].c_str());
        wrefresh(menu);
        while (true)
        {
            key = getch();
            switch (key)
            {
                case 'W':
                case 'w':
                case KEY_UP:
                {
                    if (isAcousticsOn)
                        PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                    mvwprintw(menu, index + offset, width / 2 - 2, menuItems[index].c_str());
                    index--;
                    index = (index < 0) ? menuItems.size() - 1 : index;
                    wattron(menu, COLOR_PAIR(14));
                    mvwprintw(menu, index + offset, width / 2 - 2, menuItems[index].c_str());
                    wattroff(menu, COLOR_PAIR(14));
                    break;
                }
                case 'S':
                case 's':
                case KEY_DOWN:
                {
                    if (isAcousticsOn)
                        PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                    mvwprintw(menu, index + offset, width / 2 - 2, menuItems[index].c_str());
                    index++;
                    index = (index > menuItems.size() - 1) ? 0 : index;
                    wattron(menu, COLOR_PAIR(14));
                    mvwprintw(menu, index + offset, width / 2 - 2, menuItems[index].c_str());
                    wattroff(menu, COLOR_PAIR(14));
                    break;
                }
            }
            wrefresh(menu);
            if (key == ' ' || key == 10) // enter or blankspace
            {
                if (isAcousticsOn)
                    PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        delwin(menu);

        if (index == 0)
            changeHeadColor();
        else if (index == 1)
            changeBodyColor();
    }
}

void Game::changeHeadColor() {
    WINDOW* board;
    int width = this->mScreenWidth * 2 / 3;
    int height = this->mScreenHeight * 2 / 3;
    int startX = (mScreenWidth - width) / 2;
    int startY = (mScreenHeight - height) / 2;
    int offset = height / 2 - 8, colorNum = std::min(height - 6, 12);

    board = newwin(height, width, startY, startX);
    box(board, 0, 0);
    mvwprintw(board, 1, 4, "Select the color you like for Snake's head!");
    mvwprintw(board, height / 2 - 7, 6, "NO.| Color");
    wattron(board, COLOR_PAIR(14));
    mvwprintw(board, 2 + offset, 6, std::to_string(2 - 1).c_str());
    wattroff(board, COLOR_PAIR(14));
    wattrset(board, COLOR_PAIR(2));
    mvwprintw(board, 2 + offset, 10, ".......");
    mvwaddch(board, height / 3, width / 4 * 3, '.');
    wattroff(board, COLOR_PAIR(2));
    wattrset(board, COLOR_PAIR(mBodyColor));
    for (int i = 0; i < height / 3 - 1; i++)
        mvwaddch(board, height / 3 + i + 1, width / 4 * 3, '.');
    wattroff(board, COLOR_PAIR(mBodyColor));
    for (int i = 3; i < colorNum + 2; i++) {
        mvwprintw(board, i + offset, 6, std::to_string(i - 1).c_str());
        wattrset(board, COLOR_PAIR(i));
        mvwprintw(board, i + offset, 10, ".......");
        wattroff(board, COLOR_PAIR(i));
    }

    wrefresh(board);

    int key, index = 2;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                index--;
                index = (index < 2) ? (colorNum + 1) : index;
                wattron(board, COLOR_PAIR(14));
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                wattroff(board, COLOR_PAIR(14));
                wattrset(board, COLOR_PAIR(index));
                mvwprintw(board, height / 3, width * 3 / 4, ".");
                wattroff(board, COLOR_PAIR(index));
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                index++;
                index = (index > colorNum + 1) ? 2 : index;
                wattron(board, COLOR_PAIR(14));
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                wattroff(board, COLOR_PAIR(14));
                wattrset(board, COLOR_PAIR(index));
                mvwprintw(board, height / 3, width * 3 / 4, ".");
                wattroff(board, COLOR_PAIR(index));
                break;
            }
        }

        wrefresh(board);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            werase(board);
            wrefresh(board);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
    delwin(board);
    mHeadColor = index;
}

void Game::changeBodyColor() {
    WINDOW* board;
    int width = this->mScreenWidth * 2 / 3;
    int height = this->mScreenHeight * 2 / 3;
    int startX = (mScreenWidth - width) / 2;
    int startY = (mScreenHeight - height) / 2;
    int offset = height / 2 - 8;
    int colorNum = std::min(height - 6, 12);

    board = newwin(height, width, startY, startX);
    box(board, 0, 0);
    mvwprintw(board, 1, 6, "Select the color you like for Snake's body!");
    mvwprintw(board, height / 2 - 7, 6, "NO.| Color");
    wattron(board, COLOR_PAIR(14));
    mvwprintw(board, 2 + offset, 6, std::to_string(2 - 1).c_str());
    wattroff(board, COLOR_PAIR(14));
    wattrset(board, COLOR_PAIR(mHeadColor));
    mvwaddch(board, height / 3, width / 4 * 3, '.');
    wattroff(board, COLOR_PAIR(mHeadColor));
    wattrset(board, COLOR_PAIR(2));
    mvwprintw(board, 2 + offset, 10, "......");
    for (int i = 0; i < height / 3 - 1; i++)
        mvwaddch(board, height / 3 + i + 1, width / 4 * 3, '.');
    wattroff(board, COLOR_PAIR(2));
    for (int i = 3; i < colorNum + 2; i++) {
        mvwprintw(board, i + offset, 6, std::to_string(i - 1).c_str());
        wattrset(board, COLOR_PAIR(i));
        mvwprintw(board, i + offset, 10, "......");
        wattroff(board, COLOR_PAIR(i));
    }

    wrefresh(board);

    int key, index = 2;
    while (true)
    {
        key = getch();
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                index--;
                index = (index < 2) ? (colorNum + 1) : index;
                wattron(board, COLOR_PAIR(14));
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                wattroff(board, COLOR_PAIR(14));
                wattrset(board, COLOR_PAIR(index));
                for (int i = 0; i < height / 3 - 1; i++)
                    mvwaddch(board, height / 3 + i + 1, width / 4 * 3, '.');
                wattroff(board, COLOR_PAIR(index));
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                if (isAcousticsOn)
                    PlaySound("./music/menu.wav", NULL, SND_FILENAME|SND_ASYNC);
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                index++;
                index = (index > colorNum + 1) ? 2 : index;
                wattron(board, COLOR_PAIR(14));
                mvwprintw(board, index + offset, 6, std::to_string(index - 1).c_str());
                wattroff(board, COLOR_PAIR(14));
                wattrset(board, COLOR_PAIR(index));
                for (int i = 0; i < height / 3 - 1; i++)
                    mvwaddch(board, height / 3 + i + 1, width / 4 * 3, '.');
                wattroff(board, COLOR_PAIR(index));
                break;
            }
        }

        wrefresh(board);
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            werase(board);
            wrefresh(board);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    mBodyColor = index;
}

// ----------------------------------------

// health bar System

void Game::renderHealthBar() {
    mvwprintw(mWindows[0], 1, 58, "Health Bar:");
    for(int i = 0; i < this->mPtrSnake->life_num ; ++ i){
        wattrset(mWindows[0], COLOR_PAIR(1));
        mvwaddch(mWindows[0],1, 70 + i * 2 , ' ');
        wattroff(mWindows[0], COLOR_PAIR(1));
        mvwaddch(mWindows[0],1, 71 + i * 2, ' ');
    }
    wrefresh(mWindows[0]);
}

void Game::cutHealthBar(){
    int k = this->mPtrSnake->life_num - 1;
    mvwaddch(mWindows[0],1, 70 + k * 2, ' ');
    wrefresh(mWindows[0]);
}

// ----------------------------------------


// Items System ---------------------------

void Game::createRandomItem()
{
    /* create an item at random places
    * make sure that the item doesn't overlap with the snake and food
    */
    //    (this->mPtrSnake)->setRandomSeed();
    int x, y;
    bool success = false;
    while (not success) {
        x = std::rand() % (mGameBoardWidth - 2) + 1;
        y = std::rand() % (mGameBoardHeight - 2) + 1;

        if(this->mPtrSnake->isBarrier(x, y)){
            success = false;
            continue;
        }


        if ((this->mPtrSnake)->isPartOfSnake(x, y) || (x == mFood.getX() && y == mFood.getY())) {
            success = false;
            continue;
        }

        SnakeBody item_(x, y);

        // TODO 概率合理化
        // Random Item, could be adjusted according to the number of items
        switch (x % 7) {
            case 0:{
                Item item(itemType::Freeze, x, y);
                this->mItem = item;
                break;
            }
            case 1: case 2: case 3: {
                Item item(itemType::Shrink, x, y);
                this->mItem = item;
                break;
            }
            case 4: case 5: {
                Item item(itemType::Auto, x, y);
                this->mItem = item;
                break;
            }
            case 6: {
                Item item(itemType::Award, x, y);
                this->mItem = item;
                break;
            }
            default:break;
        }

        this->mPtrSnake->senseItem(item_);

        success = true;
    }
}

void Game::renderItem() {
    if(mItem.getX() == -1 && mItem.getY() == -1){
        return;
    } else {
        switch (mItem.getType()) {
            case itemType::Freeze:
                wattrset(mWindows[1], COLOR_PAIR(15));
                mvwaddch(this->mWindows[1], this->mItem.getY(), this->mItem.getX(), this->mItem.getSymbol());
                wattroff(mWindows[1], COLOR_PAIR(15));
                break;
            case itemType::Shrink:
                wattrset(mWindows[1], COLOR_PAIR(16));
                mvwaddch(this->mWindows[1], this->mItem.getY(), this->mItem.getX(), this->mItem.getSymbol());
                wattroff(mWindows[1], COLOR_PAIR(16));
                break;
            case itemType::Auto:
                wattrset(mWindows[1], COLOR_PAIR(24));
                mvwaddch(this->mWindows[1], this->mItem.getY(), this->mItem.getX(), this->mItem.getSymbol());
                wattroff(mWindows[1], COLOR_PAIR(24));
                break;
            case itemType::Award:
                wattrset(mWindows[1], COLOR_PAIR(37));
                mvwaddch(this->mWindows[1], this->mItem.getY(), this->mItem.getX(), this->mItem.getSymbol());
                wattroff(mWindows[1], COLOR_PAIR(37));
                break;

        }
        wrefresh(this->mWindows[1]);
    }
}

void Game::effectItem() {
    switch (this->mItem.getType()) {
        case itemType::Freeze: {
            if (isAcousticsOn)
                PlaySound("./music/freezing.wav", NULL, SND_FILENAME|SND_ASYNC);
            cutDifficulty(1);
            renderDifficulty();
            adjustDelay();
            break;
        }
        case itemType::Shrink: {
            if (isAcousticsOn)
                PlaySound("./music/shrinking.wav", NULL, SND_FILENAME|SND_ASYNC);
            cutLength(1);
            break;
        }
        case itemType::Auto:{
            this->is_auto = true;
            break;
        }
        case itemType::Award:{
            if (isAcousticsOn)
                PlaySound("./music/portal.wav", NULL, SND_FILENAME|SND_ASYNC);
            is_award = true;
            break;
        }
        default: {
            break;
        }
    }
}

// item effect, but whether secure?
void Game::cutDifficulty(int minus)
{
    if (mDifficulty > 0) {
        ++difficultyOffset;
    }
    else {
        return;
    }

}

// cut the length of snake by len and erase these output
void Game::cutLength(int len) {
    if ((this->mPtrSnake->getLength() - len) < 3) {
        return;
    }
    else {
        int length = mPtrSnake->getLength();
        for (int i = 0; i < len; ++i) {
            mvwaddch(mWindows[1], (this->mPtrSnake->getSnake()[length - i - 1]).getY(),
                     (this->mPtrSnake->getSnake()[length - i - 1]).getX(), this->blank);
            this->mPtrSnake->cutTail();
        }
    }
}


void Game::renderItemInfo() const {
    WINDOW* board;
    int width = this->mScreenWidth;
    int height = this->mScreenHeight;
    int startX = 0;
    int startY = 0;
    int key;

    board = newwin(height, width, startY, startX);
    box(board, 0, 0);
    mvwprintw(board, 1, width / 2 - 5, "ITEM LIST");
    wattron(board, COLOR_PAIR(14));
    mvwprintw(board, height - 2, width / 2 - 4, "--Back--");
    wattroff(board, COLOR_PAIR(14));
    wrefresh(board);
    renderFreezeInfo();
    renderShrinkInfo();
    renderAutoInfo();
    renderAwardInfo();

    while (true) {
        key = getch();
        if (key == ' ' || key == 10) // enter or blankspace
        {
            if (isAcousticsOn)
                PlaySound("./music/pick.wav", NULL, SND_FILENAME|SND_ASYNC);
            werase(board);
            wrefresh(board);
            break;
        }
    }
    delwin(board);
}

void Game::renderFreezeInfo() const {
    WINDOW* board;
    int width = this->mScreenWidth - 2;
    int height = 5;
    int startX = 1;
    int startY = 2;
    board = newwin(height, width, startY, startX);
    box(board, 0, 0);
    wattron(board, COLOR_PAIR(15));
    mvwaddch(board, 2, 3, 'F');
    wattroff(board, COLOR_PAIR(15));
    // the frame
    mvwaddch(board, 1, 2, '+');
    mvwaddch(board, 1, 4, '+');
    mvwaddch(board, 3, 2, '+');
    mvwaddch(board, 3, 4, '+');
    mvwaddch(board, 1, 3, '-');
    mvwaddch(board, 3, 3, '-');
    mvwaddch(board, 2, 2, '|');
    mvwaddch(board, 2, 4, '|');

    wattron(board, COLOR_PAIR(17));
    mvwprintw(board, 1, 7, "The Freezing Potion");
    wattroff(board, COLOR_PAIR(17));
    mvwprintw(board, 2, 7, "Generation: randomly on the ground with frequency level 1 (the highest)");
    mvwprintw(board, 3, 7, "Function: reduce the speed by one level");
    wrefresh(board);
}

void Game::renderShrinkInfo() const {
    WINDOW* board;
    int width = mScreenWidth - 2;
    int height = 5;
    int startX = 1;
    int startY = 7;
    board = newwin(height, width, startY, startX);
    box(board, 0, 0);

    wattron(board, COLOR_PAIR(16));
    mvwaddch(board, 2, 3, 'S');
    wattroff(board, COLOR_PAIR(16));
    // the frame
    mvwaddch(board, 1, 2, '+');
    mvwaddch(board, 1, 4, '+');
    mvwaddch(board, 3, 2, '+');
    mvwaddch(board, 3, 4, '+');
    mvwaddch(board, 1, 3, '-');
    mvwaddch(board, 3, 3, '-');
    mvwaddch(board, 2, 2, '|');
    mvwaddch(board, 2, 4, '|');

    wattron(board, COLOR_PAIR(18));
    mvwprintw(board, 1, 7, "The Shrinking Potion");
    wattroff(board, COLOR_PAIR(19));
    mvwprintw(board, 2, 7, "Generation: randomly on the ground with frequency level 2");
    mvwprintw(board, 3, 7, "Function: reduce the snake length by one");
    wrefresh(board);
}


void Game::renderAutoInfo() const{
    WINDOW* board;
    int width = mScreenWidth - 2;
    int height = 5;
    int startX = 1;
    int startY = 12;
    board = newwin(height, width, startY, startX);
    box(board, 0, 0);

    wattron(board, COLOR_PAIR(24));
    mvwaddch(board, 2, 3, 'L');
    wattroff(board, COLOR_PAIR(24));
    // the frame
    mvwaddch(board, 1, 2, '+');
    mvwaddch(board, 1, 4, '+');
    mvwaddch(board, 3, 2, '+');
    mvwaddch(board, 3, 4, '+');
    mvwaddch(board, 1, 3, '-');
    mvwaddch(board, 3, 3, '-');
    mvwaddch(board, 2, 2, '|');
    mvwaddch(board, 2, 4, '|');

    wattron(board, COLOR_PAIR(25));
    mvwprintw(board, 1, 7, "The Lazy Potion");
    wattroff(board, COLOR_PAIR(25));
    mvwprintw(board, 2, 7, "Generation: randomly on the ground with frequency level 3");
    mvwprintw(board, 3, 7, "Function: give you some rest");
    wrefresh(board);
}


void Game::renderAwardInfo() const{
    WINDOW* board;
    int width = mScreenWidth - 2;
    int height = 5;
    int startX = 1;
    int startY = 17;
    board = newwin(height, width, startY, startX);
    box(board, 0, 0);

    wattron(board, COLOR_PAIR(37));
    mvwaddch(board, 2, 3, 'T');
    wattroff(board, COLOR_PAIR(37));
    // the frame
    mvwaddch(board, 1, 2, '+');
    mvwaddch(board, 1, 4, '+');
    mvwaddch(board, 3, 2, '+');
    mvwaddch(board, 3, 4, '+');
    mvwaddch(board, 1, 3, '-');
    mvwaddch(board, 3, 3, '-');
    mvwaddch(board, 2, 2, '|');
    mvwaddch(board, 2, 4, '|');

    wattron(board, COLOR_PAIR(38));
    mvwprintw(board, 1, 7, "The Award Portal");
    wattroff(board, COLOR_PAIR(38));
    mvwprintw(board, 2, 7, "Generation: randomly on the ground with frequency level 3");
    mvwprintw(board, 3, 7, "Function: send you to the bonus level");
    wrefresh(board);
}


// ----------------------------------------
void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints + this->aPoints);
    mvwprintw(this->mWindows[2], 14, 2, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::erasePoints() const
{
    mvwprintw(this->mWindows[2], 14, 2, "   ");
    wrefresh(this->mWindows[2]);
}

void Game::renderAwardPoints() const {
    std::string pointString = std::to_string(this->aPoints);
    mvwprintw(this->mWindows[2], 3, 2, "Award Points:");
    mvwprintw(this->mWindows[2], 5, 2, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 11, 2, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}


void Game::eraseDifficulty() const
{
    mvwprintw(this->mWindows[2], 11, 2, "    ");
    wrefresh(this->mWindows[2]);
}


// Food System
void Game::createRandomFood()
{
    /* create food at random places
    * make sure that the food doesn't overlap with the snake and item
    */
//    (this->mPtrSnake)->setRandomSeed();
    int x, y;
    bool success = false;
    while (not success) {
        x = std::rand() % (mGameBoardWidth - 2) + 1;
        y = std::rand() % (mGameBoardHeight - 2) + 1;

        if(this->mPtrSnake->isBarrier(x,y)){
            success = false;
            continue;
        }

        if ((this->mPtrSnake)->isPartOfSnake(x, y) || x == mItem.getX() && y == mItem.getY()) {
            success = false;
            continue;
        }

        SnakeBody food(x, y);
        this->mPtrSnake->senseFood(food);
        this->mFood = food;
        success = true;
    }
}

void Game::renderFood() const
{
    wattrset(mWindows[1], COLOR_PAIR(1));
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wattroff(mWindows[1], COLOR_PAIR(1));
    wrefresh(this->mWindows[1]);
}

// ----------------------------------------

// Terrain System
void Game::createBarrier(int Y, int X) {
    SnakeBody barrier(X, Y);
    mPtrSnake->addBarrier(barrier);
}

void Game::renderBarriers() const
{
    std::vector<SnakeBody> barriers = mPtrSnake->getBarriers();
    for (int i = 0; i < barriers.size(); i++) {
        mvwaddch(this->mWindows[1], barriers[i].getY(), barriers[i].getX(), 'X');
    }
    wrefresh(this->mWindows[1]);
}

// ----------------------------------------


// Track
void Game::findStartPoint(){
    int x, y;
    x = rand() % (mGameBoardWidth - 2) + 1;
    y = rand() % (mGameBoardHeight - 2) + 1;
    while(true) {
        if((x + 7) < (mGameBoardWidth - 1) &&
           (y + 7) < (mGameBoardHeight - 1)
                ){
            this->track_X = x;
            this->track_Y = y;
            return;
        } else {
            while((x + 7) >= (mGameBoardWidth - 1)){
                x --;
            }
            while((y + 7) >= (mGameBoardHeight - 1)){
                y --;
            }
        }
    }
}

void Game::createRandomTrack(){
    if(this->aPoints >= 6 && this->aPoints < 16){
        createHeartTrack();
    }
    else {
        int whichTrack = rand() % 5;

         switch (whichTrack) {
            case 0: case 1: createSixTrack(); break;
            case 2: createLTrack(); break;
            case 3: createTriangleTrack(); break;
            case 4: createSnakeTrack();break;
        }
    }

    int rotate_time = rand() % 4;
    rotateTrack(rotate_time);
    this->mPtrAwardSnake->mTrackCopy = this->mPtrAwardSnake->mTrack;

}

void Game::createSixTrack(){

    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 2, 2));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 3, 2));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 4, 1));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 4, 1));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 4, 0));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 3, 0));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 2, 0));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 1, 0));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y, 3));
    // Head
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y, 3));
    //
    this->mPtrAwardSnake->TrackAward = 3;

}

void Game::createLTrack() {

    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 1, 3));
    // Head
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y , 3));
    //

    this->mPtrAwardSnake->TrackAward = 2;

}

void Game::createTriangleTrack() {
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y, 3));
    // Head
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y, 3));
    //
    this->mPtrAwardSnake->TrackAward = 3;
}

void Game::createSnakeTrack() {
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 1 , 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y , 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y , 3));
    // head
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X, track_Y + 1, 3));

    this->mPtrAwardSnake->TrackAward = 5;

}

void Game::createHeartTrack() {
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2 , track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 5, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 6, track_Y + 1, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 6, track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 6, track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 6, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 5, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 5, track_Y + 5, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 5, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 4, track_Y + 6, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 3, track_Y + 6, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 6, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 2, track_Y + 5, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 5, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1, track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 4, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 3, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 2, 3));
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X , track_Y + 1, 3));
    // head
    this->mPtrAwardSnake->mTrack.push_back(SnakeBody(track_X + 1 , track_Y + 1, 3));

    this->mPtrAwardSnake->TrackAward = 10;

}

void Game::rotateTrack(int rotate_time){
    // anticlockwise rotate
    for(int j = 0; j < rotate_time; ++ j) {
        for (int i = 0; i < this->mPtrAwardSnake->mTrack.size(); ++i) {
            this->mPtrAwardSnake->mTrack[i] =
                    SnakeBody((track_X - track_Y + this->mPtrAwardSnake->mTrack[i].getY()),
                              (7 + track_Y + track_X - this->mPtrAwardSnake->mTrack[i].getX()));
        }
    }
}

const char Game::getTrackSymbol(int index) const {
    switch( index % 4){
        case 0: return 'v'; break;
        case 1: return '>'; break;
        case 2: return '^'; break;
        case 3: return '<'; break;
    }
}

void Game::renderTrack() {

    for(int i = 0; i < this->mPtrAwardSnake->mTrack.size(); ++ i){
        if(i!= (this->mPtrAwardSnake->mTrack.size() - 1)) {
            wattrset(mWindows[1], COLOR_PAIR(26));
            mvwaddch(mWindows[1], this->mPtrAwardSnake->mTrack[i].getY(), this->mPtrAwardSnake->mTrack[i].getX(), '@');
            wattroff(mWindows[1], COLOR_PAIR(26));
        } else {
            wattrset(mWindows[1], COLOR_PAIR(39));
            mvwaddch(mWindows[1], this->mPtrAwardSnake->mTrack[i].getY(), this->mPtrAwardSnake->mTrack[i].getX(), 'O');
            wattroff(mWindows[1], COLOR_PAIR(39));
        }
    }

    wrefresh(mWindows[1]);
};

// ----------------------------------------




// Snake

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    if (!isAutoColor){
        wattrset(mWindows[1], COLOR_PAIR(mHeadColor));
        mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
        wattroff(mWindows[1], COLOR_PAIR(mHeadColor));
        wattrset(mWindows[1], COLOR_PAIR(mBodyColor));
        for (int i = 1; i < snakeLength; i++) {
            mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
        }
        wattroff(mWindows[1], COLOR_PAIR(mBodyColor));
    } else {
        wattrset(mWindows[1], COLOR_PAIR(26));
        mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
        for (int i = 1; i < snakeLength; i++) {
            mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
        }
        wattroff(mWindows[1], COLOR_PAIR(26));

    }

    // clear remained chars with blank
    mvwaddch(this->mWindows[1], this->mPtrSnake->getLastTail().getY(), this->mPtrSnake->getLastTail().getX(), this->blank);
    wrefresh(this->mWindows[1]);

}

void Game::renderAwardSnake() const {
    int snakeLength = this->mPtrAwardSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrAwardSnake->getSnake();
    if (!isAutoColor){
        wattrset(mWindows[1], COLOR_PAIR(mHeadColor));
        mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
        wattroff(mWindows[1], COLOR_PAIR(mHeadColor));
        wattrset(mWindows[1], COLOR_PAIR(mBodyColor));
        for (int i = 1; i < snakeLength; i++) {
            mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
        }
        wattroff(mWindows[1], COLOR_PAIR(mBodyColor));
    } else {
        wattrset(mWindows[1], COLOR_PAIR(26));
        mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), this->mSnakeSymbol);
        for (int i = 1; i < snakeLength; i++) {
            mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
        }
        wattroff(mWindows[1], COLOR_PAIR(26));

    }

    // clear remained chars with blank
    mvwaddch(this->mWindows[1], this->mPtrAwardSnake->getLastTail().getY(), this->mPtrAwardSnake->getLastTail().getX(), this->blank);
    wrefresh(this->mWindows[1]);

}

void Game::controlSnake(int key, bool isReversed) const
{
    //    int key;
    //    key = getch();
    if (!isReversed) {
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                // change the direction of the snake.
                this->mPtrSnake->changeDirection(Direction::Up);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                // change the direction of the snake.
                Direction Down;
                this->mPtrSnake->changeDirection(Direction::Down);
                break;
            }
            case 'A':
            case 'a':
            case KEY_LEFT:
            {
                // change the direction of the snake.
                Direction Left;
                this->mPtrSnake->changeDirection(Direction::Left);
                break;
            }

            case 'D':
            case 'd':
            case KEY_RIGHT:
            {
                // change the direction of the snake.
                Direction Right;
                this->mPtrSnake->changeDirection(Direction::Right);
                break;
            }
            default:
            {
                break;
            }
        }
        return;
    }
    else {
        switch (key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                // change the direction of the snake.
                Direction Down;
                this->mPtrSnake->changeDirection(Direction::Down);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                // change the direction of the snake.
                Direction Up;
                this->mPtrSnake->changeDirection(Direction::Up);
                break;
            }
            case 'A':
            case 'a':
            case KEY_LEFT:
            {
                // change the direction of the snake.
                Direction Right;
                this->mPtrSnake->changeDirection(Direction::Right);
                break;
            }

            case 'D':
            case 'd':
            case KEY_RIGHT:
            {
                // change the direction of the snake.
                Direction Left;
                this->mPtrSnake->changeDirection(Direction::Left);
                break;
            }
            default:
            {
                break;
            }
        }
        return;
    }
}


void Game::controlAwardSnake(int key) const {
    switch (key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            // change the direction of the snake.
            this->mPtrAwardSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            // change the direction of the snake.
            Direction Down;
            this->mPtrAwardSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            // change the direction of the snake.
            Direction Left;
            this->mPtrAwardSnake->changeDirection(Direction::Left);
            break;
        }

        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            // change the direction of the snake.
            Direction Right;
            this->mPtrAwardSnake->changeDirection(Direction::Right);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::eraseSnake() const {
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for(auto i:snake){
        mvwaddch(mWindows[1],i.getY(),i.getX(),' ');
    }
    wrefresh(mWindows[1]);
}


// ----------------------------------------


void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }

    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}

void Game::renderStageOrPracticeModeBoard(std::string introduction, int stage, int level, std::string title, int mode) const {
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }
    print_paragraph(mWindows[0], mScreenWidth, 2, introduction);
    if(mode == 0)
        mvwprintw(this->mWindows[0], 1, mScreenWidth / 2 - 5, "STAGE MODE");
    else
        mvwprintw(this->mWindows[0], 1, mScreenWidth / 2 - 5, "PRACTICE MODE");
    mvwprintw(this->mWindows[0], 4, 1, " STAGE");
    switch (stage)
    {
        case 1:
            mvwprintw(this->mWindows[0], 4, 7, "1:");
            break;
        case 2:
            mvwprintw(this->mWindows[0], 4, 7, "2:");
            break;
        case 3:
            mvwprintw(this->mWindows[0], 4, 7, "3:");
            break;
        case 4:
            mvwprintw(this->mWindows[0], 4, 7, "4:");
            break;
        case 5:
            mvwprintw(this->mWindows[0], 4, 7, "5:");
            break;
        case 6:
            mvwprintw(this->mWindows[0], 4, 7, "6:");
            break;
        case 7:
            mvwprintw(this->mWindows[0], 4, 7, "EX:");
            break;
    }
    mvwprintw(this->mWindows[0], 4, 11, title.c_str());

    mvwprintw(this->mWindows[2], 1, 1, " Manual");
    if (stage != 4){
        mvwprintw(this->mWindows[2], 3, 1, " Up: W");
        mvwprintw(this->mWindows[2], 4, 1, " Down: S");
        mvwprintw(this->mWindows[2], 5, 1, " Left: A");
        mvwprintw(this->mWindows[2], 6, 1, " Right: D");
    } else {
        mvwprintw(this->mWindows[2], 3, 1, " Up: S");
        mvwprintw(this->mWindows[2], 4, 1, " Down: W");
        mvwprintw(this->mWindows[2], 5, 1, " Left: D");
        mvwprintw(this->mWindows[2], 6, 1, " Right: A");

    }
    mvwprintw(this->mWindows[2], 8, 1, " Pause: Esc");
    mvwprintw(this->mWindows[2], 10, 1, " Difficulty");
    mvwprintw(this->mWindows[2], 16, 1, " Level");

    switch (level)
    {
        case 0:
            wattrset(mWindows[2], COLOR_PAIR(27));
            mvwprintw(this->mWindows[2], 17, 2, "EASY");
            wattroff(mWindows[2], COLOR_PAIR(27));
            break;
        case 1:
            wattrset(mWindows[2], COLOR_PAIR(28));
            mvwprintw(this->mWindows[2], 17, 2, "NORMAL");
            wattroff(mWindows[2], COLOR_PAIR(28));
            break;
        case 2:
            wattrset(mWindows[2], COLOR_PAIR(29));
            mvwprintw(this->mWindows[2], 17, 2, "HARD");
            wattroff(mWindows[2], COLOR_PAIR(29));
            break;
        case 3:
            wattrset(mWindows[2], COLOR_PAIR(30));
            mvwprintw(this->mWindows[2], 17, 2, "LUNATIC");
            wattroff(mWindows[2], COLOR_PAIR(30));
            break;
        case 4:
            wattrset(mWindows[2], COLOR_PAIR(31));
            mvwprintw(this->mWindows[2], 17, 2, "EXTRA");
            wattroff(mWindows[2], COLOR_PAIR(31));
            break;
    }
    mvwprintw(this->mWindows[2], 13, 1, " Points");


    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }

    WINDOW* start = newwin(6, 30, mInformationHeight + (mScreenHeight - mInformationHeight) / 2 - 3, (mScreenWidth - mInstructionWidth) / 2 - 15);
    box(start, 0, 0);
    mvwprintw(start, 2, 4, "Tapping enter to start");
    wattron(start, COLOR_PAIR(14));
    mvwprintw(start, 4, 14, "OK");
    wattroff(start, COLOR_PAIR(14));
    wrefresh(start);
    int key;
    while (true) {
        key = getch();
        if (key == ' ' || key == 10) // enter or blankspace
        {
            // clear the pause menu if choose continue
            werase(start);
            wrefresh(start);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    delwin(start);
}

// increase difficulty according to the scores
void Game::adjustDelay(int diffculty)
{
    this->mDifficulty = this->mPoints / diffculty - this->difficultyOffset;
    this->mDelay = this->mBaseDelay * pow(0.9, this->mDifficulty); // 后期太快了！
}

void Game::initializeGame(int diffculty, bool needHealthBar)
{
    // allocate memory for a new snake
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    // snake already initialized

    /*
     * initialize the game points as zero
     * create a food at random place
     * make the snake aware of the food
     * other initializations
     */

    this->is_auto = false;
    this->is_award = false; // 测试award模式时直接设置为true；
    this->mPtrSnake->life_num = 3;
    this->mPoints = 0;
    this->aPoints = 0;
    this->roundNum = 0;
    // this->createRandomFood();
    // this->renderFood();
    // this->createRandomItem();
    // this->renderItem();

    this->renderSnake();
    if (needHealthBar)
        this->renderHealthBar();

    this->restartChoice = true;
    this->escape = false;
    this->musicIndex = diffculty + 1;
    switch (diffculty) {
        case 0:
            this->difficultyOffset = 0;
            this->mDifficulty = 0;
            break;
        case 1:
            this->difficultyOffset = 0;
            this->mDifficulty = 0;
            break;
        case 2:
            this->difficultyOffset = -1;
            this->mDifficulty = 1;
            break;
        case 3:
            this->difficultyOffset = -1;
            this->mDifficulty = 1;
            break;
    }
    this->adjustDelay();
    this->renderPoints();
    this->renderDifficulty();
}

// Endless mode
void Game::runGame()
{
    if (isMusicOn)
        playMusic(musicIndex);
    stateAfterMove state;


    this->createRandomFood();
    this->renderFood();

    // count is used to control refresh frequency for item
    bool eatItem = true;
    int count = 0;
    int goal = 300;

    int key = 0;
    while (true)
    {
        this->adjustDelay();

        wrefresh(mWindows[1]);
        key = getch();

        this->controlSnake(key);


        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27) {
            this->restartChoice = renderPauseMenu();
            this->renderFood();
            this->renderItem();
        }

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false)
            break;
        else{
            if (isMusicOn)
                playMusic(musicIndex);
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            this->renderSnake();
            if(this->mPtrSnake->life_num == 0){
                break;
            } else {
                this->cutHealthBar();
                -- this->mPtrSnake->life_num;
                if(this->mPtrSnake->life_num == 0){
                    break;
                }
                this->eraseSnake();

                this->mPoints = this->mPoints - this->mPtrSnake->getSnake().size() / 2;
                this->mDifficulty = this->mPoints / 5 - this->difficultyOffset;
                this->mDelay = this->mBaseDelay * pow(0.9, this->mDifficulty);

                this->erasePoints();
                this->renderPoints();
                this->eraseDifficulty();
                this->renderDifficulty();
                this->mPtrSnake->reInitializeSnake();


                int time = 5;
                while (time > 0){
                    renderSnake();
                    usleep(100000);
                    eraseSnake();
                    usleep(100000);
                    -- time;
                }
                renderSnake();
            }
        }


        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                if (!eatItem)
                    this->effectItem();
                this->mItem.clearItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();

        // every time eat the auto item, move 50000 steps
        if(this->is_auto){
            renderA();
            if (isAcousticsOn){
                stopMusic(musicIndex);
                mciSendString("play auto from 0", NULL, 0, NULL);
            }
            this->autoDrive(60);
            this->adjustDelay();
            mciSendString("stop auto", NULL, 0, NULL);
            if (isMusicOn)
                playMusic(musicIndex);
            clearA();

            this->mPtrSnake->getDirection(this->mPtrSnake->getSnake()[0],this->mPtrSnake->getSnake()[1]);
            continue;
        }
        // after auto mode is over, the item will be refreshed


        if(this->is_award){
            enterAwardTime(500);
            renderBoards();
            renderPoints();
            renderDifficulty();
            is_award = false;
        }


        this->renderSnake();
        this->renderFood();
        this->renderItem();

        if (eatItem) {
            int rate = log10(mPoints);
            int number = rand() % 10;
            if ((number+1) <= rate) {
                eatItem = false;
                createRandomItem();
                this->renderItem();
            }
        }

        // increase difficulty
        this->adjustDelay();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        this->renderHealthBar();
        ++ count;


    }
    if (isMusicOn)
        stopMusic(musicIndex);
    wrefresh(mWindows[1]);
}

// use for auto display only!!!
void Game::autoGame() {
    if (isMusicOn)
        playMusic(musicIndex);
    int key;
    bool eatFood = false;
    while (true) {
        wrefresh(mWindows[1]);
        key = getch();
        if (key == 'Q' || key == 'q' || key == 27)
            this->restartChoice = renderPauseMenu();

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false)
            break;
        else{
            if (isMusicOn)
                playMusic(musicIndex);
        }

        // false
        if (eatFood) {
            while (eatFood) {
                // judge whether pause and conduct corresponding behaviors
                if (key == 'Q' || key == 'q' || key == 27)
                    this->restartChoice = renderPauseMenu();

                if (this->restartChoice == false)
                    break;
                else{
                    if (isMusicOn)
                        playMusic(musicIndex);
                }

                eatFood = mPtrSnake->moveByPath();// 派真蛇去吃食物

                this->renderSnake();

                if (not eatFood) {
                    if (isAcousticsOn)
                        PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                    ++mPoints;
                    this->mPtrSnake->createNewTail();
                    createRandomFood();
                    renderFood();
                }
                usleep(5000);
            }

        }
        else {
            if (mPtrSnake->canFindBfsPath(this->mFood)) {
                Snake virtualSnake(this->mGameBoardWidth, this->mGameBoardHeight, this->mPtrSnake->getLength());

                virtualSnake.mSnake = this->mPtrSnake->copySnake();
                virtualSnake.bfsPath = this->mPtrSnake->bfsPath;

                bool veatFood = true;

                while (veatFood) {
                    veatFood = virtualSnake.moveByPath();
                }

                if (virtualSnake.canFindBfsPathToTail())
                {
                    eatFood = mPtrSnake->moveByPath();
                    this->renderSnake();
                    usleep(5000);

                }
                else if (mPtrSnake->canFindBfsPathToTail()) {

                    mPtrSnake->followSnakeTail();
                    this->renderSnake();
                    usleep(5000);

                }
                else {
                    mPtrSnake->wander();
                    this->renderSnake();
                    usleep(5000);
                }

            }
            else if (mPtrSnake->canFindBfsPathToTail())
            {
                mPtrSnake->followSnakeTail();
                this->renderSnake();
                usleep(5000);

            }
            else {
                mPtrSnake->wander();
                this->renderSnake();
                usleep(5000);
            }


            // check collisions
            if ((this->mPtrSnake)->checkCollision()) {
                if (isAcousticsOn)
                    mciSendString("play dead from 0", NULL, 0, NULL);

                std::cout << "collision!!!" << std::endl;
                this->renderSnake();
                this->renderFood();
                wrefresh(mWindows[1]);
                sleep(5);
                break;
            }
            this->renderFood();

            // increase difficulty
//            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
            this->renderPoints();
            this->renderDifficulty();
            this->adjustDelay();
        }
    }
    if (isMusicOn)
        stopMusic(musicIndex);
}

// use for auto item only!!!
void Game::autoDrive(int time = 1000) {
    isAutoColor = true;
//    renderSnake();
    int key;
    bool eatFood = false;
    while (time > -1) {
        wrefresh(mWindows[1]);
        key = getch();
        if (key == 'Q' || key == 'q' || key == 27)
            this->restartChoice = renderPauseMenu();

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false)
            break;
        else{
            if (isAcousticsOn)
                mciSendString("play auto", NULL, 0, NULL);
            else if (isMusicOn)
                playMusic(musicIndex);
        }

        // false
        if (eatFood) {
            while (eatFood) {
                // judge whether pause and conduct corresponding behaviors
                if (key == 'Q' || key == 'q' || key == 27)
                    this->restartChoice = renderPauseMenu();

                if (this->restartChoice == false)
                    break;
                else{
                    if (isAcousticsOn)
                        mciSendString("play auto", NULL, 0, NULL);
                    else if (isMusicOn)
                        playMusic(musicIndex);
                }

                eatFood = mPtrSnake->moveByPath();// 派真蛇去吃食物

                this->renderSnake();

                if (not eatFood) {
                    if (isAcousticsOn)
                        PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                    ++mPoints;
                    this->mPtrSnake->createNewTail();
                    createRandomFood();
                    renderFood();
                }
                usleep(10000);
            }

        }
        else {
            if (mPtrSnake->canFindBfsPath(this->mFood)) {
                Snake virtualSnake(this->mGameBoardWidth, this->mGameBoardHeight, this->mPtrSnake->getLength());

                virtualSnake.mSnake = this->mPtrSnake->copySnake();
                virtualSnake.bfsPath = this->mPtrSnake->bfsPath;

                bool veatFood = true;

                while (veatFood) {
                    veatFood = virtualSnake.moveByPath();
                }

                if (virtualSnake.canFindBfsPathToTail())
                {
                    eatFood = mPtrSnake->moveByPath();
                    this->renderSnake();
                    usleep(10000);

                }
                else if (mPtrSnake->canFindBfsPathToTail()) {

                    mPtrSnake->followSnakeTail();
                    this->renderSnake();
                    usleep(10000);

                }
                else {
                    mPtrSnake->wander();
                    this->renderSnake();
                    usleep(10000);
                }

            }
            else if (mPtrSnake->canFindBfsPathToTail())
            {
                mPtrSnake->followSnakeTail();
                this->renderSnake();
                usleep(10000);

            }
            else {
                mPtrSnake->wander();
                this->renderSnake();
                usleep(10000);
            }


            // check collisions
            if ((this->mPtrSnake)->checkCollision()) {
                if (isAcousticsOn)
                    mciSendString("play dead from 0", NULL, 0, NULL);
                this->renderSnake();
                this->renderFood();
                wrefresh(mWindows[1]);
                sleep(1);
                break;
            }

            this->renderFood();

            // increase difficulty
//            std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
            this->renderPoints();
            this->renderDifficulty();
            this->adjustDelay();
        }

        time --;

    }
    isAutoColor = false;
    this->is_auto = false;
}

// use for award item only!!!
void Game::enterAwardTime(int time) {
    // TODO: 展示传送门动画

    // 清除原有窗口
    for (int i = 0; i < this->mWindows.size(); i++) {
        werase(this->mWindows[i]);
        wrefresh(mWindows[i]);
    }

    for (int i = 0; i < mWindows.size(); ++i) {
        box(mWindows[i], 0, 0);
        wrefresh(mWindows[i]);
    }

    // 打印信息
    mvwprintw(this->mWindows[0], 1, mScreenWidth / 2 - 5, "Award Time");
    mvwprintw(this->mWindows[0], 2, 2,
              "Follow the Track. you can get extra points, and these points won't increase the difficulty!");

    mvwprintw(this->mWindows[0], 3, 2, "Only if you start from box:");
    wattrset(mWindows[0], COLOR_PAIR(39));
    mvwaddch(this->mWindows[0], 3, 30, '@' );
    wattroff(mWindows[0], COLOR_PAIR(39));
    mvwprintw(this->mWindows[0], 3, 32, "can you start follow the track!");
    wrefresh(mWindows[0]);

    // 初始化一条新蛇
    this->mPtrAwardSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));

    // 进入award time, 速度变慢！
    int key = 0;
    bool musicing = false;

    bool atStart = false;
    bool finishTrack = false;

    // Initialize a Track
    findStartPoint(); // 初始化地图锚点
    createRandomTrack(); // 随机初始化一条道路
    this->prev_len = (this->mPtrAwardSnake->mTrack).size();

    renderTrack(); // 展示道路


    while (time > 0) {

        wrefresh(mWindows[1]);
        key = getch();

        this->controlAwardSnake(key);

        wrefresh(mWindows[1]);

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            this->restartChoice = renderPauseMenu();

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false)
            break;
        else{
            if (isMusicOn)
                playMusic(musicIndex);
        }

        // through Wall
        if (this->mPtrAwardSnake->hitWall()) {
            this->mPtrAwardSnake->ThroughWall();
        }
        else {
            this->mPtrAwardSnake->moveForward();
        }

        this->renderAwardSnake();


        // start

//        mvwprintw(this->mWindows[0], 4, 2, "size:%d, Track:(%d, %d), Head(%d, %d)",this->mPtrAwardSnake->mTrack.size(),this->mPtrAwardSnake->mTrack[this->mPtrAwardSnake->mTrack.size() - 1].getX(),this->mPtrAwardSnake->mTrack[this->mPtrAwardSnake->mTrack.size() - 1].getY(),this->mPtrAwardSnake->getSnake()[0].getX(),this->mPtrAwardSnake->getSnake()[0].getY());

        wrefresh(mWindows[0]);

        prev_len = this->mPtrAwardSnake->mTrack.size();

        this->renderTrack();
        if(this->mPtrAwardSnake->getSnake()[0] == this->mPtrAwardSnake->mTrack[this->mPtrAwardSnake->mTrack.size() - 1]){
            if(this->mPtrAwardSnake->TrackAward == 10){  // Heart is hard!!! So give more delay
                this->mDelay = 400;
            } else {
                this->mDelay = 200;
            }
            atStart = true;
            this->mPtrAwardSnake->mTrack.pop_back();
            if (isMusicOn && !musicing){
                mciSendString("play track from 0", NULL, 0, NULL);
                musicing = true;
            }
        }


        wrefresh(mWindows[0]);

//         中途退出
        if(prev_len == this->mPtrAwardSnake->mTrack.size()){
            atStart = false;
            finishTrack = false;
            renderTrack();
            mciSendString("stop track", NULL, 0, NULL);
            musicing = false;
        }


        if(this->mPtrAwardSnake->mTrack.empty()){
            finishTrack = true;
            atStart = false;
            mciSendString("stop track", NULL, 0, NULL);
            musicing = false;
        }

        if(finishTrack) {
            // complete heart track, life ++
            if(this-> mPtrAwardSnake->TrackAward == 10){
                this->mPtrSnake->life_num ++;
            }

            this->aPoints += this-> mPtrAwardSnake->TrackAward;

            findStartPoint(); // 初始化地图锚点
            createRandomTrack(); // 随机初始化一条道路
            this->prev_len = (this->mPtrAwardSnake->mTrack).size();
            renderTrack(); // 展示道路
            this->mDelay = 40;
        } else {
            if(not atStart) {
                this->mPtrAwardSnake->mTrack = this->mPtrAwardSnake->mTrackCopy;
                renderTrack(); // 展示道路
                this->mDelay = 100;
            }
        }



        this->renderAwardSnake();

        this->renderAwardPoints();


        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        mvwprintw(this->mWindows[0], 4, 2, "Your time remained: %d ", time / 10);
        mvwprintw(this->mWindows[0], 4, 25, "s");

        wrefresh(mWindows[1]);

        -- time;
    }
    this->mDelay = this->mBaseDelay * pow(0.9, this->mDifficulty);
    this->aPoints = 0;
    mciSendString("stop track", NULL, 0, NULL);
    if (isAcousticsOn)
        PlaySound("./music/portal.wav", NULL, SND_FILENAME|SND_ASYNC);
    // TODO 展示回归动画
}

void Game::startGame()
{
    refresh();
   renderStartComics();
    playMusic(musicIndex);
    mainMenuState startChoice = this->renderStartMenu();
    gameMenuState gameChoice = ENDLESS;
    practiceMenuState practiceChoice = STA1;
    levelMenuState levelChoice = NORMAL;
    bool practiceAuxiliarySuccessVariable = true;

    while (startChoice != mainMenuState::QUIT) {
        switch (startChoice) {
            case mainMenuState::START:
                gameChoice = renderGameMenu();
                while (gameChoice != gameMenuState::RETURNMAIN) {
                    switch (gameChoice) {
                        case gameMenuState::AI:
                            stopMusic(musicIndex);
                            while (true) {
                                this->readLeaderBoard();
                                this->renderBoards();
                                this->initializeGame();
                                this->musicIndex = 4;
                                this->autoGame();
                                this->updateLeaderBoard();
                                this->writeLeaderBoard();
                                if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                    break;
                                }
                                else if (not this->escape && not this->restartChoice) {          // behavior: restart game from pause menu
                                    mPoints = 0;
                                    aPoints = 0;
                                    continue;
                                }
                                else if (not this->escape && this->restartChoice) {
                                    // game fails and restart game
                                    this->restartChoice = this->renderRestartMenu();
                                    if (this->restartChoice == false) {
                                        break;
                                    }
                                }
                            }
                            break;
                        case gameMenuState::PRACTICE:
                        returnToPracticeMenu:practiceChoice = renderPracticeMenu();
                            while (practiceChoice != practiceMenuState::RETURNPRACTICE) {
                                switch (practiceChoice) {
                                    case practiceMenuState::STA1:
                                        levelChoice = renderLevelMenu();
                                        while(levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:
                                                    if (!readStageClearRecord(0, 1)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 10 points.", 1, 0, "The first acquaintance with the snake.",1);
                                                    this->initializeGame(0);
                                                    this->stage1(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:

                                                    if (!readStageClearRecord(1, 1)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 15 points.", 1, 1, "The first acquaintance with the snake.",1);
                                                    this->initializeGame(1);
                                                    this->stage1(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:

                                                    if (!readStageClearRecord(2, 1)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 1, 2, "The first acquaintance with the snake.",1);
                                                    this->initializeGame(2);
                                                    this->stage1(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 1)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 1, 3, "The first acquaintance with the snake.",1);
                                                    this->initializeGame(3);
                                                    this->stage1(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
//                                            if (practiceAuxiliarySuccessVariable) {
//                                                renderCongratulationMenu();
//                                                break;
//                                            }
//                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
//                                                break;
//                                            }
//                                            else if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
//                                                mPoints = 0;
//                                                aPoints = 0;
//                                                continue;
//                                            }
                                           if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }

                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STA2:
                                        levelChoice = renderLevelMenu();
                                        while (levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:
                                                    if (!readStageClearRecord(0, 2)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 12 points.", 2, 0, "Why not try to break through the wall?", 1);
                                                    this->initializeGame(0);
                                                    this->stage2(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:
                                                    if (!readStageClearRecord(1, 2)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 17 points.", 2, 1, "Why not try to break through the wall?", 1);
                                                    this->initializeGame(1);
                                                    this->stage2(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:
                                                    if (!readStageClearRecord(2, 2)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 22 points.", 2, 2, "Why not try to break through the wall?", 1);
                                                    this->initializeGame(2);
                                                    this->stage2(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 2)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 27 points.", 2, 3, "Why not try to break through the wall?", 1);
                                                    this->initializeGame(3);
                                                    this->stage2(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }

                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STA3:
                                        levelChoice = renderLevelMenu();
                                        while (levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:

                                                    if (!readStageClearRecord(0, 3)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                     this->renderStageOrPracticeModeBoard("Goal:Get 12 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 0, "\"UROBOROS\"", 1);
                                                    this->initializeGame(0);
                                                    this->stage3(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:

                                                    if (!readStageClearRecord(1, 3)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                     this->renderStageOrPracticeModeBoard("Goal:Get 17 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 1, "\"UROBOROS\"", 1);
                                                    this->initializeGame(1);
                                                    this->stage3(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:

                                                    if (!readStageClearRecord(2, 3)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:Get 22 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 2, "\"UROBOROS\"", 1);
                                                    this->initializeGame(2);
                                                    this->stage3(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 3)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                     this->renderStageOrPracticeModeBoard("Goal:Get 27 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 3, "\"UROBOROS\"", 1);
                                                    this->initializeGame(3);
                                                    this->stage3(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }


                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STA4:
                                        levelChoice = renderLevelMenu();
                                        while (levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:

                                                    if (!readStageClearRecord(0, 4)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 15 points.", 4, 0, "Crowded Blocks.", 1);
                                                    this->initializeGame(0);
                                                    this->stage4(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:

                                                    if (!readStageClearRecord(1, 4)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 4, 1, "Crowded Blocks.", 1);
                                                    this->initializeGame(1);
                                                    this->stage4(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:

                                                    if (!readStageClearRecord(2, 4)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 4, 2, "Crowded Blocks.", 1);
                                                    this->initializeGame(2);
                                                    this->stage4(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 4)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 30 points.", 4, 3, "Crowded Blocks.", 1);
                                                    this->initializeGame(3);
                                                    this->stage4(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }

                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STA5:
                                        levelChoice = renderLevelMenu();
                                        while (levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:

                                                    if (!readStageClearRecord(0, 5)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 5, 0, "\"Upside Down\"", 1);
                                                    this->initializeGame(0);
                                                    this->stage5(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:

                                                    if (!readStageClearRecord(1, 5)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 5, 1, "\"Upside Down\"", 1);
                                                    this->initializeGame(1);
                                                    this->stage5(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:

                                                    if (!readStageClearRecord(2, 5)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 30 points.", 5, 2, "\"Upside Down\"", 1);
                                                    this->initializeGame(2);
                                                    this->stage5(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 5)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 35 points.", 5, 3, "\"Upside Down\"", 1);
                                                    this->initializeGame(3);
                                                    this->stage5(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }


                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STA6:
                                        levelChoice = renderLevelMenu();
                                        while (levelChoice != levelMenuState::RETURNLEVEL) {
                                            practiceAuxiliarySuccessVariable = true;
                                            switch (levelChoice)
                                            {
                                                case EASY:

                                                    if (!readStageClearRecord(0, 6)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 18 points.", 6, 0, "Anniversary Celebration", 1);
                                                    this->initializeGame(0);
                                                    this->stage6(0, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case NORMAL:

                                                    if (!readStageClearRecord(1, 6)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 96 points.", 6, 1, "Anniversary Celebration", 1);
                                                    this->initializeGame(1);
                                                    this->stage6(1, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case HARD:

                                                    if (!readStageClearRecord(2, 6)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 120 points.", 6, 2, "Anniversary Celebration", 1);
                                                    this->initializeGame(2);
                                                    this->stage6(2, &practiceAuxiliarySuccessVariable);
                                                    break;
                                                case LUNATIC:

                                                    if (!readStageClearRecord(3, 6)) {
                                                        renderPracticeOpenFailedMenu();
                                                        goto returnToPracticeMenu;
                                                        break;
                                                    }
                                                    stopMusic(musicIndex);
                                                    this->renderStageOrPracticeModeBoard("Goal:get 126 points.", 6, 3, "Anniversary Celebration", 1);
                                                    this->initializeGame(3);
                                                    this->stage6(3, &practiceAuxiliarySuccessVariable);
                                                    break;
                                            }
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }

                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }

                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    case practiceMenuState::STAEX:
                                        if (!readStageClearRecord(4, 7)) {
                                            renderPracticeOpenFailedMenu(true);
                                            goto returnToPracticeMenu;
                                            break;
                                        }
                                        stopMusic(musicIndex);
                                        while (true) {
                                            practiceAuxiliarySuccessVariable = true;
                                            this->renderStageOrPracticeModeBoard("Goal:get 30 points.", 7, 17, "Over Speed!!", 1);
                                            this->initializeGame(0);
                                            musicIndex = 6;
                                            stageEX(&practiceAuxiliarySuccessVariable);
                                            if (not this->escape && not this->restartChoice) {                 // behavior: restart game from pause menu
                                                mPoints = 0;
                                                aPoints = 0;
                                                continue;
                                            }
                                            else if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                                break;
                                            }
                                            else if (practiceAuxiliarySuccessVariable) {
                                                renderCongratulationMenu();
                                                break;
                                            }


                                            else if (not this->escape && this->restartChoice) {
                                                // game fails and restart game
                                                this->restartChoice = this->renderRestartMenu();
                                                if (this->restartChoice == false) {
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                }
                                musicIndex = 5;
                                if (isMusicOn)
                                    playMusic(musicIndex);
                                practiceChoice = renderPracticeMenu();
                            }
                            break;
                        case gameMenuState::ENDLESS:
                            stopMusic(musicIndex);
                            while (true) {
                                this->readLeaderBoard();
                                this->renderBoards();
                                this->initializeGame(0, true);
                                this->runGame();
                                this->updateLeaderBoard();
                                this->writeLeaderBoard();
                                if (this->escape && not this->restartChoice) {                       // behavior: quit game from pause menu
                                    break;
                                }
                                else if (not this->escape && not this->restartChoice) {          // behavior: restart game from pause menu
                                    mPoints = 0;
                                    aPoints = 0;
                                    continue;
                                }
                                else if (not this->escape && this->restartChoice) {
                                    // game fails and restart game
                                    this->restartChoice = this->renderRestartMenu();
                                    if (this->restartChoice == false) {
                                        break;
                                    }
                                }
                            }
                            break;
                        case gameMenuState::STAGE:
                            levelChoice = renderLevelMenu();
                            while (levelChoice != levelMenuState::RETURNLEVEL) {
                                switch (levelChoice)
                                {
                                    case EASY:
                                        stopMusic(musicIndex);
                                        startStageMode(0);
                                        break;
                                    case NORMAL:
                                        stopMusic(musicIndex);
                                        startStageMode(1);
                                        break;
                                    case HARD:
                                        stopMusic(musicIndex);
                                        startStageMode(2);
                                        break;
                                    case LUNATIC:
                                        stopMusic(musicIndex);
                                        startStageMode(3);
                                        break;
                                    case RETURNLEVEL:
                                        break;
                                    default:
                                        break;
                                }
                                musicIndex = 5;
                                if (isMusicOn)
                                    playMusic(musicIndex);
                                levelChoice = renderLevelMenu();
                            }
                            break;
                        default:
                            break;
                    }
                    musicIndex = 5;
                    if (isMusicOn)
                        playMusic(musicIndex);
                    gameChoice = renderGameMenu();
                }
                break;
            case mainMenuState::SKIN:
                renderSkin();
                break;
            case mainMenuState::ITEMS:
                renderItemInfo();
                break;
            case mainMenuState::MUSIC:
                MusicOnOff(musicIndex);
                break;
            case mainMenuState::ACOUSTICS:
                AcousticsOnOff();
            default: break;
        }
        musicIndex = 5;
        if (isMusicOn)
            playMusic(musicIndex);
        startChoice = this->renderStartMenu();
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

bool Game::readStageClearRecord(int diffculty, int stage)
{
    std::fstream record("stageRecord.dat", record.binary |record.in);
    if (!record.is_open())
    {
        record.close();
        std::fstream record("stageRecord.dat", record.binary | record.trunc | record.out);
        bool auxBoolVar = false;
        for (int i = 0; i < 25; i++)
            record.write((char*)&auxBoolVar, sizeof(auxBoolVar));
        record.close();
        record.open("stageRecord.dat", record.binary | record.in);
    }
    //定位该关卡通关记录位置
    bool currentState;
    int location = 0;
    switch (stage) {
        case 1:
            break;
        case 2:
            location += 1;
            break;
        case 3:
            location += 1;
            break;
        case 4:
            location += 1;
            break;
        case 5:
            location += 1;
            break;
        case 6:
            location += 1;
            break;
        default:
            location = 0;
            break;
    }

    switch (diffculty) {
        case 0:
            location++;
            break;
        case 1:
            location += 7;
            break;
        case 2:
            location += 13;
            break;
        case 3:
            location += 19;
            break;
        default:
            location = 0;
    }
    for(int i = 0;i<=location;i++)
        record.read((char*)(&currentState), sizeof(currentState));
    record.close();
    return currentState;
}

bool Game::writeStageOpenRecord(int diffculty, int stage, bool state)
{
    std::fstream record("stageRecord.dat", record.binary | record.in | record.out);
    if (!record.is_open())
    {
        record.close();
        std::fstream record("stageRecord.dat", record.binary | record.trunc | record.out);
        bool auxBoolVar = false;
        for (int i = 0; i < 25; i++)
            record.write((char*)&auxBoolVar, sizeof(auxBoolVar));
        record.close();
        record.open("stageRecord.dat", record.binary | record.in | record.out);
    }
    bool currentState;
    int location = 0;
    switch (stage) {
        case 1:
            break;
        case 2:
            location += 1;
            break;
        case 3:
            location += 1;
            break;
        case 4:
            location += 1;
            break;
        case 5:
            location += 1;
            break;
        case 6:
            location += 1;
            break;
        default:
            location = 0;
            break;
    }

    switch (diffculty) {
        case 0:
            location++;
            break;
        case 1:
            location += 7;
            break;
        case 2:
            location += 13;
            break;
        case 3:
            location += 19;
            break;
        default:
            location = 0;
    }
    for (int i = 0; i < location; i++)
        record.read((char*)(&currentState), sizeof(currentState));
    record.write((char*)&state, sizeof(state));
    record.close();
    return true;
}




//the implementation of stages
void Game::stage1(int diffculty,bool* success, bool isStageMode)
{
    if (isMusicOn)
        playMusic(musicIndex);

    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;


    //set terrain here
    bool LR = true;
    for (int i = 1; i < mGameBoardHeight / 2; i += 6) {
        printSquare((mGameBoardHeight - 2) / 2, (mGameBoardWidth - 2) / 2, mGameBoardWidth, mGameBoardHeight, i, (mGameBoardWidth - 2) / 2);
        deleteSquare((mGameBoardHeight - 2) / 2, (mGameBoardWidth - 2) / 2, mGameBoardWidth, mGameBoardHeight, i, (mGameBoardWidth - 2) / 2, LR);
        LR = (LR) ? false : true;
    }

    renderBarriers();

    createRandomFood();

    renderFood();

    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 10;
            break;
        case 1:
            goal = 15;
            break;
        case 2:
            goal = 20;
            break;
        case 3:
            goal = 25;
            break;
    }
    int key = 0;
    while (mPoints<goal)
    {
        wrefresh(mWindows[1]);
        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if(!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false) {
            stopMusic(musicIndex);
            *success = false;
            return;
        }
        else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }


        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            stopMusic(musicIndex);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(6);
                break;
            case 3:
                this->adjustDelay();
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;

    wrefresh(mWindows[1]);
}

void Game::stage2(int diffculty, bool* success, bool isStageMode)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set terrain here
    int pointerY = mGameBoardHeight - 2, pointerX = 1;
    bool isBackslash = false;
    while (pointerX <= mGameBoardWidth - 2) {
        if (!isBackslash) {
            printSlash(&pointerY, &pointerX);
            renderBarriers();
            wrefresh(mWindows[1]);
            isBackslash = true;
        }
        else {
            printSlash(&pointerY, &pointerX, true);
            isBackslash = false;
            wrefresh(mWindows[1]);
        }
    }

    renderBarriers();

    createRandomFood();

    renderFood();

    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 12;
            break;
        case 1:
            goal = 17;
            break;
        case 2:
            goal = 22;
            break;
        case 3:
            goal = 27;
            break;
    }
    int key = 0;
    while (mPoints < goal)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if (!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false) {
            stopMusic(musicIndex);
            *success = false;
            break;
        }else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            stopMusic(musicIndex);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(6);
                break;
            case 3:
                this->adjustDelay();
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}

void Game::stage3(int diffculty, bool* success, bool isStageMode)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set terrain here
    for (int i = mGameBoardHeight / 4 + 1; i < mGameBoardHeight * 3 / 4; i++) {
        createBarrier(i, mGameBoardWidth / 4);
        createBarrier(i, mGameBoardWidth * 3 / 4);
    }
    for (int i = mGameBoardWidth / 4; i <= mGameBoardWidth * 3 / 8; i++) {
        createBarrier(mGameBoardHeight / 4, i);
        createBarrier(mGameBoardHeight * 3 / 4, i);
    }
    for (int i = mGameBoardWidth * 5 / 8; i <= mGameBoardWidth * 3 / 4; i++) {
        createBarrier(mGameBoardHeight / 4, i);
        createBarrier(mGameBoardHeight * 3 / 4, i);
    }
    printSlash(mGameBoardHeight * 3 / 4, mGameBoardWidth / 2 - 6, false, mGameBoardHeight / 4, mGameBoardWidth * 5 / 8 - 1);
    printSlash(mGameBoardHeight / 4, mGameBoardWidth / 2 - 6, true, mGameBoardHeight * 3 / 4, mGameBoardWidth * 5 / 8 - 1);
    mPtrSnake->breakBarrier(mGameBoardWidth / 2, mGameBoardHeight / 2);
    mPtrSnake->breakBarrier(mGameBoardWidth / 2, mGameBoardHeight / 2);
    renderBarriers();

    createRandomFood();

    renderFood();


    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 12;
            break;
        case 1:
            goal = 17;
            break;
        case 2:
            goal = 22;
            break;
        case 3:
            goal = 27;
            break;
    }
    int key = 0;
    while (mPoints < 126)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if (!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false){
            stopMusic(musicIndex);
            *success = false;
            break;
        }else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision(true)) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            stopMusic(musicIndex);
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(6);
                break;
            case 3:
                this->adjustDelay();
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}

void Game::stage4(int diffculty, bool* success, bool isStageMode)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set terrain here
    for (int i = 1; i < (mGameBoardHeight * 2 / 3); i++) {
        createBarrier(i, mGameBoardWidth / 6);
    }
    for (int i = 1; i < (mGameBoardHeight / 6); i++) {
        createBarrier(i, mGameBoardWidth * 2 / 6);
    }
    for (int i = (mGameBoardHeight * 5 / 6); i < (mGameBoardHeight - 1); i++) {
        createBarrier(i, mGameBoardWidth * 2 / 3);
    }
    for (int i = (mGameBoardHeight / 3 + 1); i < (mGameBoardHeight - 1); i++) {
        createBarrier(i, mGameBoardWidth * 5 / 6);
    }
    for (int i = 1; i <= (mGameBoardWidth / 6); i++) {
        createBarrier(mGameBoardHeight * 2 / 3, i);
    }
    for (int i = 1; i <= (mGameBoardWidth * 2 / 3); i++) {
        createBarrier(mGameBoardHeight * 5 / 6, i);
    }
    for (int i = (mGameBoardWidth / 3); i < (mGameBoardWidth - 1); i++) {
        createBarrier(mGameBoardHeight / 6, i);
    }
    for (int i = (mGameBoardWidth * 5 / 6); i < (mGameBoardWidth - 1); i++) {
        createBarrier(mGameBoardHeight / 3, i);
    }

    renderBarriers();

    createRandomFood();
    renderFood();

    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 15;
            break;
        case 1:
            goal = 20;
            break;
        case 2:
            goal = 25;
            break;
        case 3:
            goal = 30;
            break;
    }
    int key = 0;
    while (mPoints < goal)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if (!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false){
            stopMusic(musicIndex);
            *success = false;
            break;
        }else{
            if (isMusicOn)
                playMusic(musicIndex);
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            stopMusic(musicIndex);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(6);
                break;
            case 3:
                this->adjustDelay();
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}

void Game::stage5(int diffculty, bool* success, bool isStageMode)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set terrain here
    bool LR = true;
    for (int i = 1; i < mGameBoardHeight / 2; i += 6) {
        printSquare((mGameBoardHeight - 2) / 2, (mGameBoardWidth - 2) / 2, mGameBoardWidth, mGameBoardHeight, i, (mGameBoardWidth - 2) / 2);
        deleteSquare((mGameBoardHeight - 2) / 2, (mGameBoardWidth - 2) / 2, mGameBoardWidth, mGameBoardHeight, i, (mGameBoardWidth - 2) / 2, LR);
        LR = (LR) ? false : true;
    }
    renderBarriers();

    createRandomFood();

    renderFood();

    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 20;
            break;
        case 1:
            goal = 25;
            break;
        case 2:
            goal = 30;
            break;
        case 3:
            goal = 35;
            break;
    }
    int key = 0;
    while (mPoints < goal)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key, true);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if (!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false){
            stopMusic(musicIndex);
            *success = false;
            break;
        }else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            stopMusic(musicIndex);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(6);
                break;
            case 3:
                this->adjustDelay();
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}

void Game::stage6(int diffculty, bool* success, bool isStageMode)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set terrain here
    //create "S"
    for (int i = (mGameBoardWidth / 17); i <= (mGameBoardWidth * 4 / 17); i++) {
        createBarrier(mGameBoardHeight / 7, i);
    }
    for (int i = (mGameBoardHeight / 7 + 1); i < (mGameBoardHeight * 3 / 7); i++) {
        createBarrier(i, mGameBoardWidth / 17);
    }
    for (int i = (mGameBoardWidth / 17); i <= (mGameBoardWidth * 4 / 17); i++) {
        createBarrier(mGameBoardHeight * 3 / 7, i);
    }
    for (int i = (mGameBoardHeight * 3 / 7 + 1); i < (mGameBoardHeight * 5 / 7); i++) {
        createBarrier(i, mGameBoardWidth * 4 / 17);
    }
    for (int i = (mGameBoardWidth / 17); (i <= mGameBoardWidth * 4 / 17); i++) {
        createBarrier(mGameBoardHeight * 5 / 7, i);
    }
    //create"J"
    for (int i = (mGameBoardHeight / 7); i < (mGameBoardHeight * 5 / 7); i++) {
        createBarrier(i, (mGameBoardWidth * 13 / 34));
    }
    for (int i = (mGameBoardWidth * 5 / 17); i <= (mGameBoardWidth * 13 / 17 / 2); i++) {
        createBarrier(mGameBoardHeight * 5 / 7, i);
    }
    //create"T"
    for (int i = (mGameBoardWidth * 9 / 17); i <= (mGameBoardWidth * 12 / 17); i++) {
        createBarrier(mGameBoardHeight / 7, i);
    }
    for (int i = (mGameBoardHeight / 7 + 1); i <= (mGameBoardHeight * 5 / 7); i++) {
        createBarrier(i, (mGameBoardWidth * 21 / 17 / 2));
    }
    //create"U"
    for (int i = (mGameBoardHeight / 7); i < (mGameBoardHeight * 5 / 7); i++) {
        createBarrier(i, (mGameBoardWidth * 13 / 17));
    }
    for (int i = (mGameBoardWidth * 13 / 17); (i <= mGameBoardWidth * 16 / 17); i++) {
        createBarrier(mGameBoardHeight * 5 / 7, i);
    }
    for (int i = (mGameBoardHeight / 7); i < (mGameBoardHeight * 5 / 7); i++) {
        createBarrier(i, (mGameBoardWidth * 16 / 17));
    }
    renderBarriers();

    createRandomFood();

    renderFood();

    //set goal
    int goal;
    switch (diffculty) {
        case 0:
            goal = 18;
            break;
        case 1:
            goal = 96;
            break;
        case 2:
            goal = 120;
            break;
        case 3:
            goal = 126;
            break;
    }
    int key = 0;
    while (mPoints < goal)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            if (!isStageMode)
                this->restartChoice = renderPauseMenu();
            else
                this->restartChoice = renderPauseMenu(true);

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false){
            stopMusic(musicIndex);
            *success = false;
            break;
        }else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            stopMusic(musicIndex);
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
        switch (diffculty) {
            case 0:
                this->adjustDelay(10);
                break;
            case 1:
                this->adjustDelay(7);
                break;
            case 2:
                this->adjustDelay(15);
                break;
            case 3:
                this->adjustDelay(20);
                break;
        }

    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}

void Game::stageEX(bool* success)
{
    stateAfterMove state;
    // control refresh frequency for item
    bool eatItem = false;
    int count = 0;

    //set diffculty here
    difficultyOffset = -6;
    mDifficulty = 6;

    //set terrain here
    int beginX = mGameBoardWidth - 6, beginY = 5;
    bool isBackSlash = false;
    while (beginX >= mGameBoardWidth / 2 + 2) {
        if (!isBackSlash) {
            while (beginY < mGameBoardHeight - 1) {
                printSlash(&beginY, &beginX, isBackSlash, beginY - 4, beginX + 4);
                beginX -= 5, beginY += 10;
                renderBarriers();
                wrefresh(mWindows[1]);
            }
            isBackSlash = true;
            beginY = 1;
        }
        else {
            while (beginY < mGameBoardHeight - 5) {
                printSlash(&beginY, &beginX, isBackSlash, beginY + 4, beginX + 4);
                beginX -= 5;
                renderBarriers();
                wrefresh(mWindows[1]);
            }
            isBackSlash = false;
            beginY = 5;
        }
        beginX -=13;
    }
    beginX = mGameBoardWidth / 2 - 6, beginY = 5, isBackSlash = false;
    while (beginX >= 1) {
        if (!isBackSlash) {
            while (beginY < mGameBoardHeight - 1) {
                printSlash(&beginY, &beginX, isBackSlash, beginY - 4, beginX + 4);
                beginX -= 5, beginY += 10;
                renderBarriers();
                wrefresh(mWindows[1]);
            }
            isBackSlash = true;
            beginY = 1;
        }
        else {
            while (beginY < mGameBoardHeight - 5) {
                printSlash(&beginY, &beginX, isBackSlash, beginY + 4, beginX + 4);
                beginX -= 5;
                renderBarriers();
                wrefresh(mWindows[1]);
            }
            isBackSlash = false;
            beginY = 5;
        }
        beginX -= 13;
    }
    renderBarriers();

    createRandomFood();

    renderFood();

    this->adjustDelay(3);

    int key = 0;
    while (mPoints < 30)
    {


        wrefresh(mWindows[1]);


        key = getch();
        this->controlSnake(key);
        count++;

        // judge whether pause and conduct corresponding behaviors
        if (key == 'Q' || key == 'q' || key == 27)
            this->restartChoice = renderPauseMenu();

        // this means player choose "Restart" or "Escape" and quit the loop
        if (this->restartChoice == false){
            stopMusic(musicIndex);
            break;
        } else{
            if (isMusicOn)
                playMusic(musicIndex);
            renderBarriers();
        }

        // check collisions
        if ((this->mPtrSnake)->checkCollision()) {
            stopMusic(musicIndex);
            if (isAcousticsOn)
                mciSendString("play dead from 0", NULL, 0, NULL);
            this->renderSnake();
            this->escape = false;
            this->restartChoice = true;
            *success = false;
            return;
        }

        // through Wall
        if (this->mPtrSnake->hitWall()) {
            state = this->mPtrSnake->ThroughWall();
        }
        else {
            state = this->mPtrSnake->moveForward();
        }

        switch (state) {
            case NONE: {
                break;
            }
            case FOOD: {
                if (isAcousticsOn)
                    PlaySound("./music/food.wav", NULL, SND_FILENAME|SND_ASYNC);
                ++(this->mPoints); // increase scores
                this->createRandomFood();
                this->mPtrSnake->createNewTail();
                break;
            }
            case ITEM: {
                this->effectItem();
                eatItem = true;
                break;
            }
            default:break;
        }

        this->renderSnake();
        this->renderFood();

        // if (count > 20 && eatItem) {
        //     int rate = log(mPoints);
        //     int number = rand() % 10;
        //     if ((number + 1) <= rate) {
        //         count = 0;
        //         eatItem = false;
        //         createRandomItem();
        //         this->renderItem();
        //     }
        // }


        // increase difficulty
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));
        this->renderPoints();
        this->renderDifficulty();
    }
    stopMusic(musicIndex);
    *success = true;
    wrefresh(mWindows[1]);
}



void Game::startStageMode(int diffculty) {

    beginingOfStage:
    bool success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:get 10 points.", 1, 0, "The first acquaintance with the snake.", 0);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:get 15 points.", 1, 1, "The first acquaintance with the snake.", 0);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 1, 2, "The first acquaintance with the snake.", 0);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 1, 3, "The first acquaintance with the snake.", 0);
            break;
    }

    this->initializeGame(diffculty);
    //只有在stage模式中遇到该关卡的时候才开启该关卡,
    writeStageOpenRecord(diffculty, 1);
    this->stage1(diffculty,&success,true);
    stopMusic(musicIndex);

    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) { //胜利后播报胜利画面（后面都一样）
        renderCongratulationMenu();
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)  // restart game
            goto beginingOfStage;
        else if (this->restartChoice == false) {  // return Menu
            return;
        }
    }


    //第二关
    success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:get 12 points.", 2, 0, "Why not try to break through the wall?", 0);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:get 17 points.", 2, 1, "Why not try to break through the wall?", 0);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:get 22 points.", 2, 2, "Why not try to break through the wall?", 0);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:get 12 points.", 2, 3, "Why not try to break through the wall?", 0);
            break;
    }
    this->initializeGame(diffculty);
    writeStageOpenRecord(diffculty, 2);
    this->stage2(diffculty, &success, true);
    stopMusic(musicIndex);

    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) {
        renderCongratulationMenu();
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)
            goto beginingOfStage;
        else if (this->restartChoice == false) {
            return;
        }
    }

    //第三关
    success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:Get 12 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 0, "\"UROBOROS\"", 1);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:Get 17 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 1, "\"UROBOROS\"", 1);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:Get 22 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 2, "\"UROBOROS\"", 1);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:Get 27 point with the distance between the head and tail less than 5 pixels.(So never try to break through the wall!)", 3, 3, "\"UROBOROS\"", 1);
            break;

    }
    this->initializeGame(diffculty);
    writeStageOpenRecord(diffculty, 3);
    this->stage3(diffculty, &success, true);

    stopMusic(musicIndex);

    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) {
        renderCongratulationMenu();
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)
            goto beginingOfStage;
        else if (this->restartChoice == false) {
            return;
        }
    }

    //第四关
    success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:get 15 points.", 4, 0, "Crowded Blocks.", 0);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 4, 1, "Crowded Blocks.", 0);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 4, 2, "Crowded Blocks.", 0);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:get 30 points.", 4, 3, "Crowded Blocks.", 0);
            break;
    }
    this->initializeGame(diffculty);
    writeStageOpenRecord(diffculty, 4);
    this->stage4(diffculty, &success, true);
    stopMusic(musicIndex);

    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) {
        renderCongratulationMenu();
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)
            goto beginingOfStage;
        else if (this->restartChoice == false) {
            return;
        }
    }


    //第五关
    success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:get 20 points.", 5, 0, "\"Upside Down\"", 0);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:get 25 points.", 5, 1, "\"Upside Down\"", 0);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:get 30 points.", 5, 2, "\"Upside Down\"", 0);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:get 35 points.", 5, 3, "\"Upside Down\"", 0);
            break;
    }
    this->initializeGame(diffculty);
    writeStageOpenRecord(diffculty, 5);
    this->stage5(diffculty, &success, true);

    stopMusic(musicIndex);

    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) {
        renderCongratulationMenu();
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)
            goto beginingOfStage;
        else if (this->restartChoice == false) {
            return;
        }
    }


    //第六关
    success = false;
    switch (diffculty) {
        case 0:
            this->renderStageOrPracticeModeBoard("Goal:get 18 points.", 6, 0, "Anniversary Celebration", 0);
            break;
        case 1:
            this->renderStageOrPracticeModeBoard("Goal:get 96 points.", 6, 1, "Anniversary Celebration", 0);
            break;
        case 2:
            this->renderStageOrPracticeModeBoard("Goal:get 120 points.", 6, 2, "Anniversary Celebration", 0);
            break;
        case 3:
            this->renderStageOrPracticeModeBoard("Goal:get 126 points.", 6, 3, "Anniversary Celebration", 0);
            break;
    }
    this->initializeGame(diffculty);
    //                    mvwaddch(this->mWindows[1],0,0,'s');
    //                    mvwaddch(this->mWindows[1],mGameBoardHeight - 1,mGameBoardWidth -1 ,'s');
    //                    wrefresh(this->mWindows[1]);
    //                    sleep(10);

    this->stage6(diffculty, &success, true);
    writeStageOpenRecord(diffculty, 6);
    stopMusic(musicIndex);
    // 退到上一级菜单 escape
    if(this->escape && not this->restartChoice){
        return;
    }
    else if (not this->escape && not this->restartChoice) { // restart
        goto beginingOfStage;
    } else if (success) {
        renderCongratulationMenu();
        //在此判断是否开启ex关卡
        if (diffculty != 0)
            writeStageOpenRecord(4, 7);
    } else if (not this->escape && this->restartChoice) {              // game fails and restart game
        this->restartChoice = this->renderRestartMenu();
        if (this->restartChoice == true)
            goto beginingOfStage;
        else if (this->restartChoice == false) {
            return;
        }
    }

    //在此处加入通关动画
    renderEndComics();

    return;
}

void Game::playMusic(int index) const{
    switch (index){
        case 1:
            mciSendString("play EasyMusic repeat", NULL, 0, NULL);
            break;
        case 2:
            mciSendString("play NormalMusic repeat", NULL, 0, NULL);
            break;
        case 3:
            mciSendString("play HardMusic repeat", NULL, 0, NULL);
            break;
        case 4:
            mciSendString("play LunaticMusic repeat", NULL, 0, NULL);
            break;
        case 5:
            mciSendString("play Menu repeat", NULL, 0, NULL);
            break;
        case 6:
            mciSendString("play auto repeat", NULL, 0, NULL);
        default:
            break;
    }
}

void Game::stopMusic(int index) const{
    switch (index){
        case 1:
            mciSendString("stop EasyMusic", NULL, 0,NULL);
            break;
        case 2:
            mciSendString("stop NormalMusic", NULL, 0, NULL);
            break;
        case 3:
            mciSendString("stop HardMusic", NULL, 0, NULL);
            break;
        case 4:
            mciSendString("stop LunaticMusic", NULL, 0, NULL);
            break;
        case 5:
            mciSendString("stop Menu", NULL, 0, NULL);
            break;
        case 6:
            mciSendString("stop auto", NULL, 0, NULL);
        default:
            break;
    }
}

void Game::MusicOnOff(int index){
    if (isMusicOn){
        isMusicOn = false;
        stopMusic(index);
    } else {
        isMusicOn = true;
        playMusic(index);
    }
}

void Game::AcousticsOnOff(){
    isAcousticsOn = !isAcousticsOn;
}

void Game::renderStartComics() const{
    int delay = 20;
    WINDOW* start;
    start = newwin(mScreenHeight, mScreenWidth, 0, 0);
    wattron(start, COLOR_PAIR(1));
    mvwprintw(start, mScreenHeight/2+5, mScreenWidth/2 - 22, ".");
    wattroff(start, COLOR_PAIR(1));
    wattron(start, COLOR_PAIR(2));
    mvwprintw(start, mScreenHeight/2 +5, 0, ".");
    wrefresh(start);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay*5));
    mvwprintw(start, mScreenHeight/2 +5, 1, ".");
    wattroff(start, COLOR_PAIR(2));
    wrefresh(start);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay*5));
    for (int i = 0; i < mScreenWidth/2 - 24; i++){
        mvwprintw(start, mScreenHeight/2 +5, i, " ");
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 + 5, i + 2, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay*5));
        wrefresh(start);
    }
    for (int i = 0; i < 8; i++){
        wattron(start, COLOR_PAIR(1));
        mvwprintw(start, mScreenHeight/2 +5, mScreenWidth/2-22+i+1, ".");
        wattroff(start, COLOR_PAIR(1));
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 + 5, mScreenWidth/2 -22+i, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        wrefresh(start);
    }

    for (int i = 1; i < 5; i++){
        wattron(start, COLOR_PAIR(1));
        mvwprintw(start, mScreenHeight/2 +5-i, mScreenWidth/2-14, ".");
        wattroff(start, COLOR_PAIR(1));
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 + 5-i+1, mScreenWidth/2 -14, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        wrefresh(start);
    }

    for (int i = 0; i < 10; i++){
        wattron(start, COLOR_PAIR(1));
        mvwprintw(start, mScreenHeight/2 + 1, mScreenWidth/2-15 - i, ".");
        wattroff(start, COLOR_PAIR(1));
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 + 1, mScreenWidth/2 - 15 - i + 1, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        wrefresh(start);
    }

    for (int i = 1; i < 5; i++){
        wattron(start, COLOR_PAIR(1));
        mvwprintw(start, mScreenHeight/2 + 1 - i, mScreenWidth/2-24, ".");
        wattroff(start, COLOR_PAIR(1));
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 + 1 - i + 1, mScreenWidth/2 -24, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        wrefresh(start);
    }

    for (int i = 0; i < 10; i++){
        wattron(start, COLOR_PAIR(1));
        mvwprintw(start, mScreenHeight/2 - 3, mScreenWidth/2 - 24 + i + 1, ".");
        wattroff(start, COLOR_PAIR(1));
        wattron(start, COLOR_PAIR(2));
        mvwprintw(start, mScreenHeight/2 - 3, mScreenWidth/2 - 24 + i, ".");
        wattroff(start, COLOR_PAIR(2));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        wrefresh(start);
    }
    wattron(start, COLOR_PAIR(2));
    mvwprintw(start, mScreenHeight/2 - 3, mScreenWidth/2 - 14, ".");
    wrefresh(start);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    wattroff(start, COLOR_PAIR(2));

    for (int j = 0; j < 5; j++){
        wattron(start, COLOR_PAIR(19+j));
        for (int i = 0; i < 5; i++){
            mvwprintw(start, mScreenHeight/2+1 + i, mScreenWidth/2 -11, ".");
            mvwprintw(start, mScreenHeight/2+1 + i, mScreenWidth/2 -6, ".");
            mvwprintw(start, mScreenHeight/2+1 + i, mScreenWidth/2 + 2, ".");
            mvwprintw(start, mScreenHeight/2+1 + i, mScreenWidth/2 + 13 , ".");
        }
        for (int i = 0; i < 7; i++)
            mvwprintw(start, mScreenHeight/2 - 1 + i, mScreenWidth/ 2+6, ".");
        for (int i = 0; i < 3; i++)
            mvwprintw(start, mScreenHeight/2+2+i, mScreenWidth/2-3,".");
        for (int i = 0; i < 4; i++){
            mvwprintw(start, mScreenHeight/2+1, mScreenWidth/2 - 10 + i, ".");
            mvwprintw(start, mScreenHeight/2+1, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(start, mScreenHeight/2+5, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(start, mScreenHeight/2+1, mScreenWidth/2 +14 + i, ".");
            mvwprintw(start, mScreenHeight/2+3, mScreenWidth/2 +14 + i, ".");
            mvwprintw(start, mScreenHeight/2+5, mScreenWidth/2 +14 + i, ".");
        }
        mvwprintw(start, mScreenHeight/2+4, mScreenWidth/2+3, ".");
        mvwprintw(start, mScreenHeight/2+3, mScreenWidth/2+7, ".");
        for (int i = 0; i < 2; i++){
            mvwprintw(start, mScreenHeight/2+2, mScreenWidth/2+8+i,".");
            mvwprintw(start, mScreenHeight/2+4, mScreenWidth/2+8+i,".");
        }
        mvwprintw(start, mScreenHeight/2+1, mScreenWidth/2+10, ".");
        mvwprintw(start, mScreenHeight/2+5, mScreenWidth/2+10, ".");
        mvwprintw(start, mScreenHeight/2+2, mScreenWidth/2+17, ".");
        wattroff(start, COLOR_PAIR(19+j));
        wrefresh(start);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay*30));
    }


    delwin(start);
}

void Game::renderEndComics() const{
    if (isMusicOn)
        mciSendString("play end repeat", NULL, 0, NULL);
    WINDOW* ending;
    ending = newwin(mScreenHeight, mScreenWidth, 0, 0);
    for (int j = 0; j < 5; j++){
        wattron(ending, COLOR_PAIR(32+j));
        mvwprintw(ending, mScreenHeight/2 - 10, mScreenWidth/2 - 5, "SnakeGame");
        mvwprintw(ending, mScreenHeight/2 - 9, mScreenWidth/2 - 20, "Item design");
        mvwprintw(ending, mScreenHeight/2 - 9, mScreenWidth/2 + 10, "by hbh");
        mvwprintw(ending, mScreenHeight/2 - 8, mScreenWidth/2 - 20, "Wall Interaction");
        mvwprintw(ending, mScreenHeight/2 - 8, mScreenWidth/2 + 10, "by hbh");
        mvwprintw(ending, mScreenHeight/2 - 7, mScreenWidth/2 - 20, "Auto Mode");
        mvwprintw(ending, mScreenHeight/2 - 7, mScreenWidth/2 +10, "by hbh");

        mvwprintw(ending, mScreenHeight/2 - 6 , mScreenWidth/2-20, "Stage Mode");
        mvwprintw(ending, mScreenHeight/2 - 6 , mScreenWidth/2+11, "by lz");
        mvwprintw(ending, mScreenHeight/2 - 5 , mScreenWidth/2-20, "Practice Mode");
        mvwprintw(ending, mScreenHeight/2 - 5 , mScreenWidth/2+11, "by lz");
        mvwprintw(ending, mScreenHeight/2 - 4 , mScreenWidth/2-20, "Terrain design");
        mvwprintw(ending, mScreenHeight/2 - 4 , mScreenWidth/2+11, "by lz");

        mvwprintw(ending, mScreenHeight/2 - 3, mScreenWidth/2 - 20, "Music system");
        mvwprintw(ending, mScreenHeight/2 - 3 , mScreenWidth/2+10, "by hyx");
        mvwprintw(ending, mScreenHeight/2 - 2, mScreenWidth/2 - 20, "UI design");
        mvwprintw(ending, mScreenHeight/2 - 2 , mScreenWidth/2+10, "by hyx");
        mvwprintw(ending, mScreenHeight/2 - 1, mScreenWidth/2 - 20, "Comics design");
        mvwprintw(ending, mScreenHeight/2 - 1 , mScreenWidth/2+10, "by hyx");


        wattroff(ending, COLOR_PAIR(32+j));
        wrefresh(ending);
        std::this_thread::sleep_for(std::chrono::milliseconds(350));
    }
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (int j = 4; j >=0 ; j--){
        wattron(ending, COLOR_PAIR(32+j));
        mvwprintw(ending, mScreenHeight/2 - 10, mScreenWidth/2 - 5, "SnakeGame");
        mvwprintw(ending, mScreenHeight/2 - 9, mScreenWidth/2 - 20, "Item design");
        mvwprintw(ending, mScreenHeight/2 - 9, mScreenWidth/2 + 10, "by hbh");
        mvwprintw(ending, mScreenHeight/2 - 8, mScreenWidth/2 - 20, "Wall Interaction");
        mvwprintw(ending, mScreenHeight/2 - 8, mScreenWidth/2 + 10, "by hbh");
        mvwprintw(ending, mScreenHeight/2 - 7, mScreenWidth/2 - 20, "Auto Mode");
        mvwprintw(ending, mScreenHeight/2 - 7, mScreenWidth/2 +10, "by hbh");

        mvwprintw(ending, mScreenHeight/2 - 6 , mScreenWidth/2-20, "Stage Mode");
        mvwprintw(ending, mScreenHeight/2 - 6 , mScreenWidth/2+11, "by lz");
        mvwprintw(ending, mScreenHeight/2 - 5 , mScreenWidth/2-20, "Practice Mode");
        mvwprintw(ending, mScreenHeight/2 - 5 , mScreenWidth/2+11, "by lz");
        mvwprintw(ending, mScreenHeight/2 - 4 , mScreenWidth/2-20, "Terrain design");
        mvwprintw(ending, mScreenHeight/2 - 4 , mScreenWidth/2+11, "by lz");

        mvwprintw(ending, mScreenHeight/2 - 3, mScreenWidth/2 - 20, "Music system");
        mvwprintw(ending, mScreenHeight/2 - 3 , mScreenWidth/2+10, "by hyx");
        mvwprintw(ending, mScreenHeight/2 - 2, mScreenWidth/2 - 20, "UI design");
        mvwprintw(ending, mScreenHeight/2 - 2 , mScreenWidth/2+10, "by hyx");
        mvwprintw(ending, mScreenHeight/2 - 1, mScreenWidth/2 - 20, "Comics design");
        mvwprintw(ending, mScreenHeight/2 - 1 , mScreenWidth/2+10, "by hyx");


        wattroff(ending, COLOR_PAIR(32+j));
        wrefresh(ending);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    werase(ending);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (int j = 0; j < 5; j++){
        wattron(ending, COLOR_PAIR(32+j));
        mvwprintw(ending, mScreenHeight/2 - 4, mScreenWidth/2-13, "Thanks for your playing!");
        wrefresh(ending);
        wattroff(ending, COLOR_PAIR(32+j));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    for (int j = 4; j >= 0; j--){
        wattron(ending, COLOR_PAIR(32+j));
        mvwprintw(ending, mScreenHeight/2 - 4, mScreenWidth/2-13, "Thanks for your playing!");
        wrefresh(ending);
        wattroff(ending, COLOR_PAIR(32+j));
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    werase(ending);
    wrefresh(ending);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    for (int j = 0; j < 5; j++){
        wattron(ending, COLOR_PAIR(19+j));
        for (int i = 0; i < 10; i++){
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2-24+i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2-24+i, ".");
            mvwprintw(ending, mScreenHeight/2-3, mScreenWidth/2-24+i, ".");
        }
        for (int i = 0; i < 3; i++){
            mvwprintw(ending, mScreenHeight/2+2+i, mScreenWidth/2-15, ".");
            mvwprintw(ending, mScreenHeight/2-2+i, mScreenWidth/2-24, ".");
        }
        for (int i = 0; i < 5; i++){
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 -11, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 -6, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 + 2, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 + 13 , ".");
        }
        for (int i = 0; i < 7; i++)
            mvwprintw(ending, mScreenHeight/2 - 1 + i, mScreenWidth/ 2+6, ".");
        for (int i = 0; i < 3; i++)
            mvwprintw(ending, mScreenHeight/2+2+i, mScreenWidth/2-3,".");
        for (int i = 0; i < 4; i++){
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 - 10 + i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 +14 + i, ".");
            mvwprintw(ending, mScreenHeight/2+3, mScreenWidth/2 +14 + i, ".");
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2 +14 + i, ".");
        }
        mvwprintw(ending, mScreenHeight/2+4, mScreenWidth/2+3, ".");
        mvwprintw(ending, mScreenHeight/2+3, mScreenWidth/2+7, ".");
        for (int i = 0; i < 2; i++){
            mvwprintw(ending, mScreenHeight/2+2, mScreenWidth/2+8+i,".");
            mvwprintw(ending, mScreenHeight/2+4, mScreenWidth/2+8+i,".");
        }
        mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2+10, ".");
        mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2+10, ".");
        mvwprintw(ending, mScreenHeight/2+2, mScreenWidth/2+17, ".");
        wattroff(ending, COLOR_PAIR(19+j));
        wrefresh(ending);
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }


    for (int j = 4; j >= 0; j--){
        wattron(ending, COLOR_PAIR(19+j));
        for (int i = 0; i < 10; i++){
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2-24+i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2-24+i, ".");
            mvwprintw(ending, mScreenHeight/2-3, mScreenWidth/2-24+i, ".");
        }
        for (int i = 0; i < 3; i++){
            mvwprintw(ending, mScreenHeight/2+2+i, mScreenWidth/2-15, ".");
            mvwprintw(ending, mScreenHeight/2-2+i, mScreenWidth/2-24, ".");
        }
        for (int i = 0; i < 5; i++){
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 -11, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 -6, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 + 2, ".");
            mvwprintw(ending, mScreenHeight/2+1 + i, mScreenWidth/2 + 13 , ".");
        }
        for (int i = 0; i < 7; i++)
            mvwprintw(ending, mScreenHeight/2 - 1 + i, mScreenWidth/ 2+6, ".");
        for (int i = 0; i < 3; i++)
            mvwprintw(ending, mScreenHeight/2+2+i, mScreenWidth/2-3,".");
        for (int i = 0; i < 4; i++){
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 - 10 + i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2 - 2 + i, ".");
            mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2 +14 + i, ".");
            mvwprintw(ending, mScreenHeight/2+3, mScreenWidth/2 +14 + i, ".");
            mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2 +14 + i, ".");
        }
        mvwprintw(ending, mScreenHeight/2+4, mScreenWidth/2+3, ".");
        mvwprintw(ending, mScreenHeight/2+3, mScreenWidth/2+7, ".");
        for (int i = 0; i < 2; i++){
            mvwprintw(ending, mScreenHeight/2+2, mScreenWidth/2+8+i,".");
            mvwprintw(ending, mScreenHeight/2+4, mScreenWidth/2+8+i,".");
        }
        mvwprintw(ending, mScreenHeight/2+1, mScreenWidth/2+10, ".");
        mvwprintw(ending, mScreenHeight/2+5, mScreenWidth/2+10, ".");
        mvwprintw(ending, mScreenHeight/2+2, mScreenWidth/2+17, ".");
        wattroff(ending, COLOR_PAIR(19+j));
        wrefresh(ending);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    werase(ending);
    wrefresh(ending);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));


    mciSendString("stop end", NULL, 0, NULL);
    delwin(ending);
}


void Game::printSlash(int beginingY, int beginingX, bool isBackslash, int endingY, int endingX) {
    if (endingX == -1 or endingY == -1) {
        if (!isBackslash) {
            while ((beginingX < mGameBoardWidth - 1) and (beginingY > 0)) {
                createBarrier(beginingY, beginingX);
                beginingX++, beginingY--;
            }
            return;
        }
        else {
            while ((beginingX < mGameBoardWidth - 1) and (beginingY < mGameBoardHeight - 1)) {
                createBarrier(beginingY, beginingX);
                beginingX++, beginingY++;
            }
            return;
        }
    }
    else {
        if (!isBackslash) {
            while ((beginingX <= endingX) and (beginingY >= endingY)) {
                createBarrier(beginingY, beginingX);
                beginingX++, beginingY--;
            }
            return;
        }
        else {
            while ((beginingX <= endingX) and (beginingY <= endingY)) {
                createBarrier(beginingY, beginingX);
                beginingX++, beginingY++;
            }
            return;
        }
    }
}

void Game::printSlash(int* beginingY, int* beginingX, bool isBackslash, int endingY, int endingX) {
    int beginX = *beginingX, beginY = *beginingY;
    if (endingX == -1 or endingY == -1) {
        if (!isBackslash) {
            while ((beginX < mGameBoardWidth - 1) and (beginY > 1)) {
                createBarrier(beginY, beginX);
                beginX++, beginY--;
            }
            *beginingX = beginX, * beginingY = beginY;
            return;
        }
        else {
            while ((beginX < mGameBoardWidth - 1) and (beginY < mGameBoardHeight - 2)) {
                createBarrier(beginY, beginX);
                beginX++, beginY++;
            }
            *beginingX = beginX, * beginingY = beginY;
            return;
        }
    }
    else {
        if (!isBackslash) {
            while ((beginX <= endingX) and (beginY >= endingY)) {
                createBarrier(beginY, beginX);
                beginX++, beginY--;
            }
            *beginingX = beginX, * beginingY = beginY;
            return;
        }
        else {
            while ((beginX <= endingX) and (beginY <= endingY)) {
                createBarrier(beginY, beginX);
                beginX++, beginY++;
            }
            *beginingX = beginX, * beginingY = beginY;
            return;
        }
    }
}
void Game::printSquare(int middlePointY, int middlePointX, int width, int height, int anchorPointY, int anchorPointX) {
    int realHeight = (middlePointY - anchorPointY) * 2, realWidth = realHeight * width / height;
    for (int i = 0; i < realWidth; i++) {
        createBarrier(anchorPointY, anchorPointX - realWidth / 2 + i);
        createBarrier(anchorPointY + realHeight - 1, anchorPointX - realWidth / 2 + i);
    }
    for (int i = 0; i < realHeight - 1; i++) {
        createBarrier(anchorPointY + 1 + i, anchorPointX - realWidth / 2);
        createBarrier(anchorPointY + 1 + i, anchorPointX - realWidth / 2 + realWidth - 1);
    }
}
void Game::deleteSquare(int middlePointY, int middlePointX, int width, int height, int anchorPointY, int anchorPointX,int LR) {
    int realHeight = (middlePointY - anchorPointY) * 2, realWidth = realHeight * width / height;
    if (LR) {
        mPtrSnake->breakBarrier(anchorPointX - realWidth / 2, middlePointY);
        mPtrSnake->breakBarrier(anchorPointX - realWidth / 2 + realWidth - 1, middlePointY);
    }
    else {
        mPtrSnake->breakBarrier(middlePointX, anchorPointY);
        mPtrSnake->breakBarrier(middlePointX, anchorPointY + realHeight - 1);
    }
}





//非game类函数
static void print_menu(WINDOW* menu, std::vector<std::string> options, int offset, int index) {
    for (int i = 0; i < options.size(); i++) {
        if (i == index) {
            wattron(menu, COLOR_PAIR(14));
            mvwprintw(menu, i + offset, 4, options[i].c_str());
            wattroff(menu, COLOR_PAIR(14));
        }
        else {
            mvwprintw(menu, i + offset, 4, options[i].c_str());
        }
    }
    wrefresh(menu);
}

static void print_menu_with_ins(WINDOW* menu, std::vector<std::string> options, int offset, int index, WINDOW* instruction_window, std::vector<std::string> paragraph, int ins_width, int ins_offset) {
    for (int i = 0; i < options.size(); i++) {
        if (i == index) {
            wattron(menu, COLOR_PAIR(14));
            mvwprintw(menu, i + offset, 4, options[i].c_str());
            wattroff(menu, COLOR_PAIR(14));
        }
        else {
            mvwprintw(menu, i + offset, 4, options[i].c_str());
        }
    }
    print_paragraph(instruction_window, ins_width, ins_offset, paragraph[index]);
    wrefresh(instruction_window);
    wrefresh(menu);
}

static void print_paragraph(WINDOW* menu, int menu_width, int offset, std::string paragraph) {
    werase(menu);
    box(menu, 0, 0);
    std::stringstream ss(paragraph);
    std::string cur_word;
    std::string space = " ";
    int count = 2, line = 0;
    ss >> cur_word;
    count += cur_word.size();
    mvwprintw(menu, offset + line, 3, cur_word.c_str());
    while (ss >> cur_word) {
        count += cur_word.size() + 1;
        if (count > menu_width - 3) {
            count = cur_word.size() + 2;
            line++;
        }
        else
            mvwprintw(menu, offset + line, count - cur_word.size(), space.c_str());
        mvwprintw(menu, offset + line, count - cur_word.size() + 1, cur_word.c_str());
    }
}



