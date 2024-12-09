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

int lastUserButtonValues[4] = {LOW, LOW, LOW, LOW};
int lastActionButtonValues[2] = {LOW, LOW};
user selectedUser;
action selectedAction;

// TODO: Assign pins
// 4 User buttons
const int userButtons[4] = {_, _, _, _}; 
// 2 Action buttons
const int actionButtons[2] = {_, _};
// 4 User Status LEDs
const int userLEDs[4] = {_, _, _, _}; 
// 2 Action Status LEDs
const int actionLEDs[2] = {_, _};