//Authors: Favian (Ian) Samatha, Michael Kim, Marco Law
//Date: November 27, 2017
//This is a C++ program that will play cylinder mule checkers
//The game is relatively close to checkers, but with a bit of twist
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <string>
#include <fstream>



using namespace std;
//  Global constants are declared and initialized
const int MAX_ARRAY_SIZE = 18;
const int MIN_ARRAY_SIZE = 8;
const int MAX_PIECES = 72; 
const int NOPLAYER = 0;
const int WHITEWINS = 1;
const int REDWINS = 2;
const int NOONEWINS = 0;
const int WHITESOLDIER = 1;
const int WHITEMULE = 2;
const int WHITEKING = 3;
const int REDSOLDIER = 4;
const int REDMULE = 5;
const int REDKING = 6;
const int WHITEPLAYER = 1;
const int REDPLAYER = 2;

void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);
bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex);
int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[],  int yLocArray[]);
bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);
bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);
bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped);
bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int main()
{
	const int maxTries = 3;
	bool jumpvalid = 0;
	bool jumped = 0;
	int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {0};
	int i = 0;
	int j = 0;
	int xIndicesMove[MAX_PIECES] = {0};
	int yIndicesMove[MAX_PIECES] = {0};
	int xIndicesJump[MAX_PIECES] = {0};
	int yIndicesJump[MAX_PIECES] = {0};
	int numRowsInBoard = 0;
	int attempts = 0;
	int player = 1; 
	int jumpcount = 0;
	int movecount = 0;
	int fromSquareNumber = 0;
	int toSquareNumber = 0;
	int locationy = 0;
	int locationX = 0;
	int squareNum = 0;
	int tries = 0;
	int ylocfuture = 0;
	int xlocfuture = 0;
	
	char terminating[1024] = {'\0'};

	//  This will prompt the user for the number of squares along the board
	//  If the board size is not an integer, its not between 8, number of squares, and 18, an tries will display
	do 
	{
		cout << "Enter the number of squares along each edge of the board\n";
		if (!(cin >> numRowsInBoard))
		{
			cerr << "ERROR:  Board size is not an integer\n8 <= number of squares <= 18\n";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		else if (numRowsInBoard % 2 == 1)
		{
			cerr << "ERROR:  Board size odd\n8 <= number of squares <= 18\n";
		}
		else if (numRowsInBoard > MAX_ARRAY_SIZE)
		{
			cerr << "ERROR:  Board size too large\n8 <= number of squares <= 18\n";
		}
		else if (numRowsInBoard < MIN_ARRAY_SIZE )
		{
			cerr << "ERROR:  Board size too small\n8 <= number of squares <= 18\n";
		}
		else
		{
			break;
		}
		attempts++;
	}while(attempts < maxTries);

	if (attempts >= maxTries)
	{
		cerr << "ERROR: Too many errors entering the size of the board\n";
		return 1;
	}
	//  Using our functions, it will create the game board
	InitializeBoard(myCMCheckersBoard, numRowsInBoard);
	DisplayBoard(myCMCheckersBoard, numRowsInBoard);

	//  This is the start of the game
	do
	{
		//  These 2 functions will check all possible moves for the checkers
		jumpcount = CountJumps(myCMCheckersBoard, numRowsInBoard, player, xIndicesJump, yIndicesJump);
		movecount = CountMove1Squares( myCMCheckersBoard, numRowsInBoard, player, xIndicesMove, yIndicesMove);

		tries = 0;
		jumpvalid = false;

		if (player == WHITEWINS && jumpcount == 0 && movecount == 0)
		{
			cout << "White is unable to move\nGAME OVER, Red has won\nEnter any character to close the game\n";
		}
		else if (player == REDPLAYER && jumpcount == 0 && movecount == 0)
		{
			cout << "Red is unable to move\nGAME OVER, White has won\nEnter any character to close the game\n";
		}
		else if (player == WHITEPLAYER && (jumpcount != 0 || movecount != 0))
		{
			cout << "White takes a turn\n";
		}
		else if (player == REDPLAYER && (jumpcount != 0 || movecount != 0))
		{
			cout << "Red takes a turn\n";
		}
		do
		{
			do
			{
				tries = 0;
				jumpvalid = false;
				//  Asks the user for which checker (in which position) they want to move
				//  Errors will display if no checkers are there
				cout << "Enter square number of the checker you want to move\n";
				if (!(cin >> fromSquareNumber))
				{
					cerr << "ERROR: you did not enter an integer\nTry again\n";
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
				else if (fromSquareNumber < 0 || fromSquareNumber > (numRowsInBoard * numRowsInBoard - 1))
				{
					cerr << "ERROR: that square is not on the board.\nTry again\n";
					continue;
				}
				locationy = fromSquareNumber / numRowsInBoard;
				locationX = fromSquareNumber % numRowsInBoard;
				if (player == WHITEPLAYER)
				{
					if (myCMCheckersBoard[locationy][locationX] == REDSOLDIER || myCMCheckersBoard[locationy][locationX] == REDMULE || myCMCheckersBoard[locationy][locationX] == REDKING)
					{
						cerr << "ERROR: that square contains an opponent's checker\nTry again\n";
						continue;
					}
				}
				if (player == REDPLAYER)
				{
					if (myCMCheckersBoard[locationy][locationX] == WHITESOLDIER || myCMCheckersBoard[locationy][locationX] == WHITEMULE || myCMCheckersBoard[locationy][locationX] == WHITEKING)
					{
						cerr << "ERROR: that square contains an opponent's checker\nTry again\n";
						continue;
					}
				}
				if (myCMCheckersBoard[locationy][locationX] == 0)
				{
					cerr << "ERROR: that square is empty\nTry again\n";
					continue;
				}
				for (i = 0; i < jumpcount; i++)
				{
					if (locationy == yIndicesJump[i] && locationX == xIndicesJump[i])
					{
						jumpvalid = true;
						break;
					}
					else if (i == jumpcount - 1)
					{
						tries++;
						cerr << "ERROR: You can jump with another checker, you may not move your chosen checker\n";
						cerr << "You can jump using checkers on the following squares: ";
						for (j = 0; j < jumpcount; j++)
						{
							squareNum = yIndicesJump[j] * numRowsInBoard + xIndicesJump[j];
							cout << squareNum << " ";
						}
						cerr << "\nTry again\n";
						break;
					}
				}
				if (tries == 1)
				{
					continue;
				}
				
				
				for (i = 0; i < movecount; i++)
				{
					if (locationy == yIndicesMove[i] && locationX == xIndicesMove[i])
					{
						break;
					}
					else if (i == movecount - 1 && jumpvalid == false)
					{
						tries++;
						cerr << "ERROR: There is no legal move for this checker\nTry again\n";
					}
				}
				if (tries == 1)
				{
					continue;
				}
				break;
			}while (player != 0);

			do
			{
				//  Asks where the user wants the checker to move
				//  The following checks if it is a valid move
				//  If a jump is available, it's move (if it is not a jump) will not be valid
				tries = 0;
				cout << "Enter the square number of the square you want to move your checker to\n";
				if (!(cin >> toSquareNumber))
				{
					cerr << "ERROR: you did not enter an integer\nTry again\n";
					cin.clear();
					cin.ignore(1000, '\n');
					continue;
				}
				else if (toSquareNumber < 0 || toSquareNumber > (numRowsInBoard * numRowsInBoard - 1))
				{
					cerr << "ERROR: It is not possible to move to a square that is not on the board.\nTry again\n";
					continue;
				}
				ylocfuture = toSquareNumber / numRowsInBoard;
				xlocfuture = toSquareNumber % numRowsInBoard;
				if (myCMCheckersBoard[ylocfuture][xlocfuture] != 0)
				{
					cerr << "ERROR: It is not possible to move to a square that is already occupied\nTry again\n";
					continue;
				}
				
				for (i = 0; i < jumpcount; i++)
				{
					if (xIndicesJump[i] == locationX  && yIndicesJump[i] == locationy && abs(ylocfuture - locationy) == 1)
					{
						tries++;
						cerr << "ERROR: You can jump with this checker, you must jump not move 1 space\nTry again\n";
						break;
					}
				}
				if (tries == 1)
				{
					continue;
				}
				if ( !(MakeMove(myCMCheckersBoard, numRowsInBoard, player, fromSquareNumber, toSquareNumber, jumped)) )
				{
					cerr << "ERROR: moving to that square is not legal, Try again\n";
					continue;
				}
				break;
			}while (player != 0);
			break;
		}while (player != 0);

		while (jumped)
		{
			//  If player did jump, the player gets a chance to move the checker again
			//  It will check for all possible valid moves
			jumped = 0;
			if(IsJump(myCMCheckersBoard, numRowsInBoard, player, xlocfuture, ylocfuture))
			{
				DisplayBoard(myCMCheckersBoard, numRowsInBoard);
				fromSquareNumber = ylocfuture * numRowsInBoard + xlocfuture;
				locationX = xlocfuture;
				locationy = ylocfuture;
				do
				{
					tries = 0;
					cout << "You can jump again, Please enter the next square you wish to move your checker to\n";
					if (!(cin >> toSquareNumber))
					{
						cerr << "ERROR: you did not enter an integer\nTry again\n";
						cin.clear();
						cin.ignore(1000, '\n');
						continue;
					}
					else if (toSquareNumber < 0 || toSquareNumber > (numRowsInBoard * numRowsInBoard - 1))
					{
						cerr << "ERROR: It is not possible to move to a square that is not on the board.\nTry again\n";
						continue;
					}
					ylocfuture = toSquareNumber / numRowsInBoard;
					xlocfuture = toSquareNumber % numRowsInBoard;
					if (myCMCheckersBoard[ylocfuture][xlocfuture] != 0)
					{
						cerr << "ERROR: It is not possible to move to a square that is already occupied\nTry again\n";
						continue;
					}
					
					for (i = 0; i < jumpcount; i++)
					{
						if (xIndicesJump[i] == locationX && yIndicesJump[i] == locationy && abs(ylocfuture - locationy) == 1)
						{
							tries++;
							cerr << "ERROR: You can jump with this checker, you must jump not move 1 space\nTry again\n";
							break;
						}
					}
					if (tries == 1)
					{
						continue;
					}
					if ( !(MakeMove(myCMCheckersBoard, numRowsInBoard, player, fromSquareNumber, toSquareNumber, jumped)) )
					{
						cerr << "ERROR: moving to that square is not legal, Try again\n";
						continue;
					}
					break;
				}while (player != 0);
			}
		}
		DisplayBoard(myCMCheckersBoard, numRowsInBoard);

		//  If there is a winner
		if (CheckWin(myCMCheckersBoard, numRowsInBoard))
		{
			cout << "Enter any character to terminate the game then press the enter key\n";
			if ( cin >> terminating ) break;
		}
		//  If one player has made a turn, it's the other player's turn
		if(player == WHITEPLAYER)
		{
			player = REDPLAYER;
		}
		else
		{
			player = WHITEPLAYER;
		}
	}while( player != 0);


	
	return 0;
}

//  This funciton initializes the board in the beginning
void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	int i = 0;
	int j = 0;
	
	for (i = 0; i < numRowsInBoard; i++)
	{
		for (j = 0; j < numRowsInBoard; j++)
		{
			if ((i == 0) && (j % 2 == 1))
			{
				CMCheckersBoard[i][j] = WHITEMULE;
			}
			else if ((i % 2 == 0) && ( i < numRowsInBoard/2 - 1) && (j % 2 == 1))
			{
				CMCheckersBoard[i][j] = WHITESOLDIER;
			}
			else if ((i % 2 == 1) && ( i <  numRowsInBoard/2 - 1) && (j % 2 == 0))
			{
				CMCheckersBoard[i][j] = WHITESOLDIER;
			}
			else if ((i == numRowsInBoard - 1) && (j % 2 == 0))
			{
				CMCheckersBoard[i][j] = REDMULE;
			}
			else if ((i % 2 == 0) && (i > numRowsInBoard/2) && (j % 2 == 1))
			{
				CMCheckersBoard[i][j] = REDSOLDIER;
			}
			else if ((i % 2 == 1) && (i > numRowsInBoard/2) && (j % 2 == 0))
			{
				CMCheckersBoard[i][j] = REDSOLDIER;
			}
		}
	}
}
 //  This function will display the board with the given number of numRowsInBoard
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	int i = 0;
	int j = 0;
	int sum = 0;

	for (i = 0; i < numRowsInBoard; i++)
	{
		for (j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == WHITESOLDIER)
			{
				cout << setw(4) << "WS";
			}
			else if (CMCheckersBoard[i][j] == WHITEMULE)
			{
				cout << setw(4) << "WM";
			}
			else if (CMCheckersBoard[i][j] == WHITEKING)
			{
				cout << setw(4) << "WK";
			}
			else if (CMCheckersBoard[i][j] == REDSOLDIER)
			{
				cout << setw(4) << "RS";
			}
			else if (CMCheckersBoard[i][j] == REDMULE)
			{
				cout << setw(4) << "RM";
			}
			else if (CMCheckersBoard[i][j] == REDKING)
			{
				cout << setw(4) << "RK";
			}
			else
			{	
				cout  << setw(4) << sum;
			}
			sum++;
		}
		cout << endl;
	}
	cout << endl << endl << endl;
}

