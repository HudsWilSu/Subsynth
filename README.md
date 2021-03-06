## Subsynth - A Digital Synthesizer Project

### Authors: 
Andrew Wilson (anwils2@pdx.edu) <br> 
Robin Su (robisu@pdx.edu) <br>
Aaron Hudson (ahuds2@pdx.edu) <br>

---
### CS 510: Computers, Sound and Music

Professor Bart Massey <br>
Spring 2021 <br>
Portland State University

---
<img src="./images/Subsynth.png" width="600" />

### Project Description

This project contains code to generate a subtractive synthesizer plug-in, built in C++ using the JUCE application framework.  The synthesizer is polyphonic, allowing six voices to play simultaneously across 10 octaves, and has a graphical user interface containing the following components:

- Interactive On-screen MIDI keyboard
  - receives input from mouse, keyboard, or external MIDI controller
- Waveform Selector
  - choice of four waveform oscillators: sine, square, saw, or triangle
- ADSR Volume Envelope
  - allows the user to set the attack, decay, sustain, and release (range 0.0 to 1.0 for each)
- Variable State Filter
  - choice of three filter types: low-pass, band-pass, or high-pass
  - allows the user to set the resonance (range 1.0 to 5.0) and cutoff frequency (range 0 to 20,000 Hz)
- Waveform Visualizer
  - displays a visual depiction of waveforms being generated
- Gain Dial
  - allows the user to set desired gain (range -50 to 0 dB) 


### Basic Project Goals
 - Implement basic digital sound processing techniques
 - Build a basic synthesizer with the following properties:
   - monophonic
   - singular waveform oscillator (sine or square)
   - variable low-pass filter
   - ADSR envelope
   - Barebones UI
   - Basic input for 7 main musical notes (A, B, C, D, E, F, and G)
 - Learn the JUCE framework

### Stretch Goals
- Add the following features to the synthesizer:
  - polyphonic
  - multiple waveform oscillators (sine, square, saw, triangle)
  - low-pass filter with resonance parameter
  - low frequency oscillator attached to filter cutoff
  - MIDI input
  - full 12 note octave support

### What We Achieved

We were able to accomplish all of our initial goals outlined above, along with implementing many of the stretch goals we identified.  Furthermore, we expanded upon many of these stretch goals to deliver a final product allowing much greater customization of the sound being generated.

### How It Went

The JUCE framework had a large barrier of entry to overcome, as it provides a very large and complex toolkit that requires a high level of understanding to utilize.  Thankfully, the documentation for JUCE is very thorough, and there were many helpful tutorials to access when learning about different capabilities.  Furthermore, we had weekly meetings to go over what each of us accomplished over the prior week, and these sessions often consisted of group programming to help debug any issues blocking progress forward.  Overall, this project was challenging due to us being rusty with C++, while also learning about digital audio processing concepts as we went, but our group was very good about helping each other out as necessary and problem solving issues together.

### How We Tested

Our testing plan consisted of two main methods:
- Unit Testing:
  
  We wrote a testing suite to verify the basic components of our plug-in functioned as expected.  Specifically, we focused on testing that changes to the oscillator, ADSR envelope, and filter returned the expected results.
  
- Output Comparison Testing:
  
  We also analyzed the output of our plug-in using visualization tools and compared the results against outputs known to perform similar effects to ensure our synthesizer was modifying the generated sound as expected. 
  
Spectrum of non-filtered C4 square wave:
![](./images/C4_square_wave.png)

Spectrum with Subsynth low pass filter at 1 kHz:
![](./images/Subsynth_LP_filter_1kHz.png)

Spectrum with Ableton Live Auto Filter low pass filter at 1 kHz:
![](./images/Ableton_AutoFilter_LP_1kHz.png)

### Additional Information
What worked?
 - Our application can launch as a functional synthesizer plugin (standalone or VST3/AU) that can be played both with the on-screen MIDI keyboard or an external MIDI controller.
 - The oscillator, ADSR envelope, and filter are all responsive to corresponding adjustments on the user interface with little delay. 

What didn't?
 - Testing was a challenge for this project. JUCE is a robust framework that adds layers of abstraction to the raw data in order to simplify aspects of building the application, which had the effect of making the complete data flow difficult to follow. We would have needed to build a fully functioning plug-in host in order to perform comprehensive testing of our implementation. We instead did what we could to perform tests that run within the plug-in to verify certain behavior.

