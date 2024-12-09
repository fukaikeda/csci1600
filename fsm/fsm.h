typedef enum {
  user1 = 0,
  user2 = 1,
  user3 = 2,
  user4 = 3,
  none = 4
} user;

typedef enum {
  returnTime = 0,
  message = 1,
  noAction = 2
} action;

typedef enum {
  sDisplayRealTime = 0,
  sWaitAfterUserBut = 1,
  sWaitAfterTimeBut = 2,
  sWaitAfterMessage = 3
} state;

// store everyone's return time 
std::map<String, String> homeTimes = {
    {"Mia", ""},
    {"Fuka", ""},
    {"Jason", ""},
    {"Kana", "test"}
};

const int userButtons[4] = {A1, A2, A3, A5}; // User buttons Do not change!!
const int actionButtons[2] = {2, 3};       // Action buttons
const int userLED = 12;                      // LED for user buttons
const int actionLED = 13;                    // LED for action buttons

// Include additional variables to store the triggered button
volatile int triggeredUserButton = -1; // Variable to track the user button that triggered ISR
static int triggeredActionButton = -1; // Variable to track the action button that triggered ISR

void initButtons();