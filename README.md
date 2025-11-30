# Penance

A cross-platform C++ game project using [raylib](https://www.raylib.com/).
Built with **CMake** and works on **Linux, macOS, and Windows**.

---

## Project Structure
```
Penance/
│── CMakeLists.txt       \# Build configuration
│── src/                 \# Game source code (.cpp, .hpp)
│── resources/           \# Game resources (images, audio, fonts etc.)
```
---

## Getting Started

### 1. Clone the repo

```sh
git clone https://github.com/Husnain56/Penance.git
cd Penance
```

-----

## Build & Run Instructions

### 🐧 Linux (via Terminal)

**Prerequisites:**
Install the necessary libraries for your distribution:

  * **Ubuntu / Debian:**

    ```sh
    sudo apt install build-essential git cmake libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
    ```

  * **Arch Linux:**

    ```sh
    sudo pacman -S base-devel git cmake alsa-lib mesa libx11 libxrandr libxi libxinerama libxcursor glu
    ```

**How to Run:**
We have a `Makefile` to automate everything. Just run:

```sh
make
```

*This will automatically configure CMake, compile the game using all CPU cores, and launch it.*

**Other Commands:**

  * `make clean` - Removes the build folder.
  * `make rebuild` - Cleans and compiles from scratch.

-----

### 🪟 Windows (via IDE)

We do not use the command line on Windows. Use your preferred Editor/IDE.

#### **Option A: Visual Studio**

1.  Open Visual Studio.
2.  Select **"Open a Local Folder"** and choose the `Penance` folder.
3.  Wait a moment for Visual Studio to detect `CMakeLists.txt` (watch the output bar at the bottom).
4.  Find the **Startup Item** dropdown in the top toolbar (it might say "Select Startup Item").
5.  Select **Penance.exe** from that list.
6.  Click the green **Play (▶)** button to build and run.

#### **Option B: VS Code**

*Requirement: Install the [C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extensions.*

1.  Open the `Penance` folder in VS Code.
2.  If asked to configure the project, choose your compiler (e.g., `Visual Studio Community 2022 Release - amd64`).
3.  Look at the **blue status bar** at the very bottom of the window.
4.  Click the **[Select Launch Target]** button (or the brackets `[]` if no target is selected) and choose `Penance`.
5.  Click the **Play (▶)** icon located **in that same bottom status bar** to run the game.
      * *Note: Do not press F5 unless you have manually configured a launch file. Use the bottom bar button.*

-----

## Tech Stack

  * **Language:** C++17
  * **Graphics/Audio:** raylib 5.5
  * **Build System:** CMake
