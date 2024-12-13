# Smart Clock: CSCI1600 Final Project

Authors: Fuka Ikeda, Yanmi Yu, Jason Ni, Kana Takizawa

<img width="732" alt="image" src="https://github.com/user-attachments/assets/af51952e-aaba-48e1-a6e7-a60e96708297" />


## Overview

The Smart Clock bridges the digital and physical worlds to connect housemates. This physical clock integrates Google Calendar (GCal) API notifications and real-time updates via WiFi. It offers two main functions:

Return Time Display: Select a housemate and press the display button. The clock’s hands move to indicate when the selected person is expected to return home based on their GCal events.
Anonymous Encouragement: Send a randomized, encouraging message via email to a selected housemate by pressing the message button.

In normal operation, the clock displays the current time, updating through a real-time clock (RTC) module.


## System Requirements

### Hardware Components:
Buttons: 4 user selection buttons, 1 display-return-time button, 1 message button

LEDs: Green indicates selected user, Red indicates action in progress

Stepper Motors: Drive hour and minute hands of the clock

WiFi Module: Establishes API connectivity

RTC Module: Provides accurate current time

### Software Dependencies:

Arduino Libraries:
rtclib: For real-time clock operations
stepper: For motor control

## Features

### User Interface:

User Selection: Select housemates using individual buttons.

Status LEDs: Indicate selected user and action in progress.

Multi-Action Buttons: Press the message or display-return-time button for specific functions after selecting a housemate.

GCal API Integration: Fetches scheduled events labeled "Home Arrival" from housemates' Google Calendars. Retrieves the start time of the next relevant event within 24 hours.

Physical Display: The clock transitions smoothly between real-time display and selected return times. The hands automatically reset to real-time display after 10 seconds.

Notifications: Sends a random, encouraging message via email to the selected housemate.

## File Description

fsm.ino: Main code file, implementing functionality and finite state machine logic.

Modules: Each component (e.g., buttons, LEDs, motors) is modularized for clarity. Inline comments explain transitions and method purposes.

## How to Download and Run

1. Clone Repository:

```

git clone https://github.com/fukaikeda/csci1600.git

cd csci1600/fsm

```

2. Install Required Libraries:

Open the Arduino IDE and install the **rtclib** and **stepper** libraries via Library Manager.

3. Load Code:

Open fsm.ino in Arduino IDE and upload it to your microcontroller.

4. Connect Hardware:

Ensure all components (buttons, LEDs, motors) are connected as described in the system architecture.

