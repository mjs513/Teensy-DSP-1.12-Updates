# Teensy-DSP-1.12-Updates

## Background

The latest version of CMSIS DSP is no longer easy to port to Teensy with out going through and generating your arm_math.a files. DSP is now broken out from CMSIS5 into its own repository as of version 5.5.1

The following folders have been removed:
<blockquote>
CMSIS/Lib/ (superseded by CMSIS/DSP/Lib/)
CMSIS/DSP_Lib/ (superseded by CMSIS/DSP/)
The following folders are deprecated:
CMSIS/Include/ (superseded by CMSIS/DSP/Include/ and CMSIS/Core/Include/)
</blockquote>

for 5.5.1
<blockquote>
CMSIS/Include/ (superseded by CMSIS/DSP/Include/ and CMSIS/Core/Include/)
</blockquote>

In previous versions ARM CMSIS provided the gcc pre-compiled binaries for arm_math. Now with the latest versions you have to build your own libraries. However, think I broke the code on how that you can do that.

## Build process

With that said with come poking around I did find that in version 5.5.0 they provided a uVision project file that you can you to compile the binaries for the library:
1. Downlowad the MDK µVision® IDE from ARM-Keil website
https://www2.keil.com/mdk5/uvision/ (Overview)

https://www.keil.com/download/product/ (download MDK-ARM - you will have to register)

2. Download Arm GNU Toolchain (my case I picked 11.3.1) to be inline with what we are currently using.
https://developer.arm.com/downloads/...hain-downloads

3. Then I downloaded the latest versions of CMSIS5 and CMSIS-DSP and put them on my D-drive (made my life easier.

4. Copied the project file for gcc and made some major edits to cover all the new library functions and edited the file paths for each board I wanted to compile for.
a. In [ project->manage -> project Items ] I deleted all the groups and recreated them using all the new groups and files from the latest CMSIS-DSP source folder.

Name:	Capture.PNG 

5. Then I selected the processor I wanted to compile for and did a left click and selected: Options for target board. In the CC Tab i edited the paths for files:

Name:	Capture1.PNG 

Then I did the Build for the target and got my compiled binary for the library. However the library files are now 9Meg versus the 2-3 meg they were before so not sure if all the compile options are correct but they are the default ones that come with the project file.

The last step was to update the arm_structs.h and arm_constants.h files in the cores.

EDIT: Ref this post: https://forum.pjrc.com/threads/71074...l=1#post312904

## Install Updates

Basically just copy and paste files in the TeensyX Files to the associated cores where ever you placed you TD install's. For me that would be
..\arduino-1.8.19-1131\hardware\teensy\avr\cores\teensy3 or teensy4 directories

Then copy the lib files in the Precompiled Binaries to:
..\arduino-1.8.19-1131\hardware\tools\arm\arm-none-eabi\lib

Then you are good to go. No need to edit the files.