//  Checks the possible jumps that a checker can make
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	int i = 0;
	int j = 0;
	int count = 0;
	
	for (i = 0; i < numRowsInBoard; i++)
	{
		xLocArray[i] = -1;
		yLocArray[i] = -1;
	}
	
	for (i = 0; i < numRowsInBoard; i++)
	{
		for (j = 0; j < numRowsInBoard; j++)
		{
			if (IsJump (CMCheckersBoard, numRowsInBoard, player, j, i))
			{
				xLocArray[count] = j;
				yLocArray[count] = i;
				count++;
			}
		}
	}
	return count;
}
//  This function will check 2 repeating numbers in the array
bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex)
{
	int i = 0;
	
	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		if (inArray1[i] == xIndex && inArray2[i] == yIndex) 
		{
			return true;
		}
	}
	return false;
}
 
//  This will count the number of possible moves a player can make
int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[],  int yLocArray[])
{
	int i = 0;
	int j = 0;
	int count = 0;
	
	for (i = 0; i < numRowsInBoard; i++)
	{
		xLocArray[i] = -1;
		yLocArray[i] = -1;
	}
	
	for (i = 0; i < numRowsInBoard; i++)
	{
		for (j = 0; j < numRowsInBoard; j++)
		{
			if (IsMove1Square (CMCheckersBoard, numRowsInBoard, player, j, i))
			{
				xLocArray[count] = j;
				yLocArray[count] = i;
				count++;
			}
		}
	}
	return count;
}
 
