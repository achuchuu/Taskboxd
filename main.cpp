#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <ctime>

using namespace std;

// COMPANION TYPES
enum CompanionType {
    NONE = 0,
    BYTEBUN = 1,
    DATABUN = 2,
    MEGABUN = 3,
    SYSBUN = 4,
    GEARCUB = 5,
    GEARLING = 6,
    MECHABEAR = 7,
    CLOCKWORKBEAST = 8,
    PIXELLIMP = 9,
    SUB_PIXEL = 10,
    CHROMXEL = 11,
    MEGA_PIXEL = 12
};

// STRUCTS
struct Companion {
    CompanionType type = NONE;
    int level = 1;
    int exp = 0;
    int mood = 60;
    int energy = 5;
    int bondLevel = 1;
    int trust = 50;
    int evolutionStage = 0; // 0 = base, 1 = evolved, 2 = mega, 3 = legendary
};

struct Player {
    string name;
    int stamina = 100;
    string equippedTitle = "";
    
    bool sharpMind = false;
    bool bookworm = false;
    bool nightOwl = false;
    bool bossSlayer = false;
    
    int dailyTasksCompleted = 0;
    int studyStreak = 0;
    int lateTasksCompleted = 0;
    int bossesDefeated = 0;
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
Companion companion;
vector<string> dailyQuests;
vector<SideQuest> sideQuestList;
int lastDailyDate = 0;

// FUNCTION DECLARATIONS
void mainMenu();
void questMenu();
void dailyQuestsMenu();
void sideQuestMenu();
void bossFight();
void displayBar(const string &label, int current, int max, int width);
void displayBoss(const Boss &boss);
void gainCompanionEXP(int amount);
void equipTitle();
bool reduceStamina(int amount);
void COMPANIONMENU();
void checkEvolution();
void displayCompanion();
void saveData();
void loadData();

// CLEAR SCREEN
void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count, cellCount;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', cellCount, {0, 0}, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, {0, 0}, &count);
    SetConsoleCursorPosition(hConsole, {0, 0});
}

// SAVE AND LOAD
void saveData() {
    ofstream file("playerData.txt");
    if (!file) {
        cout << "Error saving data!\n";
        return;
    }

    // Player & companion info
    file << player.name << " "
         << player.stamina << " "
         << player.studyStreak << " "
         << player.dailyTasksCompleted << " "
         << lastDailyDate << " "
         << (int)companion.type << " "
         << companion.level << " "
         << companion.exp << " "
         << companion.mood << " "
         << companion.energy << " "
         << companion.bondLevel << " "
         << companion.trust << " "
         << companion.evolutionStage << "\n";

    // DAILY QUESTS
    file << dailyQuests.size() << " ";
    for (const auto& q : dailyQuests) {
        file << q << "|";  // separate each quest with |
    }
    file << "\n";

    // SIDE QUESTS
    file << sideQuestList.size() << " ";
    for (const auto& sq : sideQuestList) {
        file << sq.name << "," << sq.completed << "|"; // name,completed
    }
    file << "\n";

    file.close();
}

void loadData() {
    ifstream file("playerData.txt");
    if (!file) return; // no saved data yet

    string line;

    // Player & companion
    if (getline(file, line)) {
        istringstream ss(line);
        int compType;
        ss >> player.name
           >> player.stamina
           >> player.studyStreak
           >> player.dailyTasksCompleted
           >> lastDailyDate
           >> compType
           >> companion.level
           >> companion.exp
           >> companion.mood
           >> companion.energy
           >> companion.bondLevel
           >> companion.trust
           >> companion.evolutionStage;
        companion.type = static_cast<CompanionType>(compType);
    }

    // Daily quests
    if (getline(file, line)) {
        istringstream ss(line);
        size_t n;
        ss >> n;
        dailyQuests.clear();
        string quests;
        getline(ss, quests); // rest of line
        size_t pos = 0;
        while ((pos = quests.find('|')) != string::npos) {
            string quest = quests.substr(0, pos);
            if (!quest.empty()) dailyQuests.push_back(quest);
            quests.erase(0, pos + 1);
        }
    }

    // Side quests
    if (getline(file, line)) {
        istringstream ss(line);
        size_t n;
        ss >> n;
        sideQuestList.clear();
        string quests;
        getline(ss, quests);
        size_t pos = 0;
        while ((pos = quests.find('|')) != string::npos) {
            string sq = quests.substr(0, pos);
            if (!sq.empty()) {
                size_t comma = sq.find(',');
                string name = sq.substr(0, comma);
                bool completed = (sq.substr(comma + 1) == "1");
                sideQuestList.push_back({name, completed});
            }
            quests.erase(0, pos + 1);
        }
    }

    file.close();
}

int getCurrentDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;
}

void dailyResetCheck() {
    int today = getCurrentDate();
    if (today != lastDailyDate) {
        player.dailyTasksCompleted = 0;
        lastDailyDate = today;
        saveData();
    }
}

int safeInput() {
    int x;
    cin >> x;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number: ";
        cin >> x;
    }
    cin.ignore(1000, '\n');
    return x;
}

// COMPANION FUNCTIONS
void gainCompanionEXP(int amount) {
    if (companion.type == NONE) {
        cout << "No companion to gain EXP!\n";
        return;
    }

    companion.exp += amount;
    companion.mood = min(100, companion.mood + 5);
    companion.trust = min(100, companion.trust + 2);

    cout << "Your companion gained " << amount << " EXP!\n";

    while (companion.exp >= 100) {
        companion.exp -= 100;
        companion.level++;
        companion.bondLevel = (companion.level / 5) + 1;

        cout << "\n🎉 LEVEL UP! Your companion is now Level " << companion.level << "!\n";

        // Show appropriate level-up message for current companion type
        switch(companion.type) {
            // BYTEBUN family
            case BYTEBUN:  cout << "(\\_ /)\n ( •_•)✨  ByteBun leveled up!\n / >💾\n"; break;
            case DATABUN:  cout << "(\\_ /)\n ( •ᴗ•)⚡  Databun leveled up!\n / >💿\n"; break;
            case MEGABUN:  cout << "(\\_ /)\n ( •ω•)⚡  Megabun leveled up!\n / >💻\n"; break;
            case SYSBUN:   cout << "(\\_ /)\n ( ⚆_⚆)🌟  SysBun leveled up!\n / >🖥️\n"; break;

            // GEARCUB family
            case GEARCUB:     cout << "ʕ•ᴥ•ʔ⚙️  Gearcub leveled up!\n"; break;
            case GEARLING:    cout << "ʕ•̀ᴥ•́ʔ⚙️⚙️  Gearling leveled up!\n"; break;
            case MECHABEAR:   cout << "ʕ•̀ᴥ•́ʔ🔩⚙️  MechaBear leveled up!\n"; break;
            case CLOCKWORKBEAST: cout << "ʕ•̿ᴥ•̿ʔ⚡⚙️  Clockwork Beast leveled up!\n"; break;

            // PIXELLIMP family
            case PIXELLIMP:  cout << "(>*-*)>☆  Pixelimp leveled up!\n"; break;
            case SUB_PIXEL:  cout << "(>^-^)>★  Sub-Pixel leveled up!\n"; break;
            case CHROMXEL:   cout << "(>◕-◕)>✨  Chromxel leveled up!\n"; break;
            case MEGA_PIXEL: cout << "(>⚆-⚆)>💫  Mega-Pixel leveled up!\n"; break;

            default: break;
        }

        checkEvolution();
    }
}

