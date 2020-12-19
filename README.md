# Micro-transaction Service (Socket Programming)

# Environment

---

Ubuntu 20.04.1 LTS ( on Windows 10 WSL )

# How to compile

---

In terminal, under the file directory, 

1. Type in ***make*** command
2. If success, the ***client*** binary file should appear in the same directory

# How to execute

---

In terminal, after successful compilation,

1. Type in ***./client*** command
2. Follow the instructions and enter the desired IP address & port number
3. Register / Login interface should appear on the screen

# Features

- Register / Login State
    - Login
    - Register ( no account? register now )
- Application Interface State
    - Check Balances
        - Query server for several information
    - Microtransaction
        - Allow user to establish a p2p connection and communicate
    - Logout

# System Design

---

The system implemented a **Finite State Machine** which is used to control the states. The states include Register / Login as well as Application Interface, which can be simply recognized as before / after login states.

The Finite State Machine approach is proper for this application for the following reason,

1. The application requires different parts to be shown to the user during different phrases, for example logged in / logged out interface should be different. The FSM can control this simply by state transition.
2. If any additional functions shall be added to the application, the developers can simply add another state to the FSM, well-define the transitions, and boom, new function ready to go.

# Class Diagram

---

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/72012820-e4bd-4c6e-98fd-e7facc4ae0d4/Untitled.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/72012820-e4bd-4c6e-98fd-e7facc4ae0d4/Untitled.png)
