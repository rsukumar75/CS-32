//If the start location is equal to the ending location, then we've
//solved the maze, so return true.
//Mark the start location as visted.
//For each of the four directions,
//If the location one step in that direction (from the start
//                                            location) is unvisited,
//then call pathExists starting from that location (and
//                                                  ending at the same ending location as in the
//                                                  current call).
//If that returned true,
//then return true.
//Return false.

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    if(sr==er && sc==ec)
        return true;
    maze[sr][sc]='E';
    if(sc!=nCols-1 && maze[sr][sc+1]=='.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    }
    if(sr!=nRows-1 && maze[sr+1][sc]=='.')
    {
        if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    }
    if(sc!=0 && maze[sr][sc-1]=='.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    }
    if(sr!=0 && maze[sr-1][sc]=='.')
    {
        if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    }
    return false;
}



