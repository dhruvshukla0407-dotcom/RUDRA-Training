#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int SIZE = 20;
const int INF = 1000000000;

// --------------------------------------------------
// STATE MACHINE
// --------------------------------------------------

enum State
{
    IDLE,
    MOVING,
    AVOIDANCE
};

string stateName(State state)
{
    switch (state)
    {
        case IDLE:
            return "IDLE";

        case MOVING:
            return "MOVING";

        case AVOIDANCE:
            return "AVOIDANCE";
    }

    return "UNKNOWN";
}

// --------------------------------------------------
// NODE FOR DIJKSTRA
// --------------------------------------------------

struct Node
{
    int x;
    int y;
    int distance;

    bool operator>(const Node& other) const
    {
        return distance > other.distance;
    }
};

// --------------------------------------------------
// GRID
//  -1 = obstacle
//   1 = normal terrain
//   3 = expensive terrain
//   5 = very expensive terrain
// --------------------------------------------------

int weight[SIZE][SIZE];

// Distance from start
int distanceGrid[SIZE][SIZE];

// Parent of every cell
pair<int, int> parent[SIZE][SIZE];

// Movement directions
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};


// --------------------------------------------------
// DIJKSTRA
// --------------------------------------------------

bool dijkstra(pair<int, int> start, pair<int, int> goal)
{
    // Reset distance and parent
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            distanceGrid[i][j] = INF;
            parent[i][j] = {-1, -1};
        }
    }

    priority_queue<
        Node,
        vector<Node>,
        greater<Node>
    > pq;

    distanceGrid[start.first][start.second] = 0;

    pq.push({
        start.first,
        start.second,
        0
    });

    while (!pq.empty())
    {
        Node current = pq.top();
        pq.pop();

        int x = current.x;
        int y = current.y;

        // Ignore outdated queue entries
        if (current.distance != distanceGrid[x][y])
            continue;

        // Goal reached
        if (x == goal.first && y == goal.second)
            break;

        // Check 4 neighbouring cells
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Check grid boundaries
            if (nx < 0 || nx >= SIZE ||
                ny < 0 || ny >= SIZE)
                continue;

            // Obstacle
            if (weight[nx][ny] == -1)
                continue;

            // Edge weight = cost of entering next cell
            int edgeWeight = weight[nx][ny];

            int newDistance =
                distanceGrid[x][y] + edgeWeight;

            // Relaxation
            if (newDistance < distanceGrid[nx][ny])
            {
                distanceGrid[nx][ny] = newDistance;

                parent[nx][ny] = {x, y};

                pq.push({
                    nx,
                    ny,
                    newDistance
                });
            }
        }
    }

    return distanceGrid[goal.first][goal.second] != INF;
}


// --------------------------------------------------
// RECONSTRUCT PATH
// --------------------------------------------------

vector<pair<int, int>> getPath(
    pair<int, int> start,
    pair<int, int> goal)
{
    vector<pair<int, int>> path;

    pair<int, int> current = goal;

    while (current != start)
    {
        path.push_back(current);

        current = parent[current.first][current.second];

        if (current.first == -1)
        {
            path.clear();
            return path;
        }
    }

    path.push_back(start);

    reverse(path.begin(), path.end());

    return path;
}


// --------------------------------------------------
// PRINT GRID
// --------------------------------------------------

void printGrid(
    pair<int, int> start,
    pair<int, int> goal,
    vector<pair<int, int>> path)
{
    char grid[SIZE][SIZE];

    // Create basic grid
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (weight[i][j] == -1)
                grid[i][j] = '#';

            else
                grid[i][j] = '.';
        }
    }

    // Mark path
    for (auto cell : path)
    {
        grid[cell.first][cell.second] = '*';
    }

    // Mark start and goal
    grid[start.first][start.second] = 'S';
    grid[goal.first][goal.second] = 'G';

    cout << "\n20 x 20 GRID\n\n";

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            cout << grid[i][j] << " ";
        }

        cout << endl;
    }
}


// --------------------------------------------------
// PRINT PATH
// --------------------------------------------------

