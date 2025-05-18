# Network Game (UDP Auto-Discovery)

## 🔄  Logic Overview

This application uses a UDP connection. An instance of the application launched by the **first player** attempts to receive a broadcast message. If it doesn't receive one, it starts a **server** on a dynamically allocated port and begins sending out broadcast messages.

Once a broadcast message is received (even by itself), the same instance connects to the server as a **client**. The **second player**, upon launching the application, will receive this broadcast and simply connect as a client.

When the number of clients reaches **two**, the server stops broadcasting. Any additional player pairs will repeat this client-server initialization process similarly.

## ⚙️ Build Instructions

The build process is the same for both Linux and Windows systems.

### Step-by-Step

1. Open the `networkgame.pro` file in Qt Creator.
2. Click on **Configure Project**.
3. Build the project (Qt will invoke `qmake`, generate a Makefile, and then use `make` or an appropriate tool to build):

- On **Linux**: uses `make`.
- On **Windows**: use `nmake` (MSVC) or `mingw32-make` (MinGW tested).

### 🔧 Windows Deployment

After building the project on Windows:

Run the following command in any terminal:

```bash
windeployqt networkgame.exe
```

This command scans the executable and copies all required dynamic libraries (.dll), plugins, and Qt dependencies into the same directory as the built executable. After this, the application is ready to launch.

### ⚠️ Important Note

Be sure to delete the auto-generated `networkgame.pro.user` file **before transferring or rebuilding** on another system. This file stores build-specific configuration and can cause build issues due to version mismatches.

## 📁 Project structure

```
	networkgame/
	├── src/
	│ 
	├── include/
	|
	├── ui/
	|
	├── networkgame.pro
	|
	├── README.md
	|
	└── main.cpp
```
