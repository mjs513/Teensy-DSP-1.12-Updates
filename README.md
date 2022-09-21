# Teensy-DSP-1.12-Updates

## Background

The latest version of CMSIS DSP is no longer easy to port to Teensy with out going through and generating your arm_math.a files. DSP is now broken out from CMSIS5 into its own repository as of version 5.5.1

The following folders have been removed:
<blockquote>
CMSIS/Lib/ (superseded by CMSIS/DSP/Lib/)
CMSIS/DSP_Lib/ (superseded by CMSIS/DSP/)
</blockquote>
The following folders are deprecated:
CMSIS/Include/ (superseded by CMSIS/DSP/Include/ and CMSIS/Core/Include/)
for 5.5.1
CMSIS/Include/ (superseded by CMSIS/DSP/Include/ and CMSIS/Core/Include/)
In previous versions ARM CMSIS provided the gcc pre-compiled binaries for arm_math. Now with the latest versions you have to build your own libraries. However, think I broke the code on how that you can do that.
