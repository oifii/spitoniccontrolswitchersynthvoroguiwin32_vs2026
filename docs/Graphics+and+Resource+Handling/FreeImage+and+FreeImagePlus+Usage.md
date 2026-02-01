# Graphics and Resource Handling – FreeImage and FreeImagePlus Usage

This section describes how the application integrates **FreeImage** and **FreeImagePlus** for loading, manipulating, and rendering bitmaps in the Win32 GUI. It covers project configuration, image loading at startup, runtime drawing on the device context, and the role of the FreeImagePlus wrapper.

## Build Configuration 🛠️

The Visual Studio project references both FreeImage and FreeImagePlus libraries. Include paths and linker settings differ per configuration and platform:

|  | Configuration | Include Directories | Linked Libraries |
| --- | --- | --- | --- |
| Debug | Win32 | `.\lib-src\freeimage3180_vs2017\dist\x32`<br>`.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus` | `.\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.lib`<br>`.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.lib` |
| Debug | x64 | `.\lib-src\freeimage_vs2026(x64)\Source`<br>`.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus` | `.\lib-src\freeimage_vs2026(x64)\Dist\x64\FreeImage.lib`<br>`.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.lib` |
| Release | Win32 | `.\lib-src\freeimage3180_vs2017\dist\x32`<br>`.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus` | `.\lib-src\freeimage3180_vs2017\Dist\x32\FreeImage.lib`<br>`.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus\dist\x32\FreeImagePlus.lib` |
| Release | x64 | `.\lib-src\freeimage_vs2026(x64)\Source`<br>`.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus` | `.\lib-src\freeimage_vs2026(x64)\Dist\x64\FreeImage.lib`<br>`.\lib-src\freeimage_vs2026(x64)\Wrapper\FreeImagePlus\dist\x64\FreeImagePlus.lib` |


- **Include Directories** point to FreeImage headers and the FreeImagePlus wrapper.
- **AdditionalDependencies** include both `FreeImage.lib` and `FreeImagePlus.lib`.

## Bitmap Loading at Startup 🎨

On application initialization (`InitInstance`), the JPEG background is loaded into a global `FIBITMAP*`. The code then creates scaled versions for icon resources:

```cpp
// Load main background image from disk
global_dib = FreeImage_Load(FIF_JPEG, "background.jpg", JPEG_DEFAULT);

// Generate 16×16 icon
FIBITMAP* icon16 = FreeImage_Rescale(global_dib, 16, 16, FILTER_BICUBIC);
FreeImage_Save(FIF_ICO, icon16, "background_16x16xrgb-new.ico");
FreeImage_Unload(icon16);

// Generate 32×32 icon
FIBITMAP* icon32 = FreeImage_Rescale(global_dib, 32, 32, FILTER_BICUBIC);
FreeImage_Save(FIF_ICO, icon32, "background_32x32xrgb-new.ico");
FreeImage_Unload(icon32);

// Generate 48×48 icon
FIBITMAP* icon48 = FreeImage_Rescale(global_dib, 48, 48, FILTER_BICUBIC);
FreeImage_Save(FIF_ICO, icon48, "background_48x48xrgb-new.ico");
FreeImage_Unload(icon48);
```

- **FreeImage_Load** reads common formats (JPEG, PNG, etc.).
- **FreeImage_Rescale** creates new bitmaps at specified dimensions.
- **FreeImage_Save** writes out ICO files.
- **FreeImage_Unload** frees the `FIBITMAP`.

## Rendering Images onto Win32 Device Context 🖼️

In the `WM_PAINT` handler, the background bitmap is drawn to the window using GDI’s `StretchDIBits`. This leverages FreeImage’s C-API to access raw bitmap data:

```cpp
case WM_PAINT: {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hWnd, &ps);
  SetStretchBltMode(hdc, COLORONCOLOR);

  // Draw the entire background image stretched to window size
  StretchDIBits(
    hdc,
    0, 0, global_imagewidth, global_imageheight,
    0, 0,
    FreeImage_GetWidth(global_dib),
    FreeImage_GetHeight(global_dib),
    FreeImage_GetBits(global_dib),
    FreeImage_GetInfo(global_dib),
    DIB_RGB_COLORS,
    SRCCOPY
  );

  EndPaint(hWnd, &ps);
} break;
```

- **FreeImage_GetWidth / GetHeight** retrieve dimensions.
- **FreeImage_GetBits** returns a pointer to pixel data.
- **FreeImage_GetInfo** supplies the `BITMAPINFO` for GDI.
- **StretchDIBits** handles pixel transfer.

## FreeImagePlus Integration 🔧

Although most code uses the FreeImage C-API, the project is configured to include **FreeImagePlus**, a C++ wrapper that offers:

- `**fipImage**` class for RAII management of `FIBITMAP`.
- Simplified format conversion and metadata access.
- Direct conversion to HBITMAP for GDI drawing.

Include path:

```plaintext
.\lib-src\freeimage3180_vs2017\Wrapper\FreeImagePlus
```

and corresponding `FreeImagePlus.lib` linkage ensure advanced image operations can be added without manual memory management .

---

By combining FreeImage’s robust format support with the Win32 GDI interface and optional FreeImagePlus conveniences, the application efficiently manages bitmap resources for its Voronoi-based GUI.