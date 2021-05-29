## Subsynth - A Digital Synthesizer Project

### Authors: Aaron Hudson, Andrew Wilson, Robin Su
---
CS 510: Computers, Sound and Music

Spring 2021 

Portland State University

---

### Project Goals
 - Implement basic digital sound processing techniques
 - Build a basic synthesizer with adjustable oscillators,  ADSR envelope and filters
 - Learn the JUCE framework

### What We Achieved

stuff, things

---
## Setup

- Install JUCE
- Clone repo
- Open the `.jucer` file with the Projucer
- Export files and launch IDE from inside Projucer - compatible with Visual Studio (Windows), XCode (MacOS) and Linux
- Build (if building on a Mac, see below for further details)

## Run

### Windows
- Navigate to `./Builds/YOUR EDITOR/x64/Debug/Standalone Plugin/`
- Launch `Subsynth` executable (or w/e it is on MacOS)

### MacOS
 - After exporting to XCode, be sure to select `Standalone Plugin` as the active scheme before building (the Projucer configures each project to build different target application types. For this project, we built a standalone plugin). 
 - If building the application does not automatically launch the plugin, navigate to `./Builds/MacOSX/build/Debug/` and double-click on `Subsynth`.