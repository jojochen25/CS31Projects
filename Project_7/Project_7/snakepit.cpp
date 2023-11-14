//
//  snakepit.cpp
//  Project_7
//
//  Created by Jodie Chen on 6/1/23.
//

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <type_traits>
#include <cassert>
#define CHECKTYPE(c, f, r, a)  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
       "FAILED: You changed the type of " #c "::" #f);  \
    [[gnu::unused]] auto xxx##c##_##f = static_cast<r(c::*)a>(&c::f)
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;     // max number of rows in the pit
const int MAXCOLS = 40;     // max number of columns in the pit
const int MAXSNAKES = 150;  // max number of snakes allowed

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;
const int NUMDIRS = 4;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Pit;  // This is needed to let the compiler know that Pit is a
            // type name, since it's mentioned in the Snake declaration.

class Snake
{
  public:
        // Constructor
    Snake(Pit* pp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();

  private:
    Pit* m_pit;
    int  m_row;
    int  m_col;
};

class Player
{
  public:
        // Constructor
    Player(Pit *pp, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    void   stand();
    void   move(int dir);
    void   setDead();

  private:
    Pit*  m_pit;
    int   m_row;
    int   m_col;
    int   m_age;
    bool  m_dead;
};

class Pit
{
  public:
        // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display() const;

        // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Snake*  m_snakes[MAXSNAKES];
    int     m_nSnakes;

      // Helper functions
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nSnakes);
    ~Game();

        // Mutators
    void play();