void checkEvolution() {
    int oldStage = companion.evolutionStage;
    
    // Evolution based on level and streaks
    if (companion.type >= BYTEBUN && companion.type <= SYSBUN) {
        if (companion.level >= 25 && player.studyStreak >= 30) {
            companion.evolutionStage = 3;
            companion.type = SYSBUN;
        } else if (companion.level >= 15) {
            companion.evolutionStage = 2;
            companion.type = MEGABUN;
        } else if (companion.level >= 8) {
            companion.evolutionStage = 1;
            companion.type = DATABUN;
        }
    } else if (companion.type >= GEARCUB && companion.type <= CLOCKWORKBEAST) {
        if (companion.level >= 25 && player.bossesDefeated >= 5) {
            companion.evolutionStage = 3;
            companion.type = CLOCKWORKBEAST;
        } else if (companion.level >= 15) {
            companion.evolutionStage = 2;
            companion.type = MECHABEAR;
        } else if (companion.level >= 8) {
            companion.evolutionStage = 1;
            companion.type = GEARLING;
        }
    } else if (companion.type >= PIXELLIMP && companion.type <= MEGA_PIXEL) {
        if (companion.level >= 25 && companion.bondLevel >= 10) {
            companion.evolutionStage = 3;
            companion.type = MEGA_PIXEL;
        } else if (companion.level >= 15) {
            companion.evolutionStage = 2;
            companion.type = CHROMXEL;
        } else if (companion.level >= 8) {
            companion.evolutionStage = 1;
            companion.type = SUB_PIXEL;
        }
    }
    
    if (oldStage != companion.evolutionStage) {
        clearScreen();
        cout << "\n✨ EVOLUTION! ✨\n";
        cout << "Your companion has evolved!\n";
    }
}

void displayCompanion() {
    if (companion.type == NONE) {
        cout << "No companion yet!\n";
        return;
    }
    clearScreen();
    cout << "\n╔════════════ COMPANION ════════════╗\n";
    
    switch(companion.type) {
        case BYTEBUN:
            cout << "  (\\_ /)\n   ( •_•)\n   / >💾\n";
            cout << "  Bytebun (Starter)\n";
            break;
        case DATABUN:
            cout << "  (\\_ /)\n   ( •ᴗ•)✨\n   / >💿\n";
            cout << "  Databun (Evolved)\n";
            break;
        case MEGABUN:
            cout << "  (\\_ /)\n   ( •ω•)⚡\n   / >💻\n";
            cout << "  Megabun (Mega)\n";
            break;
        case SYSBUN:
            cout << "  (\\_ /)\n   ( ⚆_⚆)🌟\n   / >🖥️\n";
            cout << "  SysBun (LEGENDARY)\n";
            break;
        case GEARCUB:
            cout << "  ʕ•ᴥ•ʔ⚙️\n";
            cout << "  Gearcub (Starter)\n";
            break;
        case GEARLING:
            cout << "  ʕ•̀ᴥ•́ʔ⚙️⚙️\n";
            cout << "  Gearling (Evolved)\n";
            break;
        case MECHABEAR:
            cout << "  ʕ•̀ᴥ•́ʔ🔩⚙️\n";
            cout << "  MechaBear (Mega)\n";
            break;
        case CLOCKWORKBEAST:
            cout << "  ʕ•̿ᴥ•̿ʔ⚡⚙️\n";
            cout << "  Clockwork Beast (LEGENDARY)\n";
            break;
        case PIXELLIMP:
            cout << "  (>*-*)>☆\n";
            cout << "  Pixelimp (Starter)\n";
            break;
        case SUB_PIXEL:
            cout << "  (>^-^)>★\n";
            cout << "  Sub-Pixel (Evolved)\n";
            break;
        case CHROMXEL:
            cout << "  (>◕-◕)>✨\n";
            cout << "  Chromxel (Mega)\n";
            break;
        case MEGA_PIXEL:
            cout << "  (>⚆-⚆)>💫\n";
            cout << "  Mega-Pixel (LEGENDARY)\n";
            break;
    }
    
    cout << "\n  Level: " << companion.level << "\n";
    displayBar("  EXP", companion.exp, 100, 15);
    displayBar("  Mood", companion.mood, 100, 15);
    displayBar("  Trust", companion.trust, 100, 15);
    cout << "  Energy: " << companion.energy << "/5\n";
    cout << "  Bond Level: " << companion.bondLevel << "\n";
    cout << "╚═══════════════════════════════════╝\n";
}

bool reduceStamina(int amount) {
    clearScreen();
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
    cout << label << ": [";
    for (int i = 0; i < width; i++) {
        if (i < filled) cout << "■";
        else cout << "□";
    }
    cout << "] " << current << "/" << max << "\n";
}

