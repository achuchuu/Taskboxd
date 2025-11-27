#include <iostream>
#include <vector>
#include <string>
using namespace std;

//  Function Declarations 
void mainMenu();
void questMenu();
void dailyQuestsMenu();
void statsMenu();
void inventoryMenu();
void settingsMenu();

//  MAIN 
int main() {
    mainMenu(); // Start the main menu
    return 0;
}

//  MAIN MENU 
void mainMenu() {
    int option;

    while (true) {
        cout << "┌─────────────────────°❀⋆.ೃ࿔*:･°❀⋆.ೃ࿔*:────────────────────┐\n";
        cout << "│                                                          │\n";
        cout << "│                      T A S K B O X D                     │\n";
        cout << "│                \"Gamify your Study Grind!\"                │\n";
        cout << "│                                                          │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        cout << "│                                                          │\n";
        cout << "│  Player: Name                           XP: □□□□□□□□ 0%  │\n";
        cout << "│  Level 100                         Stamina: □□□□□□□□ 0%  │\n";
        cout << "│                                                          │\n";
        cout << "│----------------------------------------------------------│\n";
        cout << "│                                                          │\n";
        cout << "│ ✦ Quick Actions . ݁₊ ⊹ . ݁˖ . ݁                             │\n";
        cout << "│                                                          │\n";
        cout << "│   ➀ Quests               ➁ Companion                     │\n";
        cout << "│   ➂ Stats & Progress     ➃ Inventory                     │\n";
        cout << "│   ➄ Exit                                                 │\n";
        cout << "│                                                          │\n";
        cout << "└──────────────────────────────────────────────────────────┘\n";

        cout << "Enter option: ";
        cin >> option;

        switch(option) {
            case 1: questMenu(); break;
            case 2: cout << "Companion menu (not implemented)\n"; break;
            case 3: statsMenu(); break;
            case 4: inventoryMenu(); break;
            case 5: 
                cout << "Exiting...\n"; 
                return; // exit mainMenu and end program
            default: 
                cout << "Invalid option! Press Enter to try again...\n";
        }
    }
}

//  QUEST MENU 
void questMenu() {
    int choice;

    do {
        cout << "\n┌──────────────────── QUESTS ────────────────────┐\n";
        cout << "│  1. Daily Quests                               │\n";
        cout << "│  2. Dungeon Quests                             │\n";
        cout << "│  3. Timed Boss Fights                          │\n";
        cout << "│  4. Side Quests                                │\n";
        cout << "│                                                │\n";
        cout << "│  0. Back                                       │\n";
        cout << "└────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch(choice) {
            case 1: dailyQuestsMenu(); break;
            case 2: cout << "Dungeon Quests selected.\n"; break;
            case 3: cout << "Timed Boss Fights selected.\n"; break;
            case 4: cout << "Side Quests selected.\n"; break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }

        if(choice != 0) {
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while(choice != 0);
}

//  DAILY QUESTS MENU 

vector<string> dailyQuests;

void dailyQuestsMenu()
{
    int option;

    do {
        cout << "\n┌────────── DAILY QUESTS ──────────┐\n";
        cout << "│  1. Add Quest                    │\n";
        cout << "│  2. Complete Quest               │\n";

        if (dailyQuests.empty())
            cout << "│  - No quests yet -               │\n";
        else {
            cout << "│  Your Quests:                    │\n";
            for (int i = 0; i < dailyQuests.size(); i++) {
                cout << "│   " << i + 1 << ". " << dailyQuests[i] << "\n";
            }
        }

        cout << "│                                  │\n";
        cout << "│  0. Back                         │\n";
        cout << "└──────────────────────────────────┘\n";
        cout << "Choose: ";
        cin >> option;
        cin.ignore();

        if (option == 1)
        {
            string quest;
            cout << "Enter quest description: ";
            getline(cin, quest);
            dailyQuests.push_back(quest);
        }
        else if (option == 2)
        {
            if (dailyQuests.empty())
            {
                cout << "No quests to complete!\n";
            }
            else
            {
                int index;
                cout << "Enter quest number to complete: ";
                cin >> index;

                if (index > 0 && index <= dailyQuests.size())
                {
                    dailyQuests.erase(dailyQuests.begin() + (index - 1));
                    cout << "Quest completed!\n";
                }
                else
                {
                    cout << "Invalid quest number!\n";
                }
            }
        }

        } while (option != 0);
    }

//  STATS MENU 
void statsMenu() {
    int choice;
    do {
        cout << "\n┌─────────── S T A T S  &  P R O G R E S S ───────────┐\n";
        cout << "│   1. System Status                                   │\n";
        cout << "│   2. Achievements                                    │\n";
        cout << "│      - Earned Titles                                 │\n";
        cout << "│      - Completed Achievements                        │\n";
        cout << "│                                                      │\n";
        cout << "│   0. Back                                             │\n";
        cout << "└──────────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch(choice) {
            case 1: cout << "System Status shown here.\n"; break;
            case 2: cout << "Achievements shown here.\n"; break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }

        if(choice != 0) {
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while(choice != 0);
}

//  INVENTORY MENU 
void inventoryMenu() {
    int choice;
    do {
        cout << "\n┌──────────────── I N V E N T O R Y ────────────────┐\n";
        cout << "│   1. Classes                                       │\n";
        cout << "│   2. Titles                                        │\n";
        cout << "│   3. Potions                                       │\n";
        cout << "│                                                    │\n";
        cout << "│   0. Back                                          │\n";
        cout << "└────────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch(choice) {
            case 1: cout << "Classes shown here.\n"; break;
            case 2: cout << "Titles shown here.\n"; break;
            case 3: cout << "Potions shown here.\n"; break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }

        if(choice != 0) {
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while(choice != 0);
}