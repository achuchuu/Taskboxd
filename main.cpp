#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

//  Function Declarations 
void mainMenu();
void questMenu();
void dailyQuestsMenu();
void statsMenu();
void inventoryMenu();
void settingsMenu();
void dungeonQuestsMenu();
void bossFight();
void COMPANIONMENU();


struct Boss {
    string name;
    int maxHP;
    int currentHP;
};


//  MAIN 
int main() { 
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
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
            case 2: COMPANIONMENU(); break;
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
        cout << "│  2. Boss Fights                                │\n";
        cout << "│  3. Side Quests                                │\n";
        cout << "│                                                │\n";
        cout << "│  0. Back                                       │\n";
        cout << "└────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch(choice) {
            case 1: dailyQuestsMenu(); break;
            case 2: bossFight(); break;
            case 3: cout << "Side Quests selected.\n"; break;
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

//  BOSS FIGHT 
void displayBoss(const Boss &boss) {
    cout << "\n===== BOSS FIGHT: " << boss.name << " =====\n";
    cout << R"(
          ,     \    /      ,
         / \    )\__/(     / \
        /   \  (_\  /_)   /   \
   ____/_____\__\@  @/___/_____\____
  |             |\../|              |
  |              \VV/               |
  |       THE TERRIBLE BOSS         |
  |_________________________________|
        |    /\ /      \ /\    |
        |  /   V        V   \  |
        |/                \|
    )";
    
    cout << "\nHP: " << boss.currentHP << " / " << boss.maxHP << endl;
}

void bossFight() {
    Boss boss = {"Shadow King", 100, 100};
    vector<string> subtasks;
    int damagePerTask = 20; // each task completed deals 20 damage

    while (boss.currentHP > 0) {
        displayBoss(boss);
        
        cout << "\n1. Add Attack Task\n";
        cout << "2. Complete a Task (Attack Boss!)\n";
        cout << "3. Run Away (Exit)\n";
        cout << "Choose: ";
        
        int choice;
        cin >> choice;

        if (choice == 1) {
            cin.ignore();
            cout << "Enter an attack task: ";
            string task;
            getline(cin, task);
            subtasks.push_back(task);
            cout << "Task added!\n";
        }
        else if (choice == 2) {
            if (subtasks.empty()) {
                cout << "No tasks to attack with! Add some first.\n";
            } else {
                cout << "Choose a task to complete:\n";
                for (size_t i = 0; i < subtasks.size(); i++) {
                    cout << i + 1 << ". " << subtasks[i] << endl;
                }

                int taskChoice;
                cin >> taskChoice;

                if (taskChoice > 0 && taskChoice <= subtasks.size()) {
                    cout << "You completed: " << subtasks[taskChoice - 1] << " 🔥\n";
                    boss.currentHP -= damagePerTask;
                    if (boss.currentHP < 0) boss.currentHP = 0;
                    subtasks.erase(subtasks.begin() + (taskChoice - 1));
                } else {
                    cout << "Invalid task!\n";
                }
            }
        }
        else if (choice == 3) {
            cout << "You fled the fight...\n";
            return;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    cout << "\n🎉 YOU DEFEATED THE BOSS!\n";
    cout << "Shadow King has fallen! 💥\n";
}

void COMPANIONMENU() {
    int companion_option, mood, bond_lvl, trust;
    char companion, B, G, P;

    do {
        cout << "\n┌────────────────────COMPANION────────────────┐\n";
        cout << "│  1. Pick a Companion                           │\n";
        cout << "│  2. Companion Stats                            │\n";
        cout << "│  3. Replace Companion                          │\n";
        cout << "│                                                │\n";
        cout << "│  0. Back                                       │\n";
        cout << "└────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        cin >> companion_option;

        switch(companion_option) {
            case 1: //Pick your companion
                cout << "Choose who you want to grind with everyday!\n";
                cout << "1. ByteBun (insert art)\n";
                cout << "Cheerful, loves streaks, gains EXP from consistency!\n";
                cout << "2. Gear Cub (insert art)\n";
                cout << "Hardworking, thrives in long quests and exams!\n";
                cout << "3. Pixel Limp (insert art)\n";
                cout << "Mischievous, random bonuses, dramatic when you procrastinate!\n";
                cin >> companion;

                if (companion == 'B') {
                    cout << "You picked ByteBun! 🐰💻\n";
                } else if (companion == 'G') {
                    cout << "You picked Gear Cub! 🧸⚙️\n";
                } else if (companion == 'P') {
                    cout << "You picked Pixel Limp! 🐾🎮\n";
                } else {
                    cout << "Invalid companion choice!\n";
                }

            case 2: //Companion Stats
                cout << "Companion" << companion << endl; break;
                cout << "Mood" << mood << endl; 
                cout << "Bond Level" << bond_lvl << endl; 
                cout << "Trust" << trust; 
                break;

            case 3: //Replace Companion
            cout << "Replace Companion\n"; break;

            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }

        if(companion_option != 0) {
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while(companion_option != 0);
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