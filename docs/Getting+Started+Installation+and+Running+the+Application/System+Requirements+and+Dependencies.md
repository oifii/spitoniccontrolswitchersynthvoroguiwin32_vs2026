# Getting Started: Installation and Running the Application – System Requirements and Dependencies

This section outlines the **minimum environment** needed to build and run the application and details the external libraries it relies on. Follow these guidelines to prepare your machine before opening the solution in Visual Studio.

## System Requirements

Ensure your development workstation meets the following criteria:

- **Operating System**

Windows 7 or later, targeting the Win32 Desktop subsystem.

- **Compiler & Toolset**

Microsoft Visual Studio 2017 or newer (v141+ toolsets), with a custom **VS2026 x64** configuration for 64-bit builds.

- **Architecture**

Support for both Win32 (x86) and x64 platforms.

- **Memory & Storage**

At least 4 GB RAM and sufficient disk space for source, build artifacts, and third-party libraries.

- **C++ Standard**

Full support for C++11 or later (required by Tonic and other modules).

## Dependencies 📦

The application combines a Voronoi-based GUI with a suite of Tonic-based synth demos. It links against several third-party and custom libraries located under `lib-src` and `spiwavsetlib_*`. Below is a summary:

| Library | Location | Purpose |  |
| --- | --- | --- | --- |
| **PortAudio** | `lib-src/portaudio-2021/portaudio_vs2017 | vs2026` | Real-time audio I/O with ASIO & DirectSound backends |
| **Tonic & TonicLib** | `lib-src/tonic/Tonic-master/src`<br>`.../lib/TonicLib*.lib` | Audio synthesis framework |  |
| **FreeImage & FreeImagePlus** | `lib-src/freeimage*/dist`<br>`.../Wrapper/FreeImagePlus` | Image loading and display |  |
| **libsndfile** | `lib-src/libsndfile/include`<br>`.../lib/libsndfile-1.lib` | Reading and writing audio file formats |  |
| **spiwavsetlib** | `spiwavsetlib_vs2017u | vs2026u/.../Release/*.lib` | Custom waveset generation and audio utilities |
| **System Multimedia** | `winmm.lib`, `Dsound.lib`, `Msimg32.lib` | OS-level audio timing and graphics support |  |


### Preprocessor Definitions

> **Note:** Paths differ between Win32 (VS2017) and x64 (VS2026) builds. Adjust accordingly when switching platforms.

Two macros enable low-latency audio backends:

- `PA_USE_ASIO=1` – Enables ASIO API in PortAudio
- `__WINDOWS_DS__` – Enables Windows DirectSound support

These are set automatically in each project configuration.

### Example Configuration Snippet

Below is an excerpt from the **Release | x64** configuration showing include directories and library dependencies for VS2026 builds:

```xml
<ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
  <ClCompile>
    <AdditionalIncludeDirectories>
      .\lib-src\portaudio-2021\portaudio_vs2026\include;
      .\lib-src\freeimage_vs2026(x64)\Source;
      .\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus;
      \spiwavsetlib;
      .\lib-src\tonic(x64)\Tonic-master\src;
      .\lib-src\libsndfile(x64)\include;
      %(AdditionalIncludeDirectories)
    </AdditionalIncludeDirectories>
    <PreprocessorDefinitions>
      WIN32;NDEBUG;_WINDOWS;PA_USE_ASIO=1;__WINDOWS_DS__;%(PreprocessorDefinitions)
    </PreprocessorDefinitions>
  </ClCompile>
  <Link>
    <AdditionalDependencies>
      .\lib-src\freeimage_vs2026(x64)\Dist\x64\FreeImage.lib;
      winmm.lib;
      \spiwavsetlib_vs2026u\x64\Release\spiwavsetlib_vs2019.lib;
      \lib-src\portaudio-2021\portaudio_vs2026\build\msvc\x64\Release\portaudio_x64.lib;
      \lib-src\tonic(x64)\Tonic-master\lib\TonicLibVS2026\x64\Release\TonicLib.lib;
      Dsound.lib;
      \lib-src\libsndfile(x64)\lib\libsndfile-1.lib;
      \lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.lib;
      Msimg32.lib;
      %(AdditionalDependencies)
    </AdditionalDependencies>
    <SubSystem>Windows</SubSystem>
  </Link>
</ItemDefinitionGroup>
```

## Installation Steps

Follow these steps to get the application up and running:

1. **Clone the Repository**

```bash
   git clone https://github.com/oifii/spitoniccontrolswitchersynthvoroguiwin32_vs2026.git
```

1. **Verify Library Folders**

Ensure `lib-src` and all `spiwavsetlib_*` directories are present with their contents.

1. **Open the Solution**

Launch `spitoniccontrolswitchersynthvoroguiwin32_vs2026.sln` in Visual Studio.

1. **Select Configuration & Platform**

Choose **Debug** or **Release** and **Win32** or **x64 (VS2026)**.

1. **Build & Run**

Build the solution (F7) and start debugging (F5) or run the generated executable.

```bash
# Example build commands for VS Developer Command Prompt
msbuild spitoniccontrolswitchersynthvoroguiwin32_vs2026.sln /p:Configuration=Release /p:Platform=x64
.\Release\x64\spitoniccontrolswitchersynthvoroguiwin32.exe
```

```card
{
    "title": "Keep Libraries Updated",
    "content": "Periodically update third-party libs under lib-src to ensure compatibility and performance."
}
```