#include <iostream>
#include <vector>
#include <string>

using namespace std;

// STRUCTS
struct Player {
    string name;
    int level = 1;
    int exp = 0;
    int stamina = 100;
};

struct SideQuest {
    string name;
    bool completed = false;
};

struct Boss {
    string name;
    int maxHP;
    int currentHP;
};

// GLOBAL VARIABLES
Player player;
vector<string> dailyQuests;
vector<SideQuest> sideQuestList;

// FUNCTION DECLARATIONS
void mainMenu();
void questMenu();
void dailyQuestsMenu();
void sideQuestMenu();
void bossFight();
void statsMenu();
void inventoryMenu();
void settingsMenu();
void displayBar();
void displayBoss(const Boss &boss);

void gainEXP(int amount);
bool reduceStamina(int amount);

//  MAIN 
int main() {
    cout << "Enter your player name: ";
    getline(cin, player.name);
    mainMenu();
    return 0;
}

//  PLAYER FUNCTIONS 
void gainEXP(int amount) {
    player.exp += amount;
    cout << "You gained " << amount << " EXP!\n";
    while (player.exp >= 100) {
        player.exp -= 100;
        player.level++;
        cout << "🎉 Level Up! You are now Level " << player.level << "!\n";
    }
}

bool reduceStamina(int amount) {
    if (player.stamina < amount) {
        cout << "Not enough stamina! Current: " << player.stamina << "/100\n";
        return false;
    }
    player.stamina -= amount;
    cout << "Stamina -" << amount << " | Current: " << player.stamina << "/100\n";
    return true;
}

void displayBar(const string &label, int current, int max, int width = 20) {
    int filled = (current * width) / max;
    cout << "│  " << label << ": [";
    for (int i = 0; i < width; i++) {
        if (i < filled) cout << "■";  // filled part
        else cout << " ";              // empty part
    }
    cout << "] " << current << "/" << max << "\n";
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
        cout << "│  Player: " << player.name << "                           Level: " << player.level << "\n";
        displayBar("XP", player.exp, 100);
        displayBar("Stamina", player.stamina, 100);
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

        cout << "\nEnter option: ";
        cin >> option;

        switch(option) {
            case 1: questMenu(); break;
            case 2: cout << "Companion menu not implemented yet!\n"; break;
            case 3: statsMenu(); break;
            case 4: inventoryMenu(); break;
            case 5: 
                cout << "Exiting...\n"; 
                return;
            default: cout << "Invalid option!\n";
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
            case 3: sideQuestMenu(); break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 0);
}

//  DAILY QUESTS 
void dailyQuestsMenu() {
    int option;
    cin.ignore();

    do {
        cout << "\n────────── DAILY QUESTS ──────────\n";
        cout << "  1. Add Quest                    \n";
        cout << "  2. Complete Quest               \n";
        cout << "                                  \n";
        if (!dailyQuests.empty()) {
            cout << " Your Quests:                     \n";
            for (int i = 0; i < dailyQuests.size(); i++)
                cout << "  " << i + 1 << ". " << dailyQuests[i] << "\n";
        } else {
            cout << " - No quests yet -                \n";
        }
        cout << "                                  \n";
        cout << " 0. Back                          \n";
        cout << "──────────────────────────────────\n";
        cout << "Choose: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            string quest;
            cout << "Enter quest description: ";
            getline(cin, quest);
            dailyQuests.push_back(quest);
            cout << "Quest added!\n";
        } 
        else if (option == 2) {
            if (dailyQuests.empty()) cout << "No quests to complete!\n";
            else {
                int index;
                cout << "Enter quest number to complete: ";
                cin >> index;
                if (index > 0 && index <= dailyQuests.size()) {
                    if (reduceStamina(5)) gainEXP(10); // Daily quest: 5 stamina, 10 EXP
                    dailyQuests.erase(dailyQuests.begin() + (index - 1));
                    cout << "Quest completed!\n";
                } else cout << "Invalid quest number!\n";
            }
        }
    } while(option != 0);
}

