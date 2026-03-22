# IDE Setup

## Geany

The `steampunkclock.geany` project file is provided in the project root.

### Prerequisites
- Geany installed on the host
- Container running: `podman-compose up -d` (from the topicfs directory)
- Project built at least once: Build SteampunkClock from the Build menu

### Setup
1. Open Geany
2. Open the workbench file: Tools → Workbench → Open
3. Select `smithjack.geanywb` — this file lives one level above the SteampunkClock
   repository root, in the same directory where you cloned the project
4. Double-click SteampunkClock in the Workbench tab

## Other IDEs

Contributions welcome. Please add a subdirectory here with configuration
files and a README explaining the setup process.
