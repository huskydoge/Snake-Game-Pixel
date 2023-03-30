
#include <cstdlib>
#include <ctime>
#include "queue"
#include "snake.h"

SnakeBody::SnakeBody()
{
}

SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

SnakeBody::SnakeBody(int x, int y, int trackIndex): mX(x), mY(y), trackIndex(trackIndex)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    // overload the == operator for SnakeBody comparison.
    if(this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY()){
        return true;
    }
    return false;
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

void Snake::reInitializeSnake() {

    this->getSnake().clear();
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;
    for (int i = 0; i < 2; i ++)
    {
        this->getSnake().push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
    // check if a given point with axis x, y is on the body of the snake.
    SnakeBody sn(x, y);
    for(int k = 0; k < (this-> getSnake()).size(); k ++){
        if(getSnake()[k] == sn ){
            return true;
        }
    }
    return false;
}

bool Snake::isBarrier(int x, int y){
    SnakeBody barrier(x, y);
    for(int k = 0; k < (this-> getBarriers()).size(); k ++){
        if(getBarriers()[k] == barrier ){
            return true;
        }
    }
    return false;
}

void Snake::breakBarrier(int x, int y){

    SnakeBody barrier(x, y);
    for(std::vector<SnakeBody>::iterator itor = barriers.begin(); itor != barriers.end(); ++ itor){
        if(*itor == barrier){
            barriers.erase(itor);
            break;
        }
    }
}

bool Snake::hitWall()
{
    // check if the snake has hit the wall
    // PS: the box occupies room!
    if((mDirection == (Direction::Right) && (this->getSnake())[0].getX() == mGameBoardWidth - 2)
       || (mDirection == (Direction::Left) && (this->getSnake())[0].getX() == 1)
       || (mDirection == (Direction::Up) && (this->getSnake())[0].getY() == 1)
       || (mDirection == (Direction::Down) && (this->getSnake())[0].getY() == mGameBoardHeight - 2))
    {
        return true;
    } else {
        return false;
    }
}

bool Snake::checkBound(int x, int y){
    if(x < 1 || x >mGameBoardWidth - 2 || y < 1 || y > mGameBoardHeight - 2){
        return true;
    } else {
        return false;
    }
}

bool Snake::hitSelf()
{
    // check if the snake has hit itself.
    SnakeBody head = getSnake()[0];
    for(int i = 1; i < getLength(); i ++){
        if(head == getSnake()[i]){
            return true;
        }
    }
    return false;
}

bool Snake::hitBarrier() {
    SnakeBody head = getSnake()[0];
    for (int i = 0; i < this->barriers.size(); i++) {
        if (barriers[i] == head) {
            return true;
        }
    }

    return false;
}

bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

void Snake::senseItem(SnakeBody item){
    this->mItem = item;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

std::vector<SnakeBody> Snake::copySnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection) {
    switch (this->mDirection) {
        case Direction::Up: {
            // what you need to do when the current direction of the snake is Up
            // and the user inputs a new direction?

            if (newDirection == Direction::Left || newDirection == Direction::Right) {
                this->mDirection = newDirection;
            }
            break;

        }
        case Direction::Down: {
            // what you need to do when the current direction of the snake is Down
            // and the user inputs a new direction?

            if (newDirection == Direction::Left || newDirection == Direction::Right) {
                this->mDirection = newDirection;
            }
            break;

        }
        case Direction::Left: {
            // what you need to do when the current direction of the snake is Left
            // and the user inputs a new direction?
            if (newDirection == Direction::Up || newDirection == Direction::Down) {
                this->mDirection = newDirection;
            }
            break;

        }
        case Direction::Right: {
            // what you need to do when the current direction of the snake is Right
            // and the user inputs a new direction?

            if (newDirection == Direction::Up || newDirection == Direction::Down) {
                this->mDirection = newDirection;

            }
            break;
        }

    }
    return false;
}

SnakeBody Snake::createNewHead()
{
    /*
     * read the position of the current head
     * read the current heading direction
     * add the new head according to the direction
     * return the new snake
     */

    Direction curDir = this->mDirection;

    int x, y = 0;
    x = this->getSnake()[0].getX();
    y = this->getSnake()[0].getY();

    switch(curDir) {
        case Direction::Up: -- y; break;
        case Direction::Down: ++ y;break;
        case Direction::Left: -- x;break;
        case Direction::Right: ++ x; break;
    }
    getSnake().insert(getSnake().begin(),SnakeBody(x, y));
    SnakeBody newHead(x, y);
    return newHead;
}

SnakeBody Snake::createNewTail(){

    int x, y = 0;
    x = this->lastTail.getX();
    y = this->lastTail.getY();
    (this->mSnake).push_back(this->lastTail);
    SnakeBody newTail(x, y);
    return newTail;
}

SnakeBody Snake::getLastTail(){
    return this->lastTail;
}

void Snake::cutTail() {
    this->mSnake.pop_back();
}

/*
 * If eat food, return true, otherwise return false
 */
stateAfterMove Snake::moveForward() {
    /*
     * move the snake forward.
     * If eat food, return true, otherwise return false
     */

    this->lastTail = getSnake()[getLength()- 1];
    for (int i = getLength() - 1; i > 0; i--) {

        getSnake()[i] = SnakeBody(getSnake()[i - 1].getX(), getSnake()[i - 1].getY());

    }

    int x, y = 0;
    x = this->getSnake()[0].getX();
    y = this->getSnake()[0].getY();

    // It's positive down the Y-axis
    switch (this->mDirection) {
        case Direction::Up:
            --y;
            break;
        case Direction::Down:
            ++y;
            break;
        case Direction::Left:
            --x;
            break;
        case Direction::Right:
            ++x;
            break;
    }

    getSnake()[0] = SnakeBody(x, y);

    if (this->mFood == this->getSnake()[0]) {
        return stateAfterMove::FOOD;
    } else if(this->mItem == this->getSnake()[0]){
        return stateAfterMove::ITEM;
    } else {
        return stateAfterMove::NONE;
    }
}

stateAfterMove Snake::ThroughWall(){
    this->lastTail = getSnake()[getLength()- 1];
    for (int i = getLength() - 1; i > 0; i--) {

        getSnake()[i] = SnakeBody(getSnake()[i - 1].getX(), getSnake()[i - 1].getY());

    }

    int x, y = 0;
    x = this->getSnake()[0].getX();
    y = this->getSnake()[0].getY();;

    // It's positive down the Y-axis
    switch (this->mDirection) {
        case Direction::Up:
            y = y + (mGameBoardHeight - 3);
            break;
        case Direction::Down:
            y = y - (mGameBoardHeight - 3);
            break;
        case Direction::Left:
            x = x + (mGameBoardWidth - 3);
            break;
        case Direction::Right:
            x = x - (mGameBoardWidth - 3);
            break;
    }

    getSnake()[0] = SnakeBody(x, y);

    if (this->mFood == this->getSnake()[0]) {
        return stateAfterMove::FOOD;
    } else if(this->mItem == this->getSnake()[0]){
        return stateAfterMove::ITEM;
    } else {
        return stateAfterMove::NONE;
    }
}

bool Snake::checkCollision(bool checkDistance)
{
    if (checkDistance) {
        if ((abs(mSnake[0].getX() - mSnake[mSnake.size() - 1].getX()) > 5) or (abs(mSnake[0].getY() - mSnake[mSnake.size() - 1].getY()) > 5))
            return true;
    }
    if ((this->hitSelf()) or (this->hitBarrier()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Snake::getLength()
{
    return this->mSnake.size();
}


// items
Item::Item(){
}

Item::Item(itemType type, int x, int y):type(type){
    mX = x;
    mY = y;
}

void Item::clearItem() {
    this->mX = -1;
    this->mY = -1;
    return;
}

// let the snake be aware of which direction it is going in auto mode
void Snake::getDirection(SnakeBody head, SnakeBody body){
    int x = head.getX() - body.getX();
    int y = head.getY() - body.getY();
    if(x == 1){
        mDirection = Direction::Right;
    } else if(x == -1){
        mDirection = Direction::Left;
    } else if(y == 1) {
        mDirection = Direction::Down;
    } else {
        mDirection = Direction::Up;
    }
}

itemType Item::getType() const{
    return this->type;
}

const char Item::getSymbol() const{
    switch (type) {
        case itemType::Freeze:{
            return 'F';
        }
        case itemType::Shrink:{
            return 'S';
        }
        case itemType::Auto:{
            return 'L';
        }
        case itemType::Award:{
            return 'T';
        }
    }
}

int Item::getX() const {
    return mX;
}

int Item::getY() const {
    return mY;
}


// AI Function
bool Snake::canFindBfsPath(SnakeBody end){

    // clear the bfsPath
    while(!bfsPath.empty()) {
        bfsPath.clear();
    }

    // initialize 2D map
    int bfsMap[mGameBoardWidth][mGameBoardHeight];// 0 means reachable, -1 means not

    for(int i = 0;i < mGameBoardWidth;++ i)
        for(int j = 0; j < mGameBoardHeight ;++ j)
            if(i == 0 || i == mGameBoardWidth - 1|| j == 0 || j == mGameBoardHeight - 1){
                bfsMap[i][j] = -1;
            } else {
                bfsMap[i][j] = 0;
            }

    // terrain should be considered here!
    for(auto i:barriers){
        bfsMap[i.getX()][i.getY()] = -1;
    }

    // snake's body should be excluded!
    for(auto i:getSnake())
        bfsMap[i.getX()][i.getY()]=-1;


    // direction: Right, Down, Left, Up
    int dx[4] = { 1,0,-1,0 };
    int dy[4] = { 0,1,0,-1 };

    //array to store the previous map
    bfsNode bfsNodes[mGameBoardWidth][mGameBoardHeight];

    std::queue<SnakeBody>  que;

    SnakeBody Start((getSnake()[0].getX()),(getSnake()[0].getY()));

    que.push(Start);

    while (!que.empty()) {
        SnakeBody cur = que.front();
        que.pop();

        // f destination reached, store the path (without start)
        if ((cur.getX() == end.getX()) && (cur.getY() == end.getY())) {

            bfsPath.push_back(end);
            int x = end.getX(), y = end.getY();
            int tmp_x,tmp_y;
            SnakeBody tmp;
            while (!(x==Start.getX() && y == Start.getY())) {
                tmp_x = x;
                tmp_y = y;
                x = bfsNodes[tmp_x][tmp_y].prev_x;
                y = bfsNodes[tmp_x][tmp_y].prev_y;
                if(!(x==Start.getX() && y == Start.getY())){ // exclude the head of the snake
                    tmp = SnakeBody(x, y);
                    bfsPath.push_back(tmp);
                }
            }

            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = cur.getX() + dx[i];
            int ny = cur.getY() + dy[i];

            SnakeBody curNext(nx, ny);

            if (0 < nx && nx < (mGameBoardWidth - 1) && 0 < ny && ny < (mGameBoardHeight - 1) && bfsMap[nx][ny] ==0) {

                //  visit for the first time，aka: price == 0
                if (bfsNodes[nx][ny].price == 0) {
                    que.push(curNext);
                    bfsNodes[nx][ny].price = bfsNodes[cur.getX()][cur.getY()].price+ 1;

                    bfsNodes[nx][ny].prev_x = cur.getX();
                    bfsNodes[nx][ny].prev_y = cur.getY();
                }

                    // visit for the second time, just update the price
                else {
                    if (bfsNodes[nx][ny].price > bfsNodes[cur.getX()][cur.getY()].price + 1) {
                        bfsNodes[nx][ny].price = bfsNodes[cur.getX()][cur.getY()].price + 1;
                        bfsNodes[nx][ny].prev_x = cur.getX();
                        bfsNodes[nx][ny].prev_y = cur.getY();
                    }
                }
            }
        }
    }
    return false;
}

bool Snake::canFindBfsPathToTail(){
    // clear the bfsPath
    while(!this->bfsPath.empty()) {
        this->bfsPath.clear();
    }

    SnakeBody end = SnakeBody(getSnake().back().getX(), getSnake().back().getY());

    // initialize 2D map
    int bfsMap[mGameBoardWidth][mGameBoardHeight];// 0 means reachable, -1 means not

    for(int i = 0;i < mGameBoardWidth;++ i)
        for(int j = 0; j < mGameBoardHeight ;++ j)
            if(i == 0 || i == mGameBoardWidth - 1|| j == 0 || j == mGameBoardHeight - 1){
                bfsMap[i][j] = -1;
            } else {
                bfsMap[i][j] = 0;
            }

    // terrain should be considered here!
    for(auto i:barriers){
        bfsMap[i.getX()][i.getY()] = -1;
    }

    // snake's body should be excluded!
    for(auto i:this->getSnake())
        bfsMap[i.getX()][i.getY()]=-1;

    // snake's tail should be set as reachable!
    bfsMap[this->getSnake().back().getX()][this->getSnake().back().getY()] = 0;

    // direction: Right, Down, Left, Up
    int dx[4] = { 1,0,-1,0 };
    int dy[4] = { 0,1,0,-1 };

    //array to store the previous map
    bfsNode bfsNodes[mGameBoardWidth][mGameBoardHeight];

    std::queue<SnakeBody>  que;

    SnakeBody Start((getSnake()[0].getX()),(getSnake()[0].getY()));

    que.push(Start);

    while (!que.empty()) {
        SnakeBody cur = que.front();
        que.pop();

        // f destination reached, store the path (without start)
        if ((cur.getX() == end.getX()) && (cur.getY() == end.getY())) {

            bfsPath.push_back(end);
            int x = end.getX(), y = end.getY();
            int tmp_x,tmp_y;
            SnakeBody tmp;
            while (!(x==Start.getX() && y == Start.getY())) {
                tmp_x = x;
                tmp_y = y;
                x = bfsNodes[tmp_x][tmp_y].prev_x;
                y = bfsNodes[tmp_x][tmp_y].prev_y;
                if(!(x==Start.getX() && y == Start.getY())){ // exclude the head of the snake
                    tmp = SnakeBody(x, y);
                    this->bfsPath.push_back(tmp);
                }
            }

            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = cur.getX() + dx[i];
            int ny = cur.getY() + dy[i];

            SnakeBody curNext(nx, ny);

            if (0 < nx && nx < (mGameBoardWidth - 1) && 0 < ny && ny < (mGameBoardHeight - 1) && bfsMap[nx][ny] ==0) {

                //  visit for the first time，aka: price == 0
                if (bfsNodes[nx][ny].price == 0) {
                    que.push(curNext);
                    bfsNodes[nx][ny].price = bfsNodes[cur.getX()][cur.getY()].price+ 1;

                    bfsNodes[nx][ny].prev_x = cur.getX();
                    bfsNodes[nx][ny].prev_y = cur.getY();
                }

                    // visit for the second time, just update the price
                else {
                    if (bfsNodes[nx][ny].price > bfsNodes[cur.getX()][cur.getY()].price + 1) {
                        bfsNodes[nx][ny].price = bfsNodes[cur.getX()][cur.getY()].price + 1;
                        bfsNodes[nx][ny].prev_x = cur.getX();
                        bfsNodes[nx][ny].prev_y = cur.getY();
                    }
                }
            }
        }
    }
    return false;
}

void Snake::wander(){

    int rand;
    int sign = 0;

    int x = getSnake()[0].getX();
    int y = getSnake()[0].getY();

    if(not (isPartOfSnake(x, y - 1) || checkBound(x, y - 1))){
        -- y;
    } else if(not (isPartOfSnake(x, y + 1) || checkBound(x, y + 1))){
        ++ y;
    } else if(not (isPartOfSnake(x - 1, y) || checkBound(x - 1, y))){
        -- x;
    } else {
        ++ x;
    }

//    while(true) {
//        rand = std::rand();
//        switch (rand % 4) {
//            case 0 :
//                if (isPartOfSnake(x, y - 1)) {
//                    continue;
//                }
//                sign = 0;
//                break;
//
//            case 1 :
//                if (isPartOfSnake(x, y + 1)) {
//                    continue;
//                }
//                sign = 1;
//                break;
//
//            case 2 :
//                if (isPartOfSnake(x + 1, y)) {
//                    continue;
//                }
//                sign = 2;
//                break;
//
//            case 3 :
//                if (isPartOfSnake(x - 1, y)) {
//                    continue;
//                }
//                sign = 3;
//                break;
//            default:
//                break;
//        }
//        break;
//    }

    this->lastTail = getSnake()[getLength() - 1];
    for (int i = getLength() - 1; i > 0; i--) {

        getSnake()[i] = SnakeBody(getSnake()[i - 1].getX(), getSnake()[i - 1].getY());

    }

    getSnake()[0] = SnakeBody(x, y);
//    switch (sign) {
//        case 0 : getSnake()[0] = SnakeBody(x, y + 1); break ;
//        case 1 : getSnake()[0] = SnakeBody(x, y - 1); break ;
//        case 2 : getSnake()[0] = SnakeBody(x + 1, y); break ;
//        case 3 : getSnake()[0] = SnakeBody(x - 1, y); break ;
//        default:break;
//    }

    return;
}

bool Snake::moveByPath(){

    this->lastTail = getSnake()[getLength()- 1];

    for (int i = getLength() - 1; i > 0; i--) {
        getSnake()[i] = SnakeBody(getSnake()[i - 1].getX(), getSnake()[i - 1].getY());
    }

    getSnake()[0] = bfsPath.back();
    bfsPath.pop_back();

    if(bfsPath.empty()){
        return false;   // food has been eaten
    }
    else {
        return true;
    }
}

void Snake::followSnakeTail(){

    this->lastTail = getSnake()[getLength()- 1];

    for (int i = getLength() - 1; i > 0; i--) {
        getSnake()[i] = SnakeBody(getSnake()[i - 1].getX(), getSnake()[i - 1].getY());
    }

    getSnake()[0] = this->bfsPath[bfsPath.size() -1];
}

// ----------------------------


// Barriers

void Snake::addBarrier(SnakeBody barrier) {
    barriers.push_back(barrier);
}

std::vector<SnakeBody> Snake::getBarriers() {
    return barriers;
}

// TODO renderBarriers()

// ----------------------------


