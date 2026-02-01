# Getting Started: Installation and Running the Application

This section guides you through installing dependencies, building the project, and running the application on Windows. It also explains the **project layout** and the **key files** you’ll interact with.

## Prerequisites

Before you begin, ensure you have:

- Visual Studio 2017 or later with **C++ Desktop Development** workload
- Windows SDK (included with VS installer)
- Git (to clone the repository)
- Dependencies:
- PortAudio (with ASIO support)
- FreeImage
- libsndfile
- Tonic library (audio synthesis)
- spiwavsetlib (custom audio helper library)

## Building the Project

1. Clone the repository:

```bash
   git clone https://github.com/oifii/spitoniccontrolswitchersynthvoroguiwin32_vs2026.git
```

1. Open the Visual Studio solution:

```bash
   cd spitoniccontrolswitchersynthvoroguiwin32_vs2026
   start spitoniccontrolswitchersynthvoroguiwin32.vcxproj
```

1. Select your **Configuration** (`Debug` or `Release`) and **Platform** (`Win32` or `x64`).
2. Build the solution (`Build → Build Solution`).

## Running the Application

Once built, you can launch the executable from the IDE or via File Explorer:

```bash
.\Debug\spitoniccontrolswitchersynthvoroguiwin32.exe
```

No installer is required; all DLLs and libraries are linked statically or alongside the EXE.

# Project Layout and Key Files

Below is an overview of the root-level project structure and the responsibilities of its main files.

| File | Purpose | Description |
| --- | --- | --- |
| 🔧 **spitoniccontrolswitchersynthvoroguiwin32.vcxproj** | Project Configuration | Defines build configurations, include paths, library dependencies, and the list of source/header files |
| 🖥️ **spitoniccontrolswitchersynthvoroguiwin32.cpp** / **.h** | Main Application | Entry point for Win32 application: window creation, message loop, integration of the Voronoi GUI with Tonic synths, audio I/O setup via PortAudio/ASIO |
| 🎨 **c_vorogui.h** / **c_vorogui.cpp** | Voronoi GUI Engine | Manages point-set creation, Voronoi diagram construction, rendering to HDC, and mouse interaction callbacks |
| 🔢 **c_pointset.cpp** | Point Set Management | Implements data structures for points, Delaunay triangulation, statistical mapping of synth parameters to control ratios |
| 🎛️ **Synth Demos (headers)** | Audio Synth Modules | A collection of Tonic-based synthesizer demos. Examples include: |


- **DelayTestSynth.h**: basic delay effect with tempo-sync
- **FMDroneExpSynth.h**: FM synthesis drone demo
- **SynthsAsGeneratorsExpSynth.h**: combining generators and effects
- **XYSpeedSynth.h**: interactive synth controlled by mouse speed

Each synth is registered via `TONIC_REGISTER_SYNTH`, making it discoverable by the main application.

---

**Card**

```card
{
    "title": "Build Tip",
    "content": "Ensure all AdditionalIncludeDirectories in the .vcxproj point to the correct dependency locations before building."
}
```