// MAIN MENU
void mainMenu() {
    int option;
    
    while (true) {
        clearScreen();
        
        cout << "┌─────────────────────°❀⋆.ೃ࿔*:･°❀⋆.ೃ࿔*:─────────────────────┐\n";
        cout << "│                                                          │\n";
        cout << "│                      T A S K B O X D                     │\n";
        cout << "│                \"Gamify your Study Grind!\"                │\n";
        cout << "│                                                          │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        cout << "│                                                          │\n";
        cout << "│  Player: " << player.name << "                                        │\n";
        displayBar("│  Stamina", player.stamina, 100);
        
        if (companion.type != NONE) {
            cout << "│                                                          │\n";
            cout << "│  Companion Level: " << companion.level << "                                  │\n";
            displayBar("│  Companion EXP", companion.exp, 100);
        }
        
        cout << "│                                                          │\n";
        cout << "│----------------------------------------------------------│\n";
        cout << "│                                                          │\n";
        cout << "│ ✦ Quick Actions . ˚₊ ⊹ . ˚˖ . ˚                             │\n";
        cout << "│                                                          │\n";
        cout << "│   ➀ Quests               ➁ Companion                     │\n";
        cout << "│   ➂ Exit                                                 │\n";
        cout << "│                                                          │\n";
        cout << "└──────────────────────────────────────────────────────────┘\n";
        
        cout << "\nEnter option: ";
        option = safeInput();
        
        switch(option) {
            case 1: questMenu(); break;
            case 2: COMPANIONMENU(); break;
            case 3:                 
                cout << "Exiting...\n";
                saveData();
                return;


            default: cout << "Invalid option!\n";
        }
    }
}

// QUEST MENU
void questMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "\n┌──────────────────── QUESTS ────────────────────┐\n";
        cout << "│  1. Daily Quests                               │\n";
        cout << "│  2. Boss Fights                                │\n";
        cout << "│  3. Side Quests                                │\n";
        cout << "│                                                │\n";
        cout << "│  0. Back                                       │\n";
        cout << "└────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        choice = safeInput();
        
        switch(choice) {
            case 1: dailyQuestsMenu(); break;
            case 2: bossFight(); break;
            case 3: sideQuestMenu(); break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 0);
}

// DAILY QUESTS
void dailyQuestsMenu() { 
    int option;
    
    do {
        clearScreen();
        cout << "\n─────────── DAILY QUESTS ───────────\n";
        cout << "  1. Add Quest                    \n";
        cout << "  2. Complete Quest               \n";
        cout << "                                  \n";
        if (!dailyQuests.empty()) {
            cout << " Your Quests:                     \n";
            for (size_t i = 0; i < dailyQuests.size(); i++)
                cout << "  " << i + 1 << ". " << dailyQuests[i] << "\n";
        } else {
            cout << " - No quests yet -                \n";
        }
        cout << "                                  \n";
        cout << " 0. Back                          \n";
        cout << "──────────────────────────────────\n";
        cout << "Choose: ";
        option = safeInput();
        
        if (option == 1) {
            string quest;
            cout << "Enter quest description: ";
            getline(cin, quest);
            dailyQuests.push_back(quest);
            cout << "Quest added!\n";
            saveData(); // Save after adding quest
        } 
        else if (option == 2) {
            if (dailyQuests.empty()) {
                cout << "No quests to complete!\n";
            } else {
                int index;
                cout << "Enter quest number to complete: ";
                index = safeInput();
                if (index > 0 && index <= dailyQuests.size()) {
                    if (reduceStamina(5)) {
                        gainCompanionEXP(10); // Companion gains EXP
                        dailyQuests.erase(dailyQuests.begin() + (index - 1));
                        cout << "Quest completed!\n";
                        
                        int today = getCurrentDate();
                        if (today != lastDailyDate) {
                            if (today == lastDailyDate + 1) {
                                player.studyStreak++;
                            } else {
                                player.studyStreak = 1;
                            }
                            lastDailyDate = today;
                            player.dailyTasksCompleted = 0;
                            cout << "🌟 Daily streak: " << player.studyStreak << " day(s)!\n";
                        }
                        
                        player.dailyTasksCompleted++;
                        saveData(); // Save after completing quest and updating stats
                    }
                } else {
                    cout << "Invalid quest number!\n";
                }
            }
        }
        
        if (option != 0) {
            cout << "Press Enter to continue...";
            cin.get();
        }
        
    } while(option != 0);
}

