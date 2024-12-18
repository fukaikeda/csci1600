// #define MOCK
// #define RUN_TESTS

#include "fsm.h"
#include "mock.h"

/*
 * Structure to represent the FSM state variables.
 * Tracks the current state, saved clock time, and user-triggered inputs
 * (user button and action button).
 */
typedef struct {
  State curState;
  long savedClock;
  User triggeredUserButton;
  Action triggeredActionButton;
} FSM_StateVars;

/*
 * Structure to represent FSM input variables.
 * Contains simulated inputs like the system time (millis), user button presses,
 * and action button presses.
 */
typedef struct {
  long millis;
  User userButton;
  Action actionButton;
} FSM_Inputs;

bool testFSMTransition(FSM_StateVars startStateVars, FSM_StateVars expectedStateVars, FSM_Inputs inputs, bool verbose);
const char* stateToString(State s);
void runAllTests();

/**
 * Converts an FSM state to a human-readable string.
 * 
 * Inputs: s The FSM state to convert.
 * Outputs: const char* Returns a string representation of the FSM state.
 */
const char* stateToString(State s) {
  switch (s) {
    case sDisplayRealTime: return "sDisplayRealTime";
    case sWaitAfterUserBut: return "sWaitAfterUserBut";
    case sWaitAfterTimeBut: return "sWaitAfterTimeBut";
    case sWaitAfterMessage: return "sWaitAfterMessage";
    default: return "Unknown State";
  }
}

#ifdef RUN_TESTS
/*
 * Runs all defined FSM test cases.
 * 
 * Executes a series of predefined FSM state transition tests, verifying correct
 * behavior under various input scenarios.
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting FSM tests...");
  runAllTests();
}

void loop() {
}

#endif

/*
 * Tests a single FSM transition and verifies the output.
 * 
 * Applies the provided inputs to the FSM starting from the given state variables,
 * executes the FSM update, and validates the output against the expected state.
 * 
 * Inputs:
 * startStateVars Initial FSM state variables.
 * expectedStateVars Expected FSM state variables after the transition.
 * inputs Simulated input variables.
 * verbose Enables detailed serial output for debugging.
 * Outputs:
 * bool Returns true if the test passes, false otherwise.
 */
bool testFSMTransition(FSM_StateVars startStateVars, FSM_StateVars expectedStateVars, FSM_Inputs inputs, bool verbose) {
  // Set initial state variables
  State curState = startStateVars.curState;
  savedClock = startStateVars.savedClock;
  triggeredUserButton = startStateVars.triggeredUserButton;
  triggeredActionButton = startStateVars.triggeredActionButton;

  // Apply inputs
  if (startStateVars.triggeredUserButton == User::None) triggeredUserButton = inputs.userButton;
  if (startStateVars.triggeredActionButton == Action::NoAction) triggeredActionButton = inputs.actionButton;

  // Call updateFSM
  State resultState = updateFSM(curState, inputs.millis);

  // Compare resulting state variables
  bool passed = (resultState == expectedStateVars.curState &&
                 savedClock == expectedStateVars.savedClock &&
                 triggeredUserButton == expectedStateVars.triggeredUserButton &&
                 triggeredActionButton == expectedStateVars.triggeredActionButton);

  if (verbose) {
    if (passed) {
      Serial.print("Test PASSED: ");
      Serial.println(stateToString(curState));
    } else {
      Serial.println("Test FAILED!");
      Serial.print("Start State: ");
      Serial.println(stateToString(startStateVars.curState));
      Serial.print("End State Expected: ");
      Serial.println(stateToString(expectedStateVars.curState));
      Serial.print("End State Actual: ");
      Serial.println(stateToString(resultState));
      Serial.println();
    }
  }

  return passed;
}

/*
 * Runs all FSM tests and validates expected behavior.
 * 
 * Defines multiple FSM test cases with initial and expected states, applies
 * inputs, and verifies correct state transitions.
 * Inputs: None
 * Outputs: None
 */
void runAllTests() {
  // Define test cases
  FSM_StateVars startStates[] = {
    {sDisplayRealTime, 0, User::None, Action::NoAction},
    {sWaitAfterUserBut, 1000, User::User1, Action::NoAction},
    {sWaitAfterTimeBut, 1000, User::User1, Action::ReturnTime},
    {sDisplayRealTime, 22000, User::None, Action::NoAction},
    {sWaitAfterUserBut, 23000, User::User2, Action::NoAction},
    {sWaitAfterMessage, 24000, User::User2, Action::Message}
  };

  FSM_StateVars expectedStates[] = {
    {sWaitAfterUserBut, 0, User::User1, Action::NoAction},
    {sWaitAfterTimeBut, 1000, User::User1, Action::ReturnTime},
    {sDisplayRealTime, 22000, User::None, Action::NoAction},
    {sWaitAfterUserBut, 23000, User::User2, Action::NoAction},
    {sWaitAfterMessage, 24000, User::User2, Action::Message},
    {sDisplayRealTime, 30000, User::None, Action::NoAction},
  };

  FSM_Inputs inputs[] = {
    {0, User::User1, Action::NoAction},
    {1000, User::None, Action::ReturnTime},
    {22000, User::None, Action::Message},
    {23000, User::User2, Action::NoAction},
    {24000, User::User3, Action::Message},
    {30000, User::User3, Action::Message}
  };

  int numTests = sizeof(startStates) / sizeof(startStates[0]);
  for (int i = 0; i < numTests; i++) {
    Serial.print("Running test ");
    Serial.println(i + 1);
    bool result = testFSMTransition(startStates[i], expectedStates[i], inputs[i], true);
    if (!result) {
      Serial.println("Test failed! Halting...");
      while (true);
    }
  }

  Serial.println("All tests passed!");
}
