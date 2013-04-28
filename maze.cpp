//********************************************************
// PROJECT 6: Maze Solver by Won You
//
// This program will read in a user-specified file
// containing a maze, and output the solution path 
// if one exists 
//
//********************************************************


#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int N_ROWS = 30; //Maximum number of allowable rows
const int N_COLS = 80; //Maximum number of allowable columns


void printMaze(char maze[][N_COLS], const int& nRows, const int& nCols){

  for (int i=0; i < nRows; ++i)            // Print it out.
     {
       for (int j=0; j < nCols; ++j)
	  cout << maze[i][j];

       cout << endl;
     }
     cout << endl;
}

//----------------------------------------------------------
// Find and assign the x,y positions for the starting point 
// and the destination, if they exist. Otherwise, 
// return flag = 2 implies success, exactly 2 endpoints
// return flag = 0 implies failure due to not enough endpoints
// return flag = 1 implies failure due to only 1 endpoint
// return flag = 3 implies failure due to more than 2 endpoints
//----------------------------------------------------------

int findEndpts(char maze[][N_COLS], int& x1, int& y1, int dest[2],
                const int& nRows, const int& nCols)
{
  int i, j, flag=0;
  bool tester=false;

  for (i = 0; i < nRows; ++i)            // Print it out.
     {
       for (j = 0; j < nCols; ++j)
	 if (maze[i][j] == '$' && tester==false){
            x1 = i;
            y1 = j;
            tester = true;
            flag++;
         } 
         else if (maze[i][j] == '$' && tester==true){
            dest[0] = i;
            dest[1] = j;
            tester = false;
            flag++;
	 }
  
     }

  //Display the appropriate error message          
   if (flag == 0){
       printMaze(maze, nRows, nCols);
       cout << "Error 0:There is no endpoints in the maze!" << endl;
   }
   else if (flag == 1){
       printMaze(maze, nRows, nCols);
       cout << "Error 1:there is only one endpoint in the maze!" << endl;    
   }   
   else if (flag > 2){
       printMaze(maze, nRows, nCols);
       cout << "Error 2:there are more than 2 endpoints in the maze!" << endl;   }

  return flag;
}

//Determine if the given coordinates x,y are a valid position
bool validMove(const int& x, const int& y, char maze[][N_COLS], 
               int dest[2]){

 
 if (((maze[x][y]!='+') &&  (maze[x][y]!='*'))
     && ((maze[x][y]==' ') || (x==dest[0] && y==dest[1])))
    return true;
 else
    return false;
}    

//////////////////////////////////////////////////////////////
//
// This function is the maze solving engine
// A depth-first search is performed to traverse the tree
//
//////////////////////////////////////////////////////////////

bool move(char maze[][N_COLS], int dest[2], 
          int x, int y, const int& nRows, 
          const int& nCols, bool& testFlag)
{
 
  if ((x<=nRows && x>=0) && (y<=nCols && y>=0)){

    //Basic Case: We've reached the solution
    if (x==dest[0] && y==dest[1]){

      maze[x][y] = '$';   
      testFlag = true;

    }

    if(!testFlag){

      //Place a + here to indicate that we visited this position
      if(maze[x][y]!='$')
	maze[x][y]='+';

      //Move South
      if ( ((x+1)<=nRows) && (validMove((x+1), y, maze,dest))) 

	move(maze, dest, x+1, y, nRows, nCols, testFlag);

      //Move East
      if ( ((y+1)<=nCols) && (validMove(x, (y+1), maze, dest)))

	 move(maze, dest, x, y+1, nRows, nCols, testFlag);      

      //Move North
      if ( ((x-1)>=0) && (validMove((x-1), y, maze, dest)))
    
	move(maze, dest, x-1, y, nRows, nCols, testFlag);

      //Move West
      if ( ((y-1)>=0) && (validMove(x, y-1, maze, dest)))

	move(maze, dest, x, y-1, nRows, nCols, testFlag);

      //The solution is found for this node, so place a * here 
      if (testFlag && maze[x][y]!='$')
         maze[x][y]='*';     

      else if (maze[x][y] == '$')
         maze[x][y]='$'; 

    }


  }

    return testFlag;
}   


