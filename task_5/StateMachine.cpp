#include <iostream>
#include <string>
using namespace std;

// The three possible states of the rover
enum State {
    IDLE,
    MOVING,
    AVOID
};

int main() {
    State currentState = IDLE;  // Default state

    bool obstacleDetected = false;
    bool pathClear = false;

    char command;

    cout << "=== AUTONOMOUS ROVER FSM ===\n";
    cout << "Initial state: IDLE\n";

    while (true) {
        cout << "\nCurrent State: ";

        switch (currentState) {

            case IDLE:
                cout << "IDLE (Motors stopped)\n";
                break;

            case MOVING:
                cout << "MOVING (Moving forward)\n";
                break;

            case AVOID:
                cout << "AVOID (Obstacle avoidance)\n";
                break;
        }

        cout << "\nCommands:\n";
        cout << "s - Start / resume rover\n";
        cout << "o - Simulate obstacle detection\n";
        cout << "c - Simulate clear path\n";
        cout << "x - Stop rover (IDLE)\n";
        cout << "q - Quit program\n";
        cout << "Enter command: ";

        cin >> command;

        if (command == 'q') {
            cout << "Exiting rover simulation.\n";
            break;
        }

        // State transition logic
        switch (currentState) {

            case IDLE:
                if (command == 's') {
                    currentState = MOVING;
                    cout << "Transition: IDLE -> MOVING\n";
                }
                else {
                    cout << "Rover remains in IDLE.\n";
                }
                break;

            case MOVING:
                if (command == 'o') {
                    obstacleDetected = true;
                }

                if (obstacleDetected) {
                    currentState = AVOID;
                    obstacleDetected = false;
                    cout << "Obstacle detected!\n";
                    cout << "Transition: MOVING -> AVOID\n";
                    cout << "Stopping forward motion and avoiding.\n";
                }
                else if (command == 'x') {
                    currentState = IDLE;
                    cout << "Transition: MOVING -> IDLE\n";
                }
                else {
                    cout << "Rover continues moving.\n";
                }
                break;

            case AVOID:
                if (command == 'c') {
                    pathClear = true;
                }

                if (pathClear) {
                    currentState = MOVING;
                    pathClear = false;
                    cout << "Path is clear!\n";
                    cout << "Transition: AVOID -> MOVING\n";
                }
                else if (command == 'x') {
                    currentState = IDLE;
                    cout << "Transition: AVOID -> IDLE\n";
                }
                else {
                    cout << "Rover is avoiding the obstacle.\n";
                }
                break;
        }
    }

    return 0;
}