//  This will check if a checkers' move is valid or not
bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER)
	{
		if ((CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING) && (yLoc != numRowsInBoard - 1))
		{
			if (xLoc == 0)  
			{
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == 0 || CMCheckersBoard[yLoc + 1][xLoc + 1] == 0)
				{
					return true;
				}
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				if (CMCheckersBoard[yLoc + 1][0] == 0 || CMCheckersBoard[yLoc + 1][xLoc - 1] == 0)
				{
					return true;
				}
			}
			else if (CMCheckersBoard[yLoc + 1][xLoc + 1] == 0 || CMCheckersBoard[yLoc + 1][xLoc - 1] == 0)
			{
				return true;
			}
		}
		if (CMCheckersBoard[yLoc][xLoc] == WHITEKING && yLoc != 0)
		{
			if (xLoc == 0)  
			{
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == 0 || CMCheckersBoard[yLoc - 1][xLoc + 1] == 0)
				{
					return true;
				}
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				if (CMCheckersBoard[yLoc - 1][0] == 0 || CMCheckersBoard[yLoc - 1][xLoc - 1] == 0)
				{
					return true;
				}
			}
			else if (CMCheckersBoard[yLoc - 1][xLoc + 1] == 0 || CMCheckersBoard[yLoc - 1][xLoc - 1] == 0)
			{
				return true;
			}
		}
	}

	else if (player == REDPLAYER)
	{
		if ((CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING) && (yLoc != 0))
		{
			if (xLoc == 0)  
			{
				if (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == 0 || CMCheckersBoard[yLoc - 1][xLoc + 1] == 0)
				{
					return true;
				}
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				if (CMCheckersBoard[yLoc - 1][0] == 0 || CMCheckersBoard[yLoc - 1][xLoc - 1] == 0)
				{
					return true;
				}
			}
			else if (CMCheckersBoard[yLoc - 1][xLoc + 1] == 0 || CMCheckersBoard[yLoc - 1][xLoc - 1] == 0)
			{
				return true;
			}
		}
		if (CMCheckersBoard[yLoc][xLoc] == REDKING && yLoc != numRowsInBoard - 1)
		{
			if (xLoc == 0)  
			{
				if (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == 0 || CMCheckersBoard[yLoc + 1][xLoc + 1] == 0)
				{
					return true;
				}
			}
			else if (xLoc == numRowsInBoard - 1)
			{
				if (CMCheckersBoard[yLoc + 1][0] == 0 || CMCheckersBoard[yLoc + 1][xLoc - 1] == 0)
				{
					return true;
				}
			}
			else if (CMCheckersBoard[yLoc + 1][xLoc + 1] == 0 || CMCheckersBoard[yLoc + 1][xLoc - 1] == 0)
			{
				return true;
			}
		}
	}
	return false;
}
//  This function will check for possible jumps
bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	if (player == WHITEPLAYER)
	{
		if ((CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITEKING) && (yLoc != numRowsInBoard - 1 && yLoc != numRowsInBoard - 2))
		{
			if (xLoc == 0 && (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING) && (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 1 && (CMCheckersBoard[yLoc + 1][0] == REDSOLDIER || CMCheckersBoard[yLoc + 1][0] == REDMULE || CMCheckersBoard[yLoc + 1][0] == REDKING) && (CMCheckersBoard[yLoc + 2][1] == 0))
			{
				return true;
			}
			else if (xLoc == 1 && (CMCheckersBoard[yLoc + 1][0] == REDSOLDIER || CMCheckersBoard[yLoc + 1][0] == REDMULE || CMCheckersBoard[yLoc + 1][0] == REDKING) && (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 2 && (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == REDKING) && (CMCheckersBoard[yLoc + 2][0] == 0))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc + 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == REDKING) && (CMCheckersBoard[yLoc + 2][xLoc + 2] == 0) && ( xLoc != numRowsInBoard - 1 && xLoc != numRowsInBoard - 2))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc + 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == REDKING) && (CMCheckersBoard[yLoc + 2][xLoc - 2] == 0) && ( xLoc != 0 && xLoc != 1))
			{
				return true;
			}
		}
		if (CMCheckersBoard[yLoc][xLoc] == WHITEKING && (yLoc != 0 && yLoc != 1))
		{
			if (xLoc == 0 && (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDKING) && (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 1 && (CMCheckersBoard[yLoc - 1][0] == REDSOLDIER || CMCheckersBoard[yLoc - 1][0] == REDMULE || CMCheckersBoard[yLoc - 1][0] == REDKING) && (CMCheckersBoard[yLoc - 2][0] == 0))
			{
				return true;
			}
			else if (xLoc == 1 && (CMCheckersBoard[yLoc - 1][0] == REDSOLDIER || CMCheckersBoard[yLoc - 1][0] == REDMULE || CMCheckersBoard[yLoc - 1][0] == REDKING) && (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 2 && (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == REDKING) && (CMCheckersBoard[yLoc - 2][0] == 0))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc - 1][xLoc + 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == REDKING) && (CMCheckersBoard[yLoc - 2][xLoc + 2] == 0) && ( xLoc != numRowsInBoard - 1 && xLoc != numRowsInBoard - 2))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc - 1][xLoc - 1] == REDSOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == REDKING) && (CMCheckersBoard[yLoc - 2][xLoc - 2] == 0) && ( xLoc != 0 && xLoc != 1))
			{
				return true;
			}
		}
	}

	if (player == REDPLAYER)
	{
		if ((CMCheckersBoard[yLoc][xLoc] == REDSOLDIER || CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDKING) && (yLoc != 0 && yLoc != 1))
		{
			if (xLoc == 0 && (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING) && (CMCheckersBoard[yLoc - 2][numRowsInBoard - 2] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 1 && (CMCheckersBoard[yLoc - 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][0] == WHITEMULE || CMCheckersBoard[yLoc - 1][0] == WHITEKING) && (CMCheckersBoard[yLoc - 2][1] == 0))
			{
				return true;
			}
			else if (xLoc == 1 && (CMCheckersBoard[yLoc - 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][0] == WHITEMULE || CMCheckersBoard[yLoc - 1][0] == WHITEKING) && (CMCheckersBoard[yLoc - 2][numRowsInBoard - 1] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 2 && (CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][numRowsInBoard - 1] == WHITEKING) && (CMCheckersBoard[yLoc - 2][0] == 0))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc + 1] == WHITEKING) && (CMCheckersBoard[yLoc - 2][xLoc + 2] == 0) && ( xLoc != numRowsInBoard - 1 && xLoc != numRowsInBoard - 2))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc - 1][xLoc - 1] == WHITEKING) && (CMCheckersBoard[yLoc - 2][xLoc - 2] == 0) && ( xLoc != 0 && xLoc != 1))
			{
				return true;
			}			
		}
		if (CMCheckersBoard[yLoc][xLoc] == REDKING && (yLoc != numRowsInBoard - 1 && yLoc != numRowsInBoard - 2))
		{
			if (xLoc == 0 && (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEKING) && (CMCheckersBoard[yLoc + 2][numRowsInBoard - 2] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 1 && (CMCheckersBoard[yLoc + 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][0] == WHITEMULE || CMCheckersBoard[yLoc + 1][0] == WHITEKING) && (CMCheckersBoard[yLoc + 2][1] == 0))
			{
				return true;
			}
			else if (xLoc == 1 && (CMCheckersBoard[yLoc + 1][0] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][0] == WHITEMULE || CMCheckersBoard[yLoc + 1][0] == WHITEKING) && (CMCheckersBoard[yLoc + 2][numRowsInBoard - 1] == 0))
			{
				return true;
			}
			else if (xLoc == numRowsInBoard - 2 && (CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][numRowsInBoard - 1] == WHITEKING) && (CMCheckersBoard[yLoc + 2][0] == 0))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc + 1] == WHITEKING) && (CMCheckersBoard[yLoc + 2][xLoc + 2] == 0) && ( xLoc != numRowsInBoard - 1 && xLoc != numRowsInBoard - 2))
			{
				return true;
			}
			else if ((CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITESOLDIER || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEMULE || CMCheckersBoard[yLoc + 1][xLoc - 1] == WHITEKING) && (CMCheckersBoard[yLoc + 2][xLoc - 2] == 0) && ( xLoc != 0 && xLoc != 1))
			{
				return true;
			}
		}
	}
	return false;
}

