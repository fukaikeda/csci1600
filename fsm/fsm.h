typedef enum {
  User1 = 0,
  User2 = 1,
  User3 = 2,
  User4 = 3,
  None = -1
} User;

typedef enum {
  ReturnTime = 0,
  Message = 1,
  NoAction = -1
} Action;

typedef enum {
  sDisplayRealTime = 0,
  sWaitAfterUserBut = 1,
  sWaitAfterTimeBut = 2,
  sWaitAfterMessage = 3
} State;

// store everyone's return time 
// std::map<String, String> homeTimes = {
//     {"Mia", ""},
//     {"Fuka", ""},
//     {"Jason", ""},
//     {"Kana", "test"}
// };

const char* names[4] = {"Fuka", "Mia", "Kana", "Jason"};


const int userButtons[4] = {A1, A2, A3, A5}; // User buttons Do not change!!
const int actionButtons[2] = {2, 3};       // Action buttons
const int userLED = 12;                      // LED for user buttons
const int actionLED = 13;                    // LED for action buttons

// Include additional variables to store the triggered button
volatile User triggeredUserButton = User::None; // Variable to track the user button that triggered ISR
volatile Action triggeredActionButton = Action::NoAction; // Variable to track the action button that triggered ISR
