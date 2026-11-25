# Smart City Route Optimization and Intelligent Navigation System

A student-scale DAA project in C with a GTK4 desktop interface. The city is fictional and its road data is included in the source. The program demonstrates weighted shortest-path search and basic graph traversal.

## Features
- Select a source, destination, and algorithm.
- Dijkstra and A* use weighted road distances.
- BFS finds a route with the fewest edges in the unweighted graph.
- DFS demonstrates depth-first traversal; it is not a shortest-path algorithm.
- A traffic checkbox adds a fixed cost to two sample roads.
- The map highlights the selected route and the result panel reports the route and visited-node count.

## Requirements
- C compiler (GCC or Clang)
- GTK 4 development package
- Cairo (normally installed as a GTK dependency)
- `pkg-config` (Linux/macOS) or GTK4 development environment (Windows)

## Build and run on Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-4-dev
make
./smart-city-routes
```

## Build and run on Windows
The easiest reproducible route is MSYS2 with the UCRT64 environment:
1. Install MSYS2 from https://www.msys2.org/.
2. Open the **MSYS2 UCRT64** terminal.
3. Install the compiler and GTK4:
   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gtk4 mingw-w64-ucrt-x86_64-pkgconf make
   ```
4. Open the project folder from that terminal, then run:
   ```bash
   make
   ./smart-city-routes.exe
   ```
   If `make` is unavailable, install it with `pacman -S make`.

## macOS (optional)
With Homebrew:
```bash
brew install gtk4 pkg-config
make
./smart-city-routes
```

## Project layout
- `src/main.c` — GTK interface, graph data, and algorithm implementations
- `data/city_roads.csv` — readable copy of the sample road network
- `tests/manual_test_cases.md` — manual test plan and expected outcomes
- `docs/PBL_Report.md` — editable college-style report
- `docs/diagrams.md` — editable Mermaid diagrams

## Important interpretation
Distances are fictional sample values, not real navigation data. The traffic toggle is a simple demonstration rule, not a live traffic feed. The geometric A* heuristic is used for demonstration; the supplied edge costs and coordinates are not a calibrated real-world map, so do not present it as a production navigation engine. BFS/DFS results report edge count, not kilometres.

## Testing status
This package contains manual test cases and expected outcomes. Compilation and GUI execution have not been verified in this environment. Run the build instructions and record your own screenshots and actual results before submission.