//  This will check if the move prompt by the user is a valid move
 
bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped)
{
	int distanceX = 0;
	int distanceY = 0;
	int locationX = 0;
	int locationy = 0;
	int xlocfuture = 0;
	int ylocfuture = 0;
	int differenceX = 0;
	int differenceY = 0;


	locationy = fromSquareNum / numRowsInBoard;
	locationX = fromSquareNum % numRowsInBoard;
	ylocfuture = toSquareNum / numRowsInBoard;
	xlocfuture = toSquareNum % numRowsInBoard;
	differenceX = locationX - xlocfuture;
	differenceY = locationy - ylocfuture;
if (player == REDPLAYER)
	{
		distanceY = 1;
	}
	else if (player == WHITEPLAYER )
	{
		distanceY = -1;
	}
	else
	{
		cerr << "illegal player, no move made" << endl;
		return false;
	}

	jumped = false;
	if( IsJump(CMCheckersBoard, numRowsInBoard, player, locationX, locationy) )
	{
		if( ( (abs(differenceX) == 2 || abs(differenceX) == numRowsInBoard-2 ) && abs(differenceY) == 2) &&
			( CMCheckersBoard[locationy][locationX] % 3 == 0 || distanceY * differenceY > 0 ) )
		{
			CMCheckersBoard[ylocfuture][xlocfuture] = CMCheckersBoard[locationy][locationX];
			CMCheckersBoard[locationy][locationX] = NOPLAYER; 
			if( abs(differenceX) == 2 )
			{
				CMCheckersBoard[locationy - differenceY/2 ][locationX - differenceX/2] = NOPLAYER;
			}
			else if( differenceX > 0 )
			{
				CMCheckersBoard[locationy - differenceY/2 ][(locationX + 1) % numRowsInBoard] = NOPLAYER;
			}
			else
			{
				CMCheckersBoard[locationy - differenceY/2 ][(locationX + numRowsInBoard-1) % numRowsInBoard] = NOPLAYER;
			}

			jumped = true;
		}
		else
		{
			cerr << "ERROR: Illegal move" << endl;
			return false;
		}
	}
	else if( ( (abs(differenceX) == 1 || abs(differenceX) == numRowsInBoard-1 ) && abs(differenceY) == 1)   &&
		(CMCheckersBoard[locationy][locationX] % 3 == 0 || distanceY * differenceY > 0 ) ) 
	{
		CMCheckersBoard[ylocfuture][xlocfuture] = CMCheckersBoard[locationy][locationX];
		CMCheckersBoard[locationy][locationX] = NOPLAYER; 
	}
	else
	{
		cerr << "ERROR: Illegal move" << endl;
		return false;
	}		

	if(distanceY > 0  && ylocfuture == 0 )
	{
		if( CMCheckersBoard[ylocfuture][xlocfuture] == REDMULE)
		{
			char a;
			cerr << "Red has created a Mule King,  White wins the game" << endl;
			cout << "Enter any character to terminate the game then press the enter key";
			cin >> a;
			exit(0);
		}
		else
		{
			CMCheckersBoard[ylocfuture][xlocfuture] = REDKING;
		}
	}		
	if(distanceY < 0  && ylocfuture == numRowsInBoard-1 )
	{		
		if( CMCheckersBoard[ylocfuture][xlocfuture] == WHITEMULE)
		{
			char c;
			cerr << "White has created a Mule King,  Red wins the game"<< endl;
			cout << "Enter any character to terminate the game then press the enter key";
			cin >> c;
			exit(0);
		}
		else
		{
			CMCheckersBoard[ylocfuture][xlocfuture] = WHITEKING;
		}
	}
	return true;
}

 
//  This function will continuously check if a winner is present
bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	int i = 0;
	int j = 0;
	int whitemule = 0;
	int redmule = 0;
	int white = 0;
	int red = 0;
	int whiteking = 0;
	int redking = 0;

	for (i = 0; i < numRowsInBoard; i++)
	{
		for (j = 0; j < numRowsInBoard; j++)
		{
			if (CMCheckersBoard[i][j] == WHITEMULE)
			{
				whitemule++;
			}
			else if (CMCheckersBoard[i][j] == WHITESOLDIER)
			{
				white++;
			}
			else if (CMCheckersBoard[i][j] == WHITEKING)
			{
				whiteking++;
			}
			else if (CMCheckersBoard[i][j] == REDMULE) 
			{
				redmule++;
			}
			else if (CMCheckersBoard[i][j] == REDSOLDIER)
			{
				red++;
			}
			else if (CMCheckersBoard[i][j] == REDKING)
			{
				redking++;
			}
		}
	}
	
	if (redmule == 0)
	{
		cout << "The Red Player has won the game by losing all of the Red Mules\n";
		return true;
	}
	else if (whitemule == 0)
	{
		cout << "The White Player has won the game by losing all of the White Mules\n";
		return true;
	}
	else if (whitemule != 0 && white == 0 && whiteking == 0)
	{
		cout << "The Red Player has won by capturing all of the white players soldiers and kings\n";
		return true;
	}
	else if (redmule != 0 && red == 0 && redking == 0)
	{
		cout << "The White Player has won by capturing all of the red players soldiers and kings\n";
		return true;
	}
	else 
	{
		return false;
	}
}