void printPath(vector<pair<int, int>> path)
{
    cout << "\nSHORTEST PATH:\n";

    for (auto cell : path)
    {
        cout << "("
             << cell.first
             << ","
             << cell.second
             << ")";

        if (&cell != &path.back())
            cout << " -> ";
    }

    cout << endl;
}


// --------------------------------------------------
// MAIN
// --------------------------------------------------

int main()
{
    // --------------------------------------------------
    // CREATE GRID
    // --------------------------------------------------

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            weight[i][j] = 1;
        }
    }

    // --------------------------------------------------
    // EXPENSIVE TERRAIN
    // --------------------------------------------------

    // Vertical expensive region
    for (int i = 5; i <= 14; i++)
    {
        weight[i][10] = 5;
    }

    // Horizontal expensive region
    for (int j = 3; j <= 7; j++)
    {
        weight[12][j] = 3;
    }

    // --------------------------------------------------
    // OBSTACLES
    // --------------------------------------------------

    // Horizontal obstacle
    for (int j = 4; j <= 15; j++)
    {
        weight[8][j] = -1;
    }

    // Vertical obstacle
    for (int i = 8; i <= 14; i++)
    {
        weight[i][4] = -1;
    }

    // --------------------------------------------------
    // START AND GOAL
    // --------------------------------------------------

    pair<int, int> start = {0, 0};
    pair<int, int> goal = {19, 19};

    // --------------------------------------------------
    // STATE MACHINE
    // --------------------------------------------------

    State currentState = IDLE;

    cout << "Initial State: "
         << stateName(currentState)
         << endl;


    // ==================================================
    // STATE 1 : IDLE
    // ==================================================

    if (currentState == IDLE)
    {
        cout << "\n[IDLE]"
             << endl;

        cout << "Rover is stopped."
             << endl;

        cout << "Start command received."
             << endl;

        // Transition
        currentState = MOVING;
    }


    // ==================================================
    // STATE 2 : MOVING
    // ==================================================

    if (currentState == MOVING)
    {
        cout << "\n[MOVING]"
             << endl;

        cout << "Calculating shortest path..."
             << endl;

        bool pathFound = dijkstra(start, goal);

        if (!pathFound)
        {
            cout << "No path available!"
                 << endl;

            return 0;
        }

        vector<pair<int, int>> path =
            getPath(start, goal);

        printGrid(start, goal, path);

        printPath(path);

        cout << "\nTotal Path Cost: "
             << distanceGrid[goal.first][goal.second]
             << endl;

        // --------------------------------------------------
        // SIMULATE OBSTACLE DETECTION
        // --------------------------------------------------

        cout << "\nRover is following the path..."
             << endl;

        cout << "Obstacle detected on current route!"
             << endl;

        // Transition
        currentState = AVOIDANCE;
    }


    // ==================================================
    // STATE 3 : AVOIDANCE
    // ==================================================

    if (currentState == AVOIDANCE)
    {
        cout << "\n[AVOIDANCE]"
             << endl;

        cout << "Stopping rover."
             << endl;

        cout << "Recalculating path..."
             << endl;

        // --------------------------------------------------
        // SIMULATE NEW OBSTACLE
        // --------------------------------------------------

        // Suppose rover discovered an obstacle
        // at (10,10)

        weight[10][10] = -1;

        // Recalculate path
        bool pathFound = dijkstra(start, goal);

        if (!pathFound)
        {
            cout << "No alternative path!"
                 << endl;

            return 0;
        }

        vector<pair<int, int>> newPath =
            getPath(start, goal);

        cout << "New path found!"
             << endl;

        printGrid(start, goal, newPath);

        printPath(newPath);

        cout << "\nNew Path Cost: "
             << distanceGrid[goal.first][goal.second]
             << endl;

        // Transition back to MOVING
        currentState = MOVING;
    }


    // ==================================================
    // FINAL STATE
    // ==================================================

    if (currentState == MOVING)
    {
        cout << "\n[MOVING]"
             << endl;

        cout << "Rover is following the new path."
             << endl;
    }

    return 0;
}
