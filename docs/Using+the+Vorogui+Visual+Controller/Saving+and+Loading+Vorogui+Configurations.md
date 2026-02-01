# Using the Vorogui Visual Controller – Saving and Loading Configurations

This section explains how Vorogui persists and restores custom control layouts for each synth. Users can save control‐point mappings and reload them across sessions. The API relies on three core functions to generate filenames and read/write plain‐text files.

---

## 📝 Generating Default Filenames

Vorogui must associate each synth instance with a unique configuration file.

The function **VOROGUI_GetFilename** uses the synth’s C++ class name (stripped of tags) and appends a `.txt` extension.

```cpp
string VOROGUI_GetFilename(){
  // Extract synth class name without "class" or pointer markers
  string mystring = typeid(global_pSynth).name();
  mystring = mystring.substr(6, mystring.length() - (6 + 2));
  // Append .txt extension
  mystring += ".txt";
  return mystring;
}
```

- It calls `typeid(global_pSynth).name()`.
- It removes the leading `"class "` and trailing `" *"`.
- It returns e.g. `ControlSwitcherExpSynth.txt`.

---

## 💾 Writing Configurations to Disk

When users press **'S'**, Vorogui invokes **VOROGUI_WriteToDisk** to persist the current layout. The function:

- Calls `VOROGUI_GetFilename` for a synth‐specific name.
- Opens a plain‐text file for writing.
- Writes a header row, then one line per control point.
- Uses `global_pSynth->getParameters()` to obtain parameter names and current values.
- Uses the `POINTSET` structure to get each point’s X/Y coordinates.

```cpp
void VOROGUI_WriteToDisk(POINTSET* pPOINTSET){
  string filename = VOROGUI_GetFilename();
  FILE* pFILE = fopen(filename.c_str(), "w");
  if (!pFILE) return;

  // Header: column titles
  fprintf(pFILE, "%s\t%s\t%s\t%s\n",
    "parametername",
    "parametervalue",
    "parameterxposition",
    "parameteryposition"
  );

  // Fetch synth parameters
  vector<ControlParameter> params = global_pSynth->getParameters();
  assert((pPOINTSET->npts - vorogui_numberofframepoints) == params.size());

  // Write each point’s data
  for (int id = vorogui_numberofframepoints; id < pPOINTSET->npts; id++){
    string name  = params[id - vorogui_numberofframepoints].getName();
    float  value = params[id - vorogui_numberofframepoints].getValue();
    float  xPos  = pPOINTSET->px[id];
    float  yPos  = pPOINTSET->py[id];
    fprintf(pFILE, "%s\t%f\t%f\t%f\n",
      name.c_str(), value, xPos, yPos
    );
  }

  fclose(pFILE);
}
```

### File Format

| Column | Description |
| --- | --- |
| **parametername** | Synth parameter identifier |
| **parametervalue** | Current floating‐point value |
| **parameterxposition** | X coordinate in Vorogui canvas |
| **parameteryposition** | Y coordinate in Vorogui canvas |


---

## 📂 Reading Configurations from Disk

At startup or synth change, Vorogui calls **VOROGUI_ReadFromDisk** to restore a saved layout. It:

1. Builds an empty `POINTSET` with default points.
2. Attempts to open the configuration file via `VOROGUI_GetFilename`.
3. Skips the header line and parses each subsequent row.
4. Matches each row’s parameter name to the synth’s parameter list.
5. Calls `global_pSynth->setParameter(name, value)` to restore values.
6. Updates the `POINTSET`’s X/Y arrays for control‐point positions.
7. Recomputes Voronoi and triangulation internals.

```cpp
POINTSET* VOROGUI_ReadFromDisk(){
  // Fetch parameter list
  vector<ControlParameter> params = global_pSynth->getParameters();

  // Create empty pointset (no built-in points)
  POINTSET* pPOINTSET = VOROGUI_CreatePointset(false);

  // Open config file
  string filename = VOROGUI_GetFilename();
  ifstream myfile(filename);
  string line;
  int linecount = 0;

  if (myfile){
    while (getline(myfile, line)){
      linecount++;
      if (linecount == 1) continue; // skip header

      istringstream buf(line);
      vector<string> tokens((istream_iterator<string>(buf)),
                             istream_iterator<string>());
      // tokens: name, value, x, y
      string name     = params[linecount-2].getName();
      float  value    = atof(tokens[1].c_str());
      global_pSynth->setParameter(name, value);

      int   idx       = vorogui_numberofframepoints + (linecount - 2);
      pPOINTSET->px[idx] = atof(tokens[2].c_str());
      pPOINTSET->py[idx] = atof(tokens[3].c_str());
    }
  }

  // Recompute canvas bounds and triangulation
  pPOINTSET->xmin = pPOINTSET->ymin =  DBL_MAX;
  pPOINTSET->xmax = pPOINTSET->ymax = -DBL_MAX;
  for (int i = 0; i < pPOINTSET->npts; i++){
    pPOINTSET->xmin = min(pPOINTSET->xmin, pPOINTSET->px[i]);
    pPOINTSET->xmax = max(pPOINTSET->xmax, pPOINTSET->px[i]);
    pPOINTSET->ymin = min(pPOINTSET->ymin, pPOINTSET->py[i]);
    pPOINTSET->ymax = max(pPOINTSET->ymax, pPOINTSET->py[i]);
  }
  BuildTriangleNetwork(pPOINTSET);
  ComputeAllTriangleCenters(pPOINTSET);

  return pPOINTSET;
}
```

---

## 🔗 Integration with Synth Parameters

- `**global_pSynth->getParameters()**` returns a vector of `ControlParameter` objects.
- The number of configurable points equals `params.size()`.
- `POINTSET` reserves `vorogui_numberofframepoints` for static display points.
- Saved files map directly to the parameter order to ensure consistent restoration.

---

## 🌟 Best Practices

```card
{
    "title": "Consistent Layouts",
    "content": "Use unique synth class names and avoid renaming classes, or old config files will no longer match."
}
```

- Save after arranging controls to capture current state.
- Keep backup copies of frequently tweaked synth configs.
- Load configurations before starting audio streams to avoid glitching.
- Version control `.txt` files alongside project source for reproducibility.