//  SIDE QUESTS 
void displaySideQuests() {
    cout << "\n────────────── SIDE QUESTS ──────────────\n";
    if (sideQuestList.empty()) cout << " No side quests added yet.             \n";
    else {
        for (size_t i = 0; i < sideQuestList.size(); i++) {
            cout << " " << i + 1 << ". " << sideQuestList[i].name;
            if (sideQuestList[i].completed) cout << " ✔";
            cout << "\n";
        }
    }
    cout << " 0. Back                               \n";
    cout << "────────────────────────────────────────\n";
}

void sideQuestMenu() {
    int choice;
    cin.ignore();

    do {
        displaySideQuests();
        cout << "\n1. Add Side Quest\n2. Complete a Side Quest\nChoose: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string quest;
            cout << "Enter your side quest (hobby/project): ";
            getline(cin, quest);
            sideQuestList.push_back({quest,false});
            cout << "Side quest added!\n";
        } 
        else if (choice == 2) {
            if (sideQuestList.empty()) cout << "No side quests to complete!\n";
            else {
                int q;
                cout << "Enter number of quest completed: ";
                cin >> q;
                if (q > 0 && q <= sideQuestList.size()) {
                    if (reduceStamina(10)) gainEXP(25); // Side quest: 10 stamina, 25 EXP
                    cout << "Completed: " << sideQuestList[q-1].name << " ✅\n";
                    sideQuestList.erase(sideQuestList.begin() + (q-1));
                } else cout << "Invalid choice!\n";
            }
        }
    } while(choice != 0);
}

//  BOSS FIGHT 
void displayBoss(const Boss &boss) {
    cout << "\n══════════════ BOSS FIGHT ══════════════\n";
    cout << " 👹 " << boss.name << "\n";
    cout << " HP: [";

    // Display HP bar
    int barWidth = 30; // width of the HP bar
    int filled = (boss.currentHP * barWidth) / boss.maxHP;
    for (int i = 0; i < filled; i++) cout << "♥︎";      // filled portion
    for (int i = filled; i < barWidth; i++) cout << "♡"; // empty portion
    cout << "] " << boss.currentHP << "/" << boss.maxHP << "\n\n";

    // boss ASCII art
     cout << R"(
                          ,     \    /      ,
                         / \    )\__/(     / \
                        /   \  (_\  /_)   /   \
                   ____/_____\__\@  @/___/_____\____
                  |             |\../|              |
                  |              \VV/               |
                  |            MONSTER              |
                  |_________________________________|
                         |    /\ /      \ /\    |
                         |  /   V        V   \  |
                         |/                \|
                )";
    cout << "═══════════════════════════════════════\n";
}

void bossFight() {
    Boss boss = {"Shadow King", 100, 100};
    vector<string> subtasks;
    cin.ignore();

    while (boss.currentHP > 0) {
        displayBoss(boss);
        cout << "\n1. Add Attack Task\n2. Complete a Task (Attack Boss!)\n3. Run Away\nChoose: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string task;
            cout << "Enter an attack task: ";
            getline(cin, task);
            subtasks.push_back(task);
            cout << "Task added!\n";
        } 
        else if (choice == 2) {
            if (subtasks.empty()) cout << "No tasks to attack with!\n";
            else {
                int damagePerTask = boss.currentHP / subtasks.size();
                if (damagePerTask < 1) damagePerTask = 1;

                cout << "Choose task to complete:\n";
                for (size_t i = 0; i < subtasks.size(); i++)
                    cout << i + 1 << ". " << subtasks[i] << endl;

                int t;
                cin >> t;
                if (t>0 && t<=subtasks.size()) {
                    if (reduceStamina(10)) gainEXP(20); // Boss subtask: 10 stamina, 20 EXP
                    boss.currentHP -= damagePerTask;
                    if (boss.currentHP<0) boss.currentHP=0;
                    cout << "Dealt " << damagePerTask << " damage! Boss HP: " 
                         << boss.currentHP << "/" << boss.maxHP << "\n";
                    subtasks.erase(subtasks.begin()+t-1);
                } else cout << "Invalid task!\n";
            }
        } 
        else if (choice == 3) {
            cout << "You fled the fight...\n";
            return;
        } 
        else cout << "Invalid choice.\n";
    }

    cout << "\n🎉 YOU DEFEATED THE BOSS!\n";
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