  private:
    Pit* m_pit;
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
int decodeDirection(char dir);
bool directionToDeltas(int dir, int& rowDelta, int& colDelta);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Snake implementation
///////////////////////////////////////////////////////////////////////////

Snake::Snake(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** A snake must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > pp->rows()  ||  c < 1  ||  c > pp->cols())
    {
        cout << "***** Snake created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
}

int Snake::row() const
{
    return m_row;
}

int Snake::col() const
{
    return m_col;
}

//NEED TO CHECK
void Snake::move()
{
      // Attempt to move in a random direction; if it can't move, don't move
    switch (randInt(0, NUMDIRS-1))
    {
      case UP:
            if(m_row > 1)
                m_row--;
            break;
        break;
      case DOWN:
            if(m_row < m_pit->rows())
                m_row++;
            break;
      case LEFT:
            if(m_col > 1)
                m_col--;
            break;
      case RIGHT:
            if(m_col < m_pit->cols())
                m_col++;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Pit* pp, int r, int c)
{
    if (pp == nullptr)
    {
        cout << "***** The player must be created in some Pit!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > pp->rows()  ||  c < 1  ||  c > pp->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_pit = pp;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;  // this is wrong -- replace it
}

int Player::col() const
{
    return m_col;  // this is wrong -- replace it
}

int Player::age() const
{
    return m_age;  // this is wrong -- replace it
}

void Player::stand()
{
    m_age++;
}

//DONE
void Player::move(int dir)
{
    //update player's location
    //if there is a snake adjacent, jump over that
    m_age++;
    switch(dir) {
        case UP:
            if(m_row <= 1) //make sure you dont access anything out of bounds, m_row should start at 1
                break;
            //if there's a snake above you so m_row - 1
            if(m_pit->numberOfSnakesAt(m_row-1, m_col) == 0)
                m_row--;
            else { //if there is at least one snake above, destroy the snake
                if(m_row-2 <= 0) //make sure that it's not out of bounds
                    break;
                m_pit->destroyOneSnake(m_row-1, m_col);
                m_row-=2; //player's position (row number) is now 2 squares up.
                
                //check if there is at least one snake where the player is currently at, if so, m_dead=true;
                if(m_pit->numberOfSnakesAt(m_row, m_col) >= 1)
                    setDead();
            }
            break;
            //if there's empty space above you
        case DOWN:
            if(m_row >= m_pit->rows()) //make sure you dont access anything out of bounds, m_row should start at 1
                break;
            //if there's a snake above you so m_row - 1
            if(m_pit->numberOfSnakesAt(m_row+1, m_col) == 0)
                m_row++;
            else { //if there is at least one snake above, destroy the snake
                if(m_row+2 > m_pit->rows()) //make sure that it's not out of bounds
                    break;
                m_pit->destroyOneSnake(m_row+1, m_col);
                m_row+=2; //player's position (row number) is now 2 squares up.
                
                //check if there is at least one snake where the player is currently at, if so, m_dead=true;
                if(m_pit->numberOfSnakesAt(m_row, m_col) >= 1)
                    setDead();
            }
            break;
            
        case LEFT:
            if(m_col - 1 < 1) //starts with 1
                break;
            if(m_pit->numberOfSnakesAt(m_row, m_col-1) == 0) //if no snakes,
                m_col--;
            else { //if there are snakes
                if(m_col-2 < 1) //make sure that it's not out of bounds
                    break;
                m_pit->destroyOneSnake(m_row, m_col-1);
                m_col-=2; //player's position (row number) is now 2 squars right.
                
                //check if there is at least one snake where the player is currently at, if so, m_dead=true;
                if(m_pit->numberOfSnakesAt(m_row, m_col) >= 1)
                    setDead();
            }
            break;
            
        case RIGHT: //if we move right we do m_col + 1
            //m_col starts with 0--that's the first column
            //m_pit->cols gives you the number of columns, but accessing it is also from 0 to 6
            
            //if(m_col + 1 >= m_pit->cols() --> break, not valid
            
            //this makes sense,
            if(m_col + 1 > m_pit->cols()) //m_col + 1 = m_pit->cols()
                break;
            if(m_pit->numberOfSnakesAt(m_row, m_col+1) == 0)
                m_col++;
            else { //there are snakes in the square player wants to move right to
                if(m_col+2 > m_pit->cols()) //make sure that it's not out of bounds
                    break;
                m_pit->destroyOneSnake(m_row, m_col+1);
                m_col+=2; //player's position (row number) is now 2 squars right.
                
                //check if there is at least one snake where the player is currently at, if so, m_dead=true;
                if(m_pit->numberOfSnakesAt(m_row, m_col) >= 1)
                    setDead();
            }
            break;
            //how to test this function,
    }

      //  If there's a grid position in the indicated direction that is
      //        adjacent to the player and vacant, move the player there.  If
      //        there's a grid position in the indicated direction that is
      //        adjacent to the player and contains one or more snakes, with
      //        a grid position next to and on the other side of the snake(s),
      //        jump over the snake(s), killing (destroying) one, and landing
      //        at that position (and dying if a snake is there).  In any
      //        other case, don't move.
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Pit implementation
///////////////////////////////////////////////////////////////////////////

Pit::Pit(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Pit created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nSnakes = 0;
}

Pit::~Pit()
{
    //DONE
    delete m_player; //pointer is still there but doesn't hold anything
    for(int i = 0; i < m_nSnakes; i++) {
        delete m_snakes[i];
    }
    
    m_nSnakes = 0; //??
}

int Pit::rows() const
{
    return m_rows;
}

int Pit::cols() const
{
    return m_cols;
}

Player* Pit::player() const
{
    return m_player;
}

int Pit::snakeCount() const
{
    return m_nSnakes;
}

//DONE
int Pit::numberOfSnakesAt(int r, int c) const
{
    //return number of snakes at a specific row and column
    int numSnakes = 0; //increment this to keep track of number of snakes
    
    /*for loop and some conditionals*/
    
    //what is m_snakes in the context of the grid again
    
    //run through everything in the pointer array m_snakes, consisting of pointers pointing to each
    //snake object
    
    //m_snakes array is a pointer (ignore this), m_snakes[i] is also pointer that points to a
    //Snake object; so m_snakes is just an array of pointers that point to each snake object
    //to access the accessor row() and col()(member function in the snake object)
    //-> get stuff in the Snake object
    //get stuff inside snake
    
    //m_snakes should be number of Snake objects
    for(int i = 0; i< m_nSnakes; i++) { //m_snakes should be figured out from add_snakes
        //use something with int r and int c
        //following the pointer to the object and select member field of that obj
        if(m_snakes[i]->row() == r && m_snakes[i]->col() == c)
            numSnakes++;
    }
    
    //a pointer to the first element of the array m_snakes[0]
    //array of pointers, each pointer pointing to an object in the array
    
    return numSnakes;
}

//DONE
void Pit::display() const //2 nested for loops; how to do this
{
    
      // Position (row,col) in the pit coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

        // Indicate each snake's position
    //this should be a switch statement, need numberofsnakesat first
    //nested for loops
    
    //for loop through the grid
    //for loop through the pointer array
    // how to check if a snake is at a grid point, using the snake object,
    
    //figure out the number of snakes at each position--numberofsnakesat
    
    //DO WE HAVE A VARIABLE THAT CONTAINS THE number of snakes in each pos--numberofsnakesat
    //for loop through the pointer array, each element is a pointer that points to a snake obj
    //want to get the row and col that the snake is at
    //m_nSnakes -1?
    //for each snake obj, get the row and col (position in grid) that the snake is at,
    //and find the numberofsnakes at that pos row, col
    for (int i = 0; i < m_nSnakes; i++) {
        int currRow = m_snakes[i]->row();
        int currCol = m_snakes[i]->col();
        
        int nSnakes = numberOfSnakesAt(currRow, currCol); //get number of snakes at the current row and col
        //grid starts at row 0, col 0 but row and col start at 1
        switch(nSnakes) {
            case 0:
                break;
            case 1:
                grid[currRow-1][currCol-1] = 'S';
                break;
            case 2:
                grid[currRow-1][currCol-1] = '2';
                break;
            case 3:
                grid[currRow-1][currCol-1] = '3';
                break;
            case 4:
                grid[currRow-1][currCol-1] = '4';
                break;
            case 5:
                grid[currRow-1][currCol-1] = '5';
                break;
            case 6:
                grid[currRow-1][currCol-1] = '6';
                break;
            case 7:
                grid[currRow-1][currCol-1] = '7';
                break;
            case 8:
                grid[currRow-1][currCol-1] = '8';
                break;
            default:
                grid[currRow-1][currCol-1] = '9';
            
        }
        
    }

        // Indicate player's position
    if (m_player != nullptr)
    {
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (m_player->isDead())
            gridChar = '*';
        else
            gridChar = '@';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write snake and player info
    cout << "There are " << snakeCount() << " snakes remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

//VERIFY IF THIS IS CORRECT ESP FOR THE LAST COMMENT
//PROB FINE, WORKS AS EXPECTED, YOU WANT TO DECREMENT WHEN YOU ARE DESTROYING SNAKES
//dynamically allocating a snake object
bool Pit::addSnake(int r, int c) //do this one first
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a snake on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

    // If MAXSNAKES have already been added, return false.  Otherwise,
    // dynamically allocate a new snake at coordinates (r,c).  Save the
    // pointer to newly allocated snake and return true.
    if(m_nSnakes >= MAXSNAKES)
        return false;
    
    //dynamically allocate a new Snake object at coordinates (r,c)
    m_snakes[m_nSnakes] = new Snake(this, r, c);
    m_nSnakes++;
    
    return true;

      // Your function must work as specified in the preceding paragraph even
      // in this scenario (which won't occur in this game):  MAXSNAKES
      // are added, then some are destroyed, then more are added.
        //
    
    //this should work, even if it's outside the function
    //should decrement the number of snakes

   
}

bool Pit::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a snake
    if (numberOfSnakesAt(r, c) > 0)
        return false;

      // Dynamically allocate new Player and add it to the pit
    m_player = new Player(this, r, c);
    return true;
}
//check the bounds
bool Pit::destroyOneSnake(int r, int c)
{
    
    for(int i = 0; i<m_nSnakes; i++) {
        if(m_snakes[i]->row()==r && m_snakes[i]->col() == c) {//if so, delete it
            delete m_snakes[i]; //delete the snakes obj, if you delete the obj, pointer becomes a null pointer
            //move everything back one (to the left) so we don't encounter a null pointer, similar to proj 4
            m_nSnakes--; //decrement the number of elements in the m_snakes pointer array (number of pointers - 1)
            for(int j = i; j < m_nSnakes; j++)
                m_snakes[j] = m_snakes[j+1];
            
            return true;
        }
    }
    return false;
}

//DONE
bool Pit::moveSnakes()
{
    for (int k = 0; k < m_nSnakes; k++)
    {
        m_snakes[k]->move();
      //        If that move results in that snake being in the same
      //        position as the player, the player dies.
        //if the move results in snake being in the same square player is in, player dies
        //when row, col of snake is the same as row, col of player
        if(m_snakes[k]->row() == m_player->row() && m_snakes[k]->col() == m_player->col())
            m_player->setDead();
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

bool Pit::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nSnakes)
{
    if (nSnakes < 0)
    {
        cout << "***** Cannot create Game with negative number of snakes!" << endl;
        exit(1);
    }
    if (nSnakes > MAXSNAKES)
    {
        cout << "***** Trying to create Game with " << nSnakes
             << " snakes; only " << MAXSNAKES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nSnakes > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the snakes!" << endl;
        exit(1);
    }

        // Create pit
    m_pit = new Pit(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_pit->addPlayer(rPlayer, cPlayer);

      // Populate with snakes
    while (nSnakes > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a snake where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_pit->addSnake(r, c);
        nSnakes--;
    }
}

Game::~Game()
{
    delete m_pit;
}

void Game::play()
{
    m_pit->display();
    Player* player = m_pit->player();
    if (player == nullptr)
        return;

    while ( ! m_pit->player()->isDead()  &&  m_pit->snakeCount() > 0)
    {
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)
            player->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                player->move(decodeDirection(action[0]));
                break;
            }
        }
        if ( ! player->isDead())
            m_pit->moveSnakes();
        m_pit->display();
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

bool directionToDeltas(int dir, int& rowDelta, int& colDelta)
{
    switch (dir)
    {
      case UP:     rowDelta = -1; colDelta =  0; break;
      case DOWN:   rowDelta =  1; colDelta =  0; break;
      case LEFT:   rowDelta =  0; colDelta = -1; break;
      case RIGHT:  rowDelta =  0; colDelta =  1; break;
      default:     return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////
void thisFunctionWillNeverBeCalled()
{
      // If the student deleted or changed the interfaces to the public
      // functions, this won't compile.  (This uses magic beyond the scope
      // of CS 31.)

    Snake(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Snake, row, int, () const);
    CHECKTYPE(Snake, col, int, () const);
    CHECKTYPE(Snake, move, void, ());

    Player(static_cast<Pit*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, age, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, stand, void, ());
    CHECKTYPE(Player, move, void, (int));
    CHECKTYPE(Player, setDead, void, ());

    Pit(1, 1);
    CHECKTYPE(Pit, rows, int, () const);
    CHECKTYPE(Pit, cols, int, () const);
    CHECKTYPE(Pit, player, Player*, () const);
    CHECKTYPE(Pit, snakeCount, int, () const);
    CHECKTYPE(Pit, numberOfSnakesAt, int, (int,int) const);
    CHECKTYPE(Pit, display, void, () const);
    CHECKTYPE(Pit, addSnake, bool, (int,int));
    CHECKTYPE(Pit, addPlayer, bool, (int,int));
    CHECKTYPE(Pit, destroyOneSnake, bool, (int,int));
    CHECKTYPE(Pit, moveSnakes, bool, ());

    Game(1,1,1);
    CHECKTYPE(Game, play, void, ());
}

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 3, 2);
    
    // Remove this line after completing test.
    //return 0;       // Remove this line after completing test.

    Game g(9, 10, 20);
    
    

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32  //  Windows

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Windows

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
