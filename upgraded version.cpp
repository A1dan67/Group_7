/
#include <iostream>
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // for time()
using namespace std;

// CONSTANTS 
const int LANES = 3;            // 0 = left lane, 1 = middle lane, 2 = right lane
const int ROAD_LENGTH = 5;      // how many rows ahead the matatu can "see"
const int MAX_OBSTACLES = 10;   // maximum obstacles allowed on the road at once
const int STARTING_LIVES = 3;   // number of crashes allowed before game over
const int WIN_SCORE = 15;       // score needed to finish the journey safely

// FUNCTION PROTOTYPES
void initObstacles(int lane[], int row[], char type[], bool active[]);
void spawnObstacle(int lane[], int row[], char type[], bool active[]);
void drawRoad(int matatuLane, int lane[], int row[], char type[], bool active[], int score, int lives);
char getPlayerAction();
void updateObstacles(int lane[], int row[], char type[], bool active[],
                      bool braking, int matatuLane, int &score, int &lives);
void printInstructions();

//  MAIN PROGRAM
int main() {
    srand((unsigned int)time(0)); // seed the random number generator

    int matatuLane = LANES / 2;   // start in the middle lane
    int score = 0;
    int lives = STARTING_LIVES;

    // Parallel arrays describing each obstacle slot
    int  obsLane[MAX_OBSTACLES];
    int  obsRow[MAX_OBSTACLES];
    char obsType[MAX_OBSTACLES];
    bool obsActive[MAX_OBSTACLES];

    initObstacles(obsLane, obsRow, obsType, obsActive);
    printInstructions();

    //  MAIN GAME LOOP
    while (lives > 0 && score < WIN_SCORE) {

        drawRoad(matatuLane, obsLane, obsRow, obsType, obsActive, score, lives);

        char action = getPlayerAction();
        bool braking = false;

        if (action == 'a' && matatuLane > 0) {
            matatuLane--;            // move left
        } else if (action == 'd' && matatuLane < LANES - 1) {
            matatuLane++;            // move right
        } else if (action == 's') {
            braking = true;          // brake / stay in place this turn
        }
        // any other key (or Enter) just keeps the matatu going straight

        updateObstacles(obsLane, obsRow, obsType, obsActive, braking, matatuLane, score, lives);

        // Randomly decide whether to spawn a new obstacle this turn
        if (rand() % 2 == 0) {
            spawnObstacle(obsLane, obsRow, obsType, obsActive);
        }
    }

    // GAME OVER / WIN MESSAGE 
    cout << "\n==============================\n";
    if (lives <= 0) {
        cout << "GAME OVER! The matatu crashed too many times.\n";
    } else {
        cout << "Congratulations! The matatu reached its destination safely!\n";
    }
    cout << "Final Score: " << score << "\n";
    cout << "==============================\n";

    return 0;
}

// FUNCTION DEFINITIONS

// Marks every obstacle slot as empty/inactive at the start of the game
void initObstacles(int lane[], int row[], char type[], bool active[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        lane[i] = -1;
        row[i] = -1;
        type[i] = ' ';
        active[i] = false;
    }
}

// Looks for a free obstacle slot and fills it with a new obstacle far ahead
void spawnObstacle(int lane[], int row[], char type[], bool active[]) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!active[i]) {
            active[i] = true;
            lane[i] = rand() % LANES;                  // random lane
            row[i] = ROAD_LENGTH;                       // appears far away
            type[i] = (rand() % 3 == 0) ? 'Z' : 'V';     // mostly vehicles, sometimes a zebra crossing
            return; // only spawn ONE obstacle per call
        }
    }
}

// Prints the road as text, showing the matatu and all active obstacles
void drawRoad(int matatuLane, int lane[], int row[], char type[], bool active[], int score, int lives) {
    cout << "\n";
    for (int r = ROAD_LENGTH; r >= 0; r--) {
        cout << "| ";
        for (int l = 0; l < LANES; l++) {
            char symbol = '.'; // empty road by default

            if (r == 0 && l == matatuLane) {
                symbol = 'M'; // the matatu's current position/lane
            }

            for (int i = 0; i < MAX_OBSTACLES; i++) {
                if (active[i] && row[i] == r && lane[i] == l) {
                    symbol = type[i]; // 'V' (vehicle) or 'Z' (zebra crossing)
                }
            }

            cout << symbol << " ";
        }
        cout << "|\n";
    }
    cout << "Score: " << score << "   Lives: " << lives << "\n";
}

// Reads a single character action from the player
char getPlayerAction() {
    char action;
    cout << "\nAction (a = left, d = right, s = brake, Enter = straight): ";
    cin.get(action);
    if (action != '\n') {
        cin.ignore(1000, '\n'); // discard the rest of the line, if any
    }
    return action;
}

// Moves obstacles closer (unless braking), checks for collisions,
// and updates the score and number of lives
void updateObstacles(int lane[], int row[], char type[], bool active[],
                      bool braking, int matatuLane, int &score, int &lives) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (!active[i]) {
            continue;
        }

        if (!braking) {
            row[i]--; // obstacle gets one step closer
        }

        if (row[i] <= 0) {
            if (lane[i] == matatuLane) {
                cout << "\n*** CRASH! The matatu hit a "
                     << (type[i] == 'Z' ? "pedestrian at a zebra crossing" : "vehicle")
                     << "! ***\n";
                lives--;
            } else {
                score++; // obstacle safely avoided
            }
            active[i] = false; // remove the obstacle either way
        }
    }
}

// Displays the welcome message and controls
void printInstructions() {
    cout << "===========================================\n";
    cout << " AUTONOMOUS MATATU - OBSTACLE AVOIDANCE GAME\n";
    cout << "===========================================\n";
    cout << "You are remotely monitoring a self-driving matatu.\n";
    cout << "Obstacles approach from the top of the road:\n";
    cout << "    V = another vehicle\n";
    cout << "    Z = a zebra crossing (pedestrians)\n";
    cout << "Controls:\n";
    cout << "    a      -> change to the LEFT lane\n";
    cout << "    d      -> change to the RIGHT lane\n";
    cout << "    s      -> BRAKE (stay in place for a turn)\n";
    cout << "    Enter  -> continue straight\n";
    cout << "Avoid letting an obstacle reach row 0 in your lane!\n";
}
