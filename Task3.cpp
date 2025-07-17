#include <iostream>
#include <cstdlib>   
#include <ctime>     
#include <limits>    

using namespace std;

// TASK 3: Number Guessing Game
int main() {
    srand(time(0)); 
    string playerName;
    char playAgain;

    cout << "Enter your name: ";
    getline(cin, playerName);

    cout << "\n Welcome, " << playerName << ", to the Number Guessing Game!\n";

    do {
        int secretNumber = rand() % 100 + 1;
        int guess, attempts = 0;

        cout << "\nI have picked a number between 1 and 100.\nTry to guess it!\n";

        // Game loop
        while (true) {
            cout << "Enter your guess: ";
            cin >> guess;

            // Input validation
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "❌ Invalid input. Please enter a number between 1 and 100.\n";
                continue;
            }

            if (guess < 1 || guess > 100) {
                cout << "❗ Guess must be between 1 and 100.\n";
                continue;
            }

            attempts++;

            if (guess > secretNumber) {
                cout << "Too high! Try a smaller number.\n";
            } else if (guess < secretNumber) {
                cout << "Too low! Try a bigger number.\n";
            } else {
                cout << "Well done, " << playerName << "! You guessed it in " << attempts << " attempts.\n";
                cout << "Your score: " << 100 - attempts + 1 << "\n";
                break;
            }
        }

        // Ask to play again
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;

        // Clear input buffer in case someone types something like "yes"
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\n👋 Thanks for playing, " << playerName << "! See you next time!\n";
    return 0;
}
