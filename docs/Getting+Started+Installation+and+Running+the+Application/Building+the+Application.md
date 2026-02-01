# Getting Started: Installation and Running the Application

This section guides you through setting up your environment and building the **spitoniccontrolswitchersynthvoroguiwin32** application. Follow these steps to install prerequisites, verify the directory layout, open the project in Visual Studio, select the correct build configuration, and compile the application.

## Prerequisites

Before building, ensure you have the following installed:

- **Visual Studio 2017** (or later) with the **“Desktop development with C++”** workload.
- **Windows 10 SDK** (10.0) capable of targeting Win32 and x64 desktop apps.
- Git client to clone the repository.

## Directory Structure 🔍

Your repository root should match this layout:

```bash
spitoniccontrolswitchersynthvoroguiwin32_vs2026/
├── lib-src/
│   ├── portaudio-2021/
│   ├── freeimage3180_vs2017/
│   ├── tonic/
│   └── libsndfile/
├── spiwavsetlib_vs2017u/   # Custom static library for VS2017
├── spiwavsetlib_vs2026u/   # Custom static library for VS2026
├── spitoniccontrolswitchersynthvoroguiwin32.vcxproj
├── spitoniccontrolswitchersynthvoroguiwin32.vcxproj.filters
├── src/                    # Application source files
└── ReadMe.txt
```

- **lib-src/** contains all third-party headers and binaries.
- **spiwavsetlib_***/ folders hold the custom static library built separately.
- The `.vcxproj` at the root is your Visual Studio project file.

## Build Configuration

Open the solution file `spitoniccontrolswitchersynthvoroguiwin32.vcxproj` in Visual Studio. This project defines four build configurations:

| Configuration | Platform | PlatformToolset | Precompiled Headers | Debug Libraries |
| --- | --- | --- | --- | --- |
| Debug | Win32 | v145 | Create | true |
| Debug | x64 | v145 | Create | true |
| Release | Win32 | v145 | Create | false |
| Release | x64 | v145 | Create | false |


Each configuration:

- Sets appropriate **compiler optimizations** (Disabled in Debug, MaxSpeed in Release).
- Uses **precompiled headers** for faster builds.
- Links against the correct 32- or 64-bit libraries.

## Third-Party Dependencies 📦

The linker pulls in these libraries. Ensure all paths resolve under `.\lib-src\...` and your `spiwavsetlib_*` folders.

| Library | Include Directory | Library File |
| --- | --- | --- |
| PortAudio (Win32) | `.\lib-src\portaudio-2021\portaudio_vs2017\include` | `.\lib-src\portaudio-2021\portaudio_vs2017\build\msvc\Win32\Release\portaudio_x86.lib` |
| PortAudio (x64) | `.\lib-src\portaudio-2021\portaudio_vs2026\include` | `.\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\x64\Release\portaudio_x64.lib` |
| FreeImage (x32) | `.\lib-src\freeimage3180_vs2017\dist\x32` | `.\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.lib` |
| FreeImagePlus (x32) | `.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus` | `.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.lib` |
| FreeImage (x64) | `.\lib-src\freeimage_vs2026(x64)\Source` | `.\lib-src\freeimage_vs2026(x64)\Dist\x64\FreeImage.lib` |
| FreeImagePlus (x64) | `.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus` | `.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.lib` |
| TonicLib (Win32) | `.\lib-src\tonic\Tonic-master\src` | `.\lib-src\tonic\Tonic-master\lib\TonicLibVS2017\Release\TonicLib.lib` |
| TonicLib (x64) | `.\lib-src\tonic(x64)\Tonic-master\src` | `.\lib-src\tonic(x64)\Tonic-master\lib\TonicLibVS2026\x64\Release\TonicLib.lib` |
| libsndfile | `.\lib-src\libsndfile\include` | `.\lib-src\libsndfile\libsndfile-1.lib` (32-bit) / `.\lib-src\libsndfile(x64)\lib\libsndfile-1.lib` (64-bit) |
| spiwavsetlib (custom) | `.\spiwavsetlib` | `.\spiwavsetlib_vs2017u\release\spiwavsetlib_vs2017.lib` (Win32) / `.\spiwavsetlib_vs2026u\x64\Release\spiwavsetlib_vs2019.lib` (x64) |
| Windows Multimedia | — | `winmm.lib`, `Dsound.lib`, `Msimg32.lib` |


## Step-by-Step Build Guide 🛠️

1. **Clone the repository**

```bash
   git clone https://github.com/oifii/spitoniccontrolswitchersynthvoroguiwin32_vs2026.git
   cd spitoniccontrolswitchersynthvoroguiwin32_vs2026
```

1. **Verify folder layout**
2. Confirm `lib-src/` contains `portaudio-2021`, `freeimage3180_vs2017`, `tonic`, `libsndfile`.
3. Ensure `spiwavsetlib_vs2017u/` and `spiwavsetlib_vs2026u/` exist with their `.lib` files.
4. **Open the project**
5. Launch Visual Studio.
6. Open **spitoniccontrolswitchersynthvoroguiwin32.vcxproj**.
7. **Select your configuration**
8. From the toolbar drop-down, choose **Debug** or **Release**, and **Win32** or **x64**.
9. **Build the solution**
10. Press **Ctrl+Shift+B** or select **Build ▶ Build Solution**.
11. The linker will automatically include PortAudio, TonicLib, FreeImage, FreeImagePlus, libsndfile, spiwavsetlib, and Windows multimedia libraries.

Alternatively, use MSBuild in a **Developer Command Prompt**:

```bash
msbuild spitoniccontrolswitchersynthvoroguiwin32.vcxproj /p:Configuration=Release /p:Platform=x64
```

## Next Steps

- After a successful build, copy the appropriate PortAudio DLL into the output folder:

```bash
  copy .\lib-src\portaudio-2021\portaudio_vs2026\build\msvc\x64\Release\portaudio_x64.dll .\x64\Release\
```

- Run **spitoniccontrolswitchersynthvoroguiwin32.exe** from your chosen configuration folder.

Congratulations! 🎉 You have built the application and are ready to explore the Vorogui graphical controller and Tonic synth demos.