How satisfied are you with the result?
 - We found the process of developing this project very satisfying, and we are happy with our end product.  Working on this project offered an immense learning opportunity that coincided nicely with taking the course, as we got to experiment with and implement many of the features referenced in lecture.  Additionally, we gained experience in developing a plug-in, which not all of us had before this project.  Overall, we feel that our end product meets and exceeds our initial expectations, and it was very exciting getting to move past our basic outline and implement many of our stretch goals. 

What would you like to improve in the future?
 - Adding additional oscillators (both waveform generators and low frequency for parameter dynamics) would be a good enhancement to this project. This would allow for greater variety of timbres and effects to each voice in the synthesizer. 

---
## Build Requirements

- [JUCE](https://juce.com/get-juce)
- JUCE compatible IDE/build system:
  - Windows:
    - Visual Studio, Code::Blocks
  - MacOS:
    - XCode
  - Linux:
    - Makefile, Code::Blocks

## Setup

- Install JUCE
- Clone repo
- Open the `.jucer` file with the Projucer
- Export files and launch IDE from inside Projucer

> **_NOTE:_** We tested and built this app primarily as a standalone plugin, but it can also be built as a VST3 or AU (MacOS only) plug-in. See below for details on build instructions for each platform. 

## Build/Run

 > **_NOTE:_** There is a [known error](https://forum.juce.com/t/ms-visual-studio-16-9-4-compile-failed-after-vs-update/45555) in Visual Studio that produces a build error MSB3025 regarding the `\Shared Code\` directory. This has been fixed in the latest JUCE `develop`, but this project was based on the current `master` release. Standalone and VST3/AU builds are not affected.
 >
 > Additionally there are a number of C++ warnings (depending on compilier flags) related to JUCE framework/module code. As we cannot affect this, we simply tried to eliminate all warnings as they pertained to the code we wrote for this project.

#### Windows
  - To build (by default Visual Studio builds all build targets):
    1. Select `Release` from the Solutions Configuration dropdown in the lower toolbar
    2. From the menu bar, navigate to Build > Build Solution
  - To run the built plugin (standalone):
    1. Navigate to `./Builds/YOUR EDITOR/x64/Release/Standalone Plugin/`
    2. Launch `Subsynth` executable
  - To run the built plugin (VST3):
    1. Navigate to `./Builds/YOUR EDITOR/x64/Release/VST3/`
    2. Find the `.vst3` file and load into your plug-in host of choice.
  - To build and run with debugging
    1. From the menu bar, navigate to Debug > Start Debugging

#### MacOS
 - To run as a standalone plugin (release version):
    1. From the menu bar, navigate to Product > Scheme > Edit Scheme...
    2. Be sure to select `Standalone Plugin` as the active scheme 
    3. Under `Run` settings, select `Release` as the `Build Configuration`
    4. Navigate to Product > Scheme > Build For > Run
    5. If building the application does not automatically launch the plugin, navigate to `./Builds/MacOSX/build/Release/` and double-click on `Subsynth`.
 - To run as a standalone plugin (debug version):
    1. First two steps above
    2. Under `Run` settings, select `Debug` as the `Build Configuration`
    3. Continue with steps 4 and 5
 - To build a VST3 or AU plug-in, change the target scheme before building. The plug-ins can be found in the corresponding `./Builds/MacOSX/build/Release/` directory. They can then be loaded in your digital audio workstation/plug-in host of choice.

 > **_NOTE:_** Though JUCE supports linux, no development was done on this platform and no Build/Run instructions can be provided.

##### External MIDI Control with Standalone Plugin

 - By default the standalone plugin does not enable external midi devices. You must select the device in `options` in the upper left. 

---
### References

[JUCE Documentation](https://docs.juce.com/master/index.html)

[JUCE Coding Standards](https://juce.com/discover/stories/coding-standards)

`.clang-format` provided by JUCE developer `reuk` on the [JUCE Forum](https://forum.juce.com/t/automatic-juce-like-code-formatting-with-clang-format/31624)

[The Audio Programmer Youtube Channel](https://www.youtube.com/c/TheAudioProgrammer)

[The Audio Programmer Github Repository](https://github.com/TheAudioProgrammer/tapSynth)
 - We adapted code from this repository for select sections - specifics are documented in our source code.

CS510 course materials

---
### License

This software is released under the [GNU General Public License v.3](./LICENSE).
