# Troubleshooting and Tips – Saving and Loading Layouts

When working with Vorogui layouts, you may encounter unexpected behavior when saving or loading configurations. This guide highlights key areas to check and offers practical solutions to common issues.

---

## Generated Filename Consistency

Vorogui uses your synth’s C++ **class name** to generate the layout filename. If this changes between runs, Vorogui will look for a different file and your saved layout won’t load.

```cpp
string VOROGUI_GetFilename(){
  // Use synth's class name without tags
  string mystring = typeid(global_pSynth).name();
  mystring = mystring.substr(6, mystring.length() - (6 + 2)); 
  // Remove "class" and " *" tags
  mystring += ".txt";              // Append .txt extension
  return mystring;
}
```

• Ensure the **class name** of your synth (`ControlSwitcherExpSynth`, `FMDroneExpSynth`, etc.) remains stable across builds.

---

## Permissions and File Locations

Vorogui writes and reads plain-text files in the **current working directory**. If you see no file or an empty layout after saving:

- ✔️ Verify the application has **write** permission in that folder.
- ✔️ Confirm the generated filename (e.g., `DelayTestSynth.txt`) appears where you expect.
- ❌ On Windows, avoid system-protected directories (e.g., `C:\Program Files\…`).

---

## Point-to-Parameter Mapping

`VOROGUI_WriteToDisk` assumes a **fixed ordering** between Voronoi points (after the frame boundary) and your synth’s parameter list. Any mismatch leads to wrong values or missing points on reload.

```cpp
void VOROGUI_WriteToDisk(POINTSET* pPOINTSET){
  string filename = VOROGUI_GetFilename();
  FILE* pFILE = fopen(filename.c_str(), "w");
  if(pFILE){
    fprintf(pFILE, "parametername\tparametervalue\tparameterxposition\tparameteryposition\n");
    auto params = global_pSynth->getParameters();
    // Must match: (npts – frameCount) == params.size()
    assert((pPOINTSET->npts - vorogui_numberofframepoints) == params.size());

    for(int id = vorogui_numberofframepoints; id < pPOINTSET->npts; ++id){
      auto& cp = params[id - vorogui_numberofframepoints];
      fprintf(pFILE, "%s\t%f\t%f\t%f\n",
              cp.getName().c_str(),
              cp.getValue(),
              pPOINTSET->px[id],
              pPOINTSET->py[id]);
    }
    fclose(pFILE);
  }
}
```

• **Warning:** Changing `addParameter(…)` order, min/max settings, or the number of parameters invalidates older layouts.

---

## Cleaning Up Outdated Layout Files 🧹

After modifying your synth’s parameters:

1. **Delete old **`**.txt**`** files** matching the previous class name or parameter set.
2. Re-save your layout with **Ctrl + S** in the app to regenerate fresh files.
3. Relaunch to ensure the new mapping loads correctly.

---

## Quick-Reference Table of Common Issues

| Symptom | Root Cause | Solution |
| --- | --- | --- |
| No layout file appears after saving | Lacking write permission or wrong working directory | Grant RW access; run outside system folders |
| Layout loads but points mis-aligned / wrong values | Parameter list changed since last save | Delete old file; re-generate via current build |
| “Assertion failed” in `WriteToDisk` | `npts – frameCount ≠ params.size()` | Check `vorogui_numberofframepoints` vs. param count |
| App cannot open `.txt` file on startup | Class name changed; new filename does not match old | Rename or remove outdated `.txt` |


---

```card
{
    "title": "Best Practice",
    "content": "Keep your synth\u2019s parameter list stable once sharing or archiving layouts."
}
```

*By following these tips, you’ll ensure seamless saving and reloading of your Vorogui layouts.*