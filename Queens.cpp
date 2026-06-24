#include<iostream>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
using namespace std;

bool canPlace(int r, int c, vector<bool>& rowUsed,vector<bool>& colUsed,vector<pair<int,int>>& queens) //Function to check legality of a cell
{
    if(rowUsed[r])
        return false;

    if(colUsed[c])
        return false;

    for(const auto& q : queens)
    {
        int qr = q.first;
        int qc = q.second;

        if(abs(r-qr)<=1 && abs(c-qc)<=1)   //this condition checks diagonally adjacent (touching) cells
        {
            return false;
        }
    }

    return true;
}


// Optimization 2:
// Forward Checking
// After placing a queen, verify that every remaining color
// still has at least one legal cell available.
// If an entire color gets blocked, backtrack immediately.
bool forwardCheck(int nextColorIndex,
                  vector<char>& colors,
                  map<char, vector<pair<int,int>>>& colorCells,
                  vector<bool>& rowUsed,
                  vector<bool>& colUsed,
                  vector<pair<int,int>>& queens)
{
    // Check all remaining colors
    for(int i = nextColorIndex; i < colors.size(); i++)
    {
        char color = colors[i];

        bool foundValidCell = false;

        // Check whether this color still has at least one valid cell
        for(const auto& cell : colorCells[color])
        {
            int r = cell.first;
            int c = cell.second;

            if(canPlace(r,c,rowUsed,colUsed,queens))
            {
                foundValidCell = true;
                break;
            }
        }

        // Entire color got blocked
        if(!foundValidCell)
            return false;
    }

    return true;
}


bool Solve(int colorIndex,
           vector<char>& colors,
           map<char, vector<pair<int,int>>>& colorCells,
           vector<bool>& rowUsed,
           vector<bool>& colUsed,
           vector<pair<int,int>>& queens)
{
    // Base case: all colors processed
    if(colorIndex == colors.size())
        return true;

    char currentColor = colors[colorIndex];

    // Try every cell belonging to this color
    for(const auto& cell : colorCells[currentColor])
    {
        int r = cell.first;
        int c = cell.second;

        if(canPlace(r, c, rowUsed, colUsed, queens))
        {
            // Place queen
            rowUsed[r] = true;
            colUsed[c] = true;
            queens.push_back({r, c});

            // Optimization 2:
            // If this placement blocks every possible cell of
            // any remaining color, backtrack immediately.
            if(forwardCheck(colorIndex + 1,
                            colors,
                            colorCells,
                            rowUsed,
                            colUsed,
                            queens))
            {
                if(Solve(colorIndex + 1,
                         colors,
                         colorCells,
                         rowUsed,
                         colUsed,
                         queens))
                {
                    return true;
                }
            }

            // Backtrack
            rowUsed[r] = false;
            colUsed[c] = false;
            queens.pop_back();
        }
    }

    return false;
}

int main()
{
    int n;

    cout<<"Enter board size (n): ";
    cin>>n;

    vector<vector<char>> colorBoard(n,vector<char>(n)); //matrix consisting of the corresponding color symbol in every cell

    cout<<"Enter the color matrix:\n";
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>colorBoard[i][j];
        }
    }

    map<char, vector<pair<int,int>>> colorCells;  // a map where color symbol is the key and corresponding cells are the values

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            colorCells[colorBoard[i][j]].push_back({i,j});
        }
    }

    vector<char> colors;  // a vector of all the existing colors on the colorBoard

    for(const auto& entry : colorCells)
    {
        colors.push_back(entry.first);
    }

    // Validation condition: no. of colors should be equal to matrix size
    if(colors.size() != n)
    {
        cout << "\nInvalid puzzle!" << endl;
        cout << "Expected " << n
             << " unique colors but found "
             << colors.size() << endl;
        return 0;
    }

    // Optimization 1:
    // Most Constrained Variable (MRV)
    // Solve colors having fewer cells first.
    // This usually reduces backtracking significantly.
    sort(colors.begin(),
         colors.end(),
         [&](char a, char b)
         {
             return colorCells[a].size() < colorCells[b].size();
         });

    vector<bool> rowUsed(n,false);
    vector<bool> colUsed(n,false);

    vector<pair<int,int>> queens; //vector consisting of the coordinates of queens placed

    bool found = Solve(0,
                       colors,
                       colorCells,
                       rowUsed,
                       colUsed,
                       queens);

    if(found)
    {
        cout << "\nSolution found:\n";

        for(const auto& q : queens)                  //for loop to print solution
        {
            cout << "(" << q.first << ", " << q.second << ")\n";
        }
    }
    else
    {
        cout << "\nNo solution exists.\n";
    }

    return 0;
}