//Grab the arguments and set the input file
//and output file and the search history correctly 
void getCmd( int argc, char* argv[], ifstream& infile, 
             ofstream& outfile, bool& search_Hist ){

   bool fail = false;
   string s1,s2; 

   //Make sure there are at least 2 arguments
   if ( argc >= 2 ) {

      infile.open(argv[1]);
      fail = !infile;
	 
      //Check to see if there are more than 2 arguments
      if (!fail && argc >=3 ){
	if (argc == 3){
          s1 = argv[2];
          if (s1 =="-h")
             search_Hist = true;
          else
             outfile.open( argv[2] );
             fail = !outfile; 
	}
        else if (argc == 4){
           s1 = argv[2];
	   s2 = argv[3];

	  if ((s1 != "-h") && (s2 == "-h")){
            cout <<"We got here" <<endl;
             search_Hist = true;
             outfile.open( argv[2] );
             fail = !outfile;

	   }
	  else if ((s1 == "-h") && (s2 != "-h")) {
             search_Hist = true;
	     outfile.open( argv[3] ); 
             fail = !outfile;
	  } 
	  else 
	     fail = true;
	}
      }  
   }
   else
     fail = true;

   if (fail) {
      cout << "Calling Syntax: maze <input file-name> [-h] [output file]\n";
      cout << "or:  maze <input file-name> [output file] [-h]\n";
      exit(1);
   }

}

//initialize the maze matrix to the one contained in the file 
int readMaze ( ifstream &InFile, char maze[ ][ N_COLS ], 
                int &nRows, int &nCols )
{

/*
           ---------------------------------
 PreConditions:
   maze[][] is an array of size N_ROWS X N_C0LS

 PostConditions: 

  Array argument:
      maze   holds the maze as a 2-dimensional grid

  Reference Arguments:
      nRows     == number of rows in the maze file  (must be <= N_ROWS)
      nCols     == number of columns in the maze file (must be <= N_COLS)

           ---------------------------------
*/
   int i = 0, j;    // Row and column indices.
   int flag = 0;    // Return value is 0 if file is ok.
   char c;

   nRows= 0,  nCols= 0;
 
   while ( InFile && i < N_ROWS )     // Get the next row
    {
      j = 0; // Column index.
      while ( InFile.get(c) && c != '\n' && j < N_COLS )  
      {
         maze[i][j] = c;                   // Next character of maze
         ++j;
      }
      if (j > nCols) 
          nCols= j;            // Count the number of columns in the maze
                               //   (allows non-rectangular mazes).
      if ( InFile )     
         ++i;
    }
    nRows= i;                  // Count the number of rows

// ---------------------------------------
//  Check Error Conditions.
// ---------------------------------------
    if ( nRows >= N_ROWS || nCols >= N_COLS )
    {
       cerr << "Sorry, my " << N_ROWS << " x " << N_COLS 
            << " array isn't big enough to hold your maze.\n";
       flag = 1;
    }
    return flag;
}


//Write out the solutions to the maze, both to the
//screen and output file, if one was supplied
void printSolution(char maze[][N_COLS], const int& nRows, 
                   const int& nCols, ofstream& os, 
                   const bool& giveHist){

        //Loop through the rows
	   for (int i=0; i < nRows; i++){

        //Loop through the columns
	     for (int j=0; j < nCols; j++){

                 if (!giveHist && maze[i][j]=='+'){ 
                   cout << " ";
                   if (os)
	                   os << " ";
                 }
                 else{
		   cout << maze[i][j];
		   if (os)
			   os << maze[i][j];
                 }
	     }
   
	     cout << endl;
	     if (os)
		     os << endl;
	   }
}

int main(int argc, char *argv[]) {

   ifstream myfile;               //This is the input file
   ofstream myoutput;             //This is the output file 
   bool searchHistory = false,    //Does the user want the search history
     successFlag= false;          //Test to see if a path leads to a solution
   char maze[ N_ROWS ][ N_COLS ]; //original maze read from a file 
   int nRows, nCols,              //The dimensions of the maze
       dest[] = {0, 0}, //The array of the x and y coordinates of the destination
       x=0, y=0, //The x and y coordinates of the starting position 
       solvable=0;  //Flag to see if there are exactly 2 endpoints in the maze

   //Grab the command line arguments
   getCmd( argc, argv, myfile, myoutput, searchHistory);

   // Read in the maze
   int flag = readMaze ( myfile, maze, nRows, nCols ); 
  
   if (flag == 0){
 
     solvable = findEndpts(maze, x, y, dest, nRows, nCols);
      
     //Try to solve the maze if there are 2 endpoints
     if (solvable==2){

       //Is there a solution?
       if(move( maze, dest, x, y, nRows, nCols, successFlag ))
	 {
         cout << "\nSuccess!\n " << endl;
         printSolution(maze, nRows, nCols, myoutput, searchHistory);
     
	 }
       else{
         cout << "\nNo solution exists!\n" << endl;
         printSolution(maze, nRows, nCols, myoutput, true);
       }

     }

   }
   else
     return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
