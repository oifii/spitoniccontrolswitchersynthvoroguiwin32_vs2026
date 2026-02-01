# Using the Vorogui Visual Controller – Mouse Interaction Model

The Vorogui engine provides a **high-level mouse event API** for interactive control of parameters and point‐set editing in the Win32 application. Handlers are exposed in `c_vorogui.h` and invoked from the main window procedure to enable:

- Real-time **parameter adjustment** via Voronoi regions
- **Dragging** of control points
- **Toggling** of per-point text labels

Below is a detailed breakdown of each interaction mode.

---

## Event Handler Prototypes

The three core mouse handlers live in `c_vorogui.h` and operate on a `POINTSET*` representing the Voronoi diagram .

| Function | Description |
| --- | --- |
| **VOROGUI_OnLButtonDown** | Begin parameter adjust or start moving a control point |
| **VOROGUI_OnLButtonUp** | Finalize a point move |
| **VOROGUI_OnRButtonUp** | Toggle display of per-point text labels |


```cpp
// c_vorogui.h
void VOROGUI_OnLButtonDown(POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
void VOROGUI_OnLButtonUp  (POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
void VOROGUI_OnRButtonUp  (POINTSET* pPOINTSET, HWND hwnd, WPARAM wParam, LPARAM lParam);
```

---

## Integration with Win32 Message Loop

In your `WndProc`, each mouse message forwards to the corresponding Vorogui handler :

```cpp
case WM_LBUTTONDOWN:
    VOROGUI_OnLButtonDown(global_pPOINTSET, hWnd, wParam, lParam);
    break;
case WM_LBUTTONUP:
    VOROGUI_OnLButtonUp(global_pPOINTSET, hWnd, wParam, lParam);
    break;
case WM_RBUTTONUP:
    VOROGUI_OnRButtonUp(global_pPOINTSET, hWnd, wParam, lParam);
    break;
```

---

## 1. Parameter Adjustment 🎛️

Clicking on a Voronoi cell **without** holding Ctrl allows you to adjust its linked synthesizer parameter. The flow is:

1. **Nearest‐point lookup**

Calls `VOROGUI_GetNearestPointsetObject`, which wraps `FindNearestNeighbor` with `POINTSET_ADJACENTTRIFLAG_FIRSTADJTRI` .

1. **Voronoi polygon extraction**

Retrieves the polygon for that site via `VOROGUI_GetVoronoiPolygon`.

1. **Vertical‐position → ratio**

Computes the polygon’s `ymin`/`ymax` and maps the click’s `yPos` into a `0–1` **control ratio**.

1. **Parameter mapping**

Fetches the parameter’s `[min,max]` from `global_pSynth->getParameters()`, computes

`value = ratio*(max-min) + min`, then calls

`global_pSynth->setParameter(name, value)`.

1. **Redraw**

Invalidates the window to reflect the new fill level.

```cpp
// Inside VOROGUI_OnLButtonDown
if (!(wParam & MK_CONTROL)) {
  int id = VOROGUI_GetNearestPointsetObject(pPOINTSET, xPos, yPos, &vorogui_itriseed);
  if (id >= vorogui_numberofframepoints) {
    POINT poly[100]; int nPoly=0;
    if (VOROGUI_GetVoronoiPolygon(pPOINTSET, id, poly, &nPoly)) {
      long xmin,xmax,ymin,ymax; int iymin;
      VOROGUI_GetPolygonMinMax(poly,nPoly,&xmin,&xmax,&ymin,&ymax,&iymin);
      double ratio = (double)abs(ymax - yPos) / abs(ymax - ymin);
      ratio = std::clamp(ratio, 0.0, 1.0);
      pPOINTSET->controlratio[id] = ratio;

      auto params = global_pSynth->getParameters();
      auto &param = params[id - vorogui_numberofframepoints];
      double val = ratio*(param.getMax() - param.getMin()) + param.getMin();
      global_pSynth->setParameter(param.getName(), val);
    }
  }
  RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE);
}
```

*Key functions:* `VOROGUI_GetNearestPointsetObject`,

`VOROGUI_GetVoronoiPolygon`, `VOROGUI_GetPolygonMinMax`,

`synth->getParameters()`, `synth->setParameter()` .

