# Chronicle — Ironhold World Engine

Chronicle is the backend world-state engine for **Ironhold**, an open-world
survival game. It's being built milestone by milestone for the Systems
Programming Club hackathon: starting as a debug console and growing into a
persistent, crash-safe, query-able world simulator for thousands of
concurrent entities.

Built in C++20 using only the standard library.

## Branches

Each milestone lives on its own branch, and each one builds on the last
(no rewrites — features accumulate).

| Branch | Milestone | What it adds |
|--------|-----------|---------------|
| `M1`   | The Game Debug Console | REPL loop, `.quit`/`.help`/`.version`/`.status`, world-command stub |
| `M2`   | Entity Commands and the World State | `SPAWN PLAYER`, `LIST PLAYERS`, in-memory `WorldState`, validation |
| `M3`   | The Persistent World | Binary `PlayerRecord` layout, paged `Pager`, disk-backed `WorldState`, world file survives restarts |

`main`/default branch tracking isn't set up yet — check out the branch for
the milestone you want:

```bash
git clone https://github.com/Amirtha-yazhini/chronicle.git
cd chronicle
git checkout M3   # or M1 / M2
```

## Building

Requires a C++20-capable compiler (e.g. `g++`) and `make`.

```bash
cd chronicle
make            # builds ./chronicle
make clean      # removes the binary
```

The Makefile's source list grows with each milestone:
- **M1**: `main.cpp console.cpp input_buffer.cpp`
- **M2**: adds `command.cpp`
- **M3**: adds `entity.cpp pager.cpp`

## Running

**M1 / M2** (no world file yet):
```bash
./chronicle
```

**M3** (world persists to a file):
```bash
./chronicle ironhold.world   # creates the file if it doesn't exist
```
If no argument is given, `ironhold.world` in the current directory is used
by default.

### System commands (all branches)
| Command    | Description                          |
|------------|---------------------------------------|
| `.quit`    | Flush state and exit (M3: also closes the Pager) |
| `.help`    | List available commands               |
| `.version` | Print engine version                  |
| `.status`  | Print current world status            |

### World commands
- **M1**: any non-dot input is echoed back as `World command queued: ...`
  (stub only, nothing is executed).
- **M2/M3**: `SPAWN PLAYER <id> <username> <email>` and `LIST PLAYERS` are
  parsed and executed against `WorldState`.

## Milestone details

### M1 — Debug Console
- REPL in `src/console.cpp` prompts (`ironclad>`), reads a line, and
  dispatches on a leading `.`.
- `InputBuffer` (`src/input_buffer.cpp`) trims whitespace and logs the
  session's command history to `ironclad.log` on `.quit`.
- Known gap: `application_state.hpp` is a placeholder — the
  `AppState`/`MetaResult` enum from the spec isn't wired in as a real type
  yet; dispatch is done with plain string comparisons instead.

### M2 — Entity Commands and World State
- `include/Engine.hpp` defines `Player`, `WorldState` (in-memory
  `std::vector<Player>`), `Command`, and `ExecResult`.
- `src/command.cpp` parses `SPAWN PLAYER` / `LIST PLAYERS` and validates:
  - id must be a positive integer (rejects `0` and negatives)
  - id must not already exist in the world (duplicate check)
  - username ≤ 32 chars, email ≤ 255 chars (both required, non-empty)
- Console switched from `cin >> in` (M1) to `getline`, so multi-word
  commands now parse correctly.
- World state is **not persisted** yet — restarting the binary wipes it
  (that's M3).

### M3 — The Persistent World
- `include/entity.hpp` defines a packed, exactly-291-byte `PlayerRecord`
  (`uint32_t id` + `char[32]` username + `char[255]` email), with a
  `static_assert` enforcing the size.
- `include/pager.hpp` / `src/pager.cpp` implement a `Pager` over
  4096-byte pages (`ENTITIES_PER_PAGE = 14`, `MAX_PAGES = 100`,
  `MAX_ENTITIES = 1400`). Pages are lazily loaded from disk on first
  access and flushed on `close()`.
- `WorldState` (in `Engine.hpp`) now wraps a `Pager` instead of a
  `std::vector`, and reconstructs `num_entities` from the world file's
  size on startup.
- `.status` reports `World: online — <file> (<N> entities, <N> pages)`
  as specified.
- The world file path comes from `argv[1]`; `main()`/`console()` now take
  `argc, argv`.