// SIDE QUESTS
void sideQuestMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "\n─────────────── SIDE QUESTS ───────────────\n";
        if (sideQuestList.empty()) {
            cout << " No side quests added yet.             \n";
        } else {
            for (size_t i = 0; i < sideQuestList.size(); i++) {
                cout << " " << i + 1 << ". " << sideQuestList[i].name;
                if (sideQuestList[i].completed) cout << " ✓";
                cout << "\n";
            }
        }
        cout << " 0. Back                               \n";
        cout << "────────────────────────────────────────\n";
        cout << "\n1. Add Side Quest\n2. Complete a Side Quest\nChoose: ";
        choice = safeInput();
        
        if (choice == 1) {
            string quest;
            cout << "Enter your side quest (hobby/project): ";
            getline(cin, quest);
            sideQuestList.push_back({quest, false});
            cout << "Side quest added!\n";
            saveData(); // Save after adding side quest
        } 
        else if (choice == 2) {
            if (sideQuestList.empty()) {
                cout << "No side quests to complete!\n";
            } else {
                int q;
                cout << "Enter number of quest completed: ";
                q = safeInput();
                if (q > 0 && q <= sideQuestList.size()) {
                    if (reduceStamina(10)) {
                        gainCompanionEXP(25);
                        cout << "Completed: " << sideQuestList[q-1].name << " ✅\n";
                        sideQuestList.erase(sideQuestList.begin() + (q-1));
                        saveData(); // Save after completing side quest
                    }
                } else {
                    cout << "Invalid choice!\n";
                }
            }
        }
        
        if (choice != 0) {
            cout << "Press Enter to continue...";
            cin.get();
        }
        
    } while(choice != 0);
}

// BOSS FIGHT
void displayBoss(const Boss &boss) {
    clearScreen();
    cout << "\n╔══════════════ BOSS FIGHT ══════════════╗\n";
    cout << " 👹 " << boss.name << "\n";
    cout << " HP: [";
    
    int barWidth = 30;
    int filled = (boss.currentHP * barWidth) / boss.maxHP;
    for (int i = 0; i < filled; i++) cout << "♥︎";
    for (int i = filled; i < barWidth; i++) cout << "♡";
    cout << "] " << boss.currentHP << "/" << boss.maxHP << "\n\n";
}

void bossFight() {
    Boss boss = {"Final Exam Monster", 100, 100};
    vector<string> subtasks;
    
    while (boss.currentHP > 0) {
        displayBoss(boss);
        cout << "\n1. Add Attack Task\n2. Complete a Task (Attack Boss!)\n3. Run Away\nChoose: ";
        int choice = safeInput();
        
        if (choice == 1) {
            string task;
            cout << "Enter an attack task: ";
            getline(cin, task);
            subtasks.push_back(task);
            cout << "Task added!\n";
        } else if (choice == 2) {
            if (subtasks.empty()) {
                cout << "No tasks to attack with!\n";
            } else {
                int damagePerTask = boss.currentHP / subtasks.size();
                if (damagePerTask < 1) damagePerTask = 1;
                
                cout << "Choose task to complete:\n";
                for (size_t i = 0; i < subtasks.size(); i++)
                    cout << i + 1 << ". " << subtasks[i] << endl;
                
                int t = safeInput();
                if (t > 0 && t <= subtasks.size()) {
                    if (reduceStamina(10)) {
                        gainCompanionEXP(20);
                        boss.currentHP -= damagePerTask;
                        if (boss.currentHP < 0) boss.currentHP = 0;
                        cout << "Dealt " << damagePerTask << " damage! Boss HP: "
                             << boss.currentHP << "/" << boss.maxHP << "\n";
                        subtasks.erase(subtasks.begin() + t - 1);
                    }
                } else {
                    cout << "Invalid task!\n";
                }
            }
        } else if (choice == 3) {
            cout << "You fled the fight...\n";
            return;
        } else {
            cout << "Invalid choice.\n";
        }
        
        if (choice != 0) {
            cout << "Press Enter to continue...";
            cin.get();
        }
    }
    
    clearScreen();
    player.bossesDefeated++;
    cout << "\n🎉 YOU DEFEATED THE BOSS!\n";
    gainCompanionEXP(50); // Big bonus for boss!
    cout << "\nPress Enter to continue...";
    cin.get();
}