---

## 2. Control Point Movement 🔀

Holding **Ctrl** while clicking initiates a drag of an existing control point:

- **OnLButtonDown (Ctrl)**
- Finds the nearest point beyond the frame border
- Stores its index in `vorogui_idpointtobemoved`
- **OnLButtonUp (Ctrl)**
- **Collision check**: ensures the new `(xPos,yPos)` is not within 1 pixel of another point
- **Coordinate update**: writes `pPOINTSET->px[id] = xPos`, `py[id] = yPos`
- **Rebuild mesh**
- Reset extents (`xmin`, `ymin`, `xmax`, `ymax`)
- `BuildTriangleNetwork(pPOINTSET)`
- `ComputeAllTriangleCenters(pPOINTSET)`
- **Redraw** via `RedrawWindow`

```cpp
// VOROGUI_OnLButtonDown
else if (wParam & MK_CONTROL) {
  int id = VOROGUI_GetNearestPointsetObject(pPOINTSET, xPos, yPos, &vorogui_itriseed);
  if (id >= vorogui_numberofframepoints)
    vorogui_idpointtobemoved = id;
}

// VOROGUI_OnLButtonUp
if (wParam & MK_CONTROL && vorogui_idpointtobemoved != -1) {
  // 1) Collision detection
  for(int i=0;i<pPOINTSET->npts;i++){
    if (abs(pPOINTSET->px[i]-xPos)<1 && abs(pPOINTSET->py[i]-yPos)<1){
      vorogui_idpointtobemoved = -1;
      return;
    }
  }
  // 2) Update location
  pPOINTSET->px[vorogui_idpointtobemoved] = xPos;
  pPOINTSET->py[vorogui_idpointtobemoved] = yPos;
  vorogui_idpointtobemoved = -1;

  // 3) Rebuild triangulation
  pPOINTSET->ntri = 0;
  pPOINTSET->xmin = pPOINTSET->ymin = DBL_MAX;
  pPOINTSET->xmax = pPOINTSET->ymax = -DBL_MAX;
  for(int i=0;i<pPOINTSET->npts;i++){
    pPOINTSET->xmin = min(pPOINTSET->xmin, pPOINTSET->px[i]);
    pPOINTSET->xmax = max(pPOINTSET->xmax, pPOINTSET->px[i]);
    pPOINTSET->ymin = min(pPOINTSET->ymin, pPOINTSET->py[i]);
    pPOINTSET->ymax = max(pPOINTSET->ymax, pPOINTSET->py[i]);
  }
  BuildTriangleNetwork(pPOINTSET);
  ComputeAllTriangleCenters(pPOINTSET);

  // 4) Refresh view
  RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE);
}
```

*Dependencies:* `BuildTriangleNetwork`, `ComputeAllTriangleCenters` .

---

## 3. Label Toggle 🏷️

A **right-click** anywhere in the view toggles on-screen text labels for each control point:

```cpp
void VOROGUI_OnRButtonUp(POINTSET* pPOINTSET, HWND hwnd, WPARAM, LPARAM) {
  vorogui_displaytextlabelflag = !vorogui_displaytextlabelflag;
  RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE);
}
```

This simple flip controls whether `TextOutA` calls render parameter names over each site .

---

## Global State & Configuration

| Variable | Role |
| --- | --- |
| vorogui_numberofframepoints | Number of non‐parameter “frame” points (index threshold) |
| vorogui_idpointtobemoved | Index of the point being dragged (or –1 if none) |
| vorogui_displaytextlabelflag | Toggles rendering of per‐point text labels |


These globals live alongside the handlers in `c_vorogui.c/.cpp`.

---

## Summary of Dependencies

- **WIN32 GDI** (`Get/SelectObject`, `RedrawWindow`)
- **Pointset API** (`FindNearestNeighbor`, `BuildTriangleNetwork`, `ComputeAllTriangleCenters`)
- **Voronoi helpers** (`VOROGUI_GetVoronoiPolygon`, `VOROGUI_GetPolygonMinMax`)
- **Tonic synth** (`global_pSynth`, parameter list, `setParameter`)

Together, these enable an interactive, Voronoi‐based GUI for controlling synthesis parameters and editing control‐point layouts in real time.