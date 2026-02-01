# Getting Started: Installation and Running the Application – Running the Application

Welcome! This section walks you through launching the **SpitonicControlSwitcherSynthVoroguiWin32** app on Windows. We cover launching from Visual Studio or directly from the build output folder, passing an optional ASIO device name, and what happens under the hood once the app starts.

## Running from Visual Studio

Open the solution in Visual Studio 2022/2026 and select your desired configuration:

- **Configuration**: Debug / Release
- **Platform**: x86 / x64

Then press **F5** (Start Debugging) or **Ctrl+F5** (Start Without Debugging). Visual Studio will build and launch the executable, creating the main GUI window via the Windows subsystem.

## Running from the Output Folder

You can also run the compiled `.exe` directly:

| Configuration Folder | Path Example |
| --- | --- |
| Debug | `...\Build\Debug\SpitonicControl.exe` |
| Release | `...\Build\Release\SpitonicControl.exe` |
| x64 Release | `...\Build\x64\Release\SpitonicControl.exe` |


Just navigate to the folder and double-click the **SpitonicControl.exe** file to launch.

## Command-Line Option: ASIO Input Device

At startup, you may specify the name of the ASIO input device as the **first** command-line argument. If omitted, the app searches for a default such as `"E-MU ASIO"`, stored in the global variable `global_audioinputdevicename`. The app looks up this name and opens the matching PortAudio device:

```cpp
bool SelectAudioInputDevice() {
  // Build map of all device names → indices
  for (int i = 0; i < Pa_GetDeviceCount(); ++i) {
    auto info = Pa_GetDeviceInfo(i);
    global_inputdevicemap[info->name] = i;
  }
  // Try to find the user-specified name...
  auto it = global_inputdevicemap.find(global_audioinputdevicename);
  int deviceId = (it != global_inputdevicemap.end())
               ? it->second
               : Pa_GetDefaultInputDevice();
  global_inputParameters.device = deviceId;
  // ...set channel count, format, latency, ASIO flags
  global_inputParameters.channelCount = NUM_CHANNELS;
  global_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  global_inputParameters.suggestedLatency =
    Pa_GetDeviceInfo(deviceId)->defaultLowInputLatency;
  return true;
}
```

This lookup logic is implemented in the `SelectAudioInputDevice` function .

## GUI Window Initialization

The Windows entry point (`_tWinMain`) registers the window class, parses command-line arguments, and calls `InitInstance` to create and show the main window:

```cpp
MyRegisterClass(hInstance);
if (!InitInstance(hInstance, nCmdShow))
  return FALSE;
// In InitInstance:
HWND hWnd = CreateWindow(
  szWindowClass, szTitle,
  WS_OVERLAPPEDWINDOW,  // or WS_POPUP if no title bar
  global_x, global_y,
  global_xwidth, global_yheight,
  NULL, NULL,
  hInstance, NULL
);
SetLayeredWindowAttributes(hWnd, 0, global_alpha, LWA_ALPHA);
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);
```

The call to `ShowWindow` and `UpdateWindow` makes the Vorogui controller visible .

## Audio Streaming

Once the window is ready, the app opens a PortAudio stream (ASIO or DirectSound) and starts it:

```cpp
PaError err = Pa_OpenStream(
  &global_stream,
  &global_inputParameters,
  &global_outputParameters,
  sampleRate, bufferFrames,
  paNoFlag,      // no special flags
  RenderCallback,// audio callback for synth rendering
  nullptr        // user data
);
if (err != paNoError) // handle error...
err = Pa_StartStream(global_stream);
```

On success, audio flows through the ASIO/DirectSound driver to your DAC .

## Interactive Vorogui Controller

The main window hosts a **Vorogui**-based point set. Each point maps to a synth parameter from the Tonic demo suite. Click, drag, or Ctrl-drag points to adjust synth parameters in real time. The Voronoi polygons update dynamically, providing both visual and sonic feedback.

## Closing the Application

To exit, simply use the standard Windows close button (✖) or select **File → Exit** (if the menu bar is enabled). The message loop detects `WM_DESTROY`, stops the audio stream, and cleans up resources before terminating.

---

🎉 You’re all set! Launch, explore the Vorogui interface, and enjoy live control over your Tonic-based synthesizers.