// COMPANION MENU
void COMPANIONMENU() {
    int companion_option;
    
    do {
        clearScreen();
        
        if (companion.type != NONE) {
            displayCompanion();
        }
        
        cout << "\n┌────────────────────COMPANION────────────────────┐\n";
        cout << "│  1. Pick a Companion                           │\n";
        cout << "│  2. View Companion Details                     │\n";
        cout << "│  3. Replace Companion                          │\n";
        cout << "│                                                │\n";
        cout << "│  0. Back                                       │\n";
        cout << "└────────────────────────────────────────────────┘\n";
        cout << "Choose an option: ";
        companion_option = safeInput();
        
        switch(companion_option) {
            case 1: {
                if (companion.type != NONE) {
                    cout << "You already have a companion!\n";
                } else {
                    clearScreen();
                    cout << "Choose who you want to grind with everyday!\n";
                    cout << "1. ByteBun (Cheerful, loves streaks!)\n";
                    cout << "2. Gear Cub (Hardworking, thrives in challenges!)\n";
                    cout << "3. Pixel Limp (Mischievous, random bonuses!)\n";
                    cout << "0. Cancel\n";
                    
                    int choice = safeInput();
                    
                    switch(choice) {
                        case 1:
                        clearScreen();
                            companion.type = BYTEBUN;
                            companion.level = 1;
                            companion.exp = 0;
                            companion.mood = 60;
                            companion.energy = 5;
                            companion.bondLevel = 1;
                            companion.trust = 50;
                            cout << "You picked ByteBun! 🐰💻\n";
                            cout << "(\\_ /)\n ( •_•)\n / >💾\n";
                            cout << "Yay! Let's raise bytes everyday!\n";
                            break;
                        case 2:
                        clearScreen();
                            companion.type = GEARCUB;
                            companion.level = 1;
                            companion.exp = 0;
                            companion.mood = 50;
                            companion.energy = 5;
                            companion.bondLevel = 1;
                            companion.trust = 50;
                            cout << "You picked Gear Cub! 🧸⚙️\n";
                            cout << "ʕ•ᴥ•ʔ⚙️\n";
                            cout << "Grr! My gears never stop!\n";
                            break;
                        case 3:
                        clearScreen();
                            companion.type = PIXELLIMP;
                            companion.level = 1;
                            companion.exp = 0;
                            companion.mood = 40;
                            companion.energy = 5;
                            companion.bondLevel = 1;
                            companion.trust = 50;
                            cout << "You picked Pixel Limp! �🎮\n";
                            cout << "(>*-*)>☆\n";
                            cout << "Hehe, let's have fun!\n";
                            break;
                        case 0:
                            break;
                        default:
                            cout << "Invalid choice!\n";
                    }
                }
                break;
            }
            case 2:
                if (companion.type == NONE) {
                    cout << "No companion selected yet!\n";
                } else {
                    displayCompanion();
                }
                break;
            case 3:
                if (companion.type == NONE) {
                    cout << "You don't have a companion yet!\n";
                } else {
                    clearScreen();
                    cout << "Are you sure you want to replace your companion? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore();
                    if (confirm == 'y' || confirm == 'Y') {
                        companion = Companion();
                        cout << "Companion removed.\n";
                    } else {
                        cout << "Cancelled.\n";
                    }
                }
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice!\n";
        }
        
        if (companion_option != 0) {
            cout << "Press Enter to continue...";
            cin.get();
        }
        
    } while(companion_option != 0);
}

int main() {
    // Open a new console window
    AllocConsole();
     // Set UTF-8 code page
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    // Redirect standard I/O to the new console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);

    

    std::cout << "Welcome to TaskBox!\n";

    // Your existing program logic
    loadData();
    dailyResetCheck();
    if (player.name.empty()) {
        std::cout << "Enter your name: ";
        std::getline(std::cin, player.name);
    }
    mainMenu();

    system("pause");
    return 0;
}