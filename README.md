# qsim — Quantum Computing Concepts, Visualized

A from-scratch quantum computing simulator in C, rendered with raylib. No
quantum SDKs, no external math libraries — every gate, every measurement,
every collapse is hand-implemented on raw complex amplitudes, then mapped to
a live 3D Bloch sphere so you can *watch* the math happen instead of just
reading numbers.

Started as a single-qubit superposition demo and grew, concept by concept,
into a working tour of the core ideas behind quantum computing: superposition,
entanglement, interference, and two of the field's signature protocols
(Deutsch-Jozsa and teleportation), plus Grover's search and a decoherence
model.

## Why this exists

Bloch spheres and bra-ket notation are easy to read and hard to *feel*.
Watching a red dot snap to the sphere's center the instant you entangle two
qubits, or watching a probability bar visibly cancel to zero from
interference, makes these concepts land in a way equations alone don't.
Every visual here is driven directly by the underlying amplitude math — there
is no separate "animation logic" faking the physics.

---

## Building

Requires a MinGW/GCC toolchain and raylib (prebuilt `include/` + `lib/`
distribution, or built from source).

```bash
mingw32-make RAYLIB_PATH=/c/raylib
./build/qsim.exe
```

If `raylib` lives somewhere else, point `RAYLIB_PATH` at the folder that
directly contains its `include/` and `lib/` subfolders.

---

## Controls

| Key | Action |
|---|---|
| `H` `X` `Y` `Z` | Apply gate to qubit 0 |
| `Shift + H/X/Y/Z` | Apply gate to qubit 1 |
| `C` | CNOT, control=qubit 0, target=qubit 1 |
| `M` | Measure (collapses the active state) |
| `R` | Reset everything |
| `J` | Run Deutsch-Jozsa with a random oracle |
| `P` | Prepare a message qubit for teleportation |
| `K` | Run the teleportation protocol |
| `G` | Prepare a GHZ state (3-qubit entanglement) |
| `V` | Grover's search — first press initializes, second runs one iteration |
| `N` | Toggle decoherence noise on qubit 0 |
| `↑` / `↓` | Adjust decoherence noise strength while active |
| Mouse drag | Orbit the camera |

The three spheres on screen show **qubit 0**, **qubit 1**, and a third slot
that displays either the **teleportation target** or, during a GHZ
demonstration, **qubit 2** of the three-qubit register.

---

## What each concept demonstrates

### Superposition (`H`, `X`, `Y`, `Z`, `S`, `T`)

A single qubit is `α|0⟩ + β|1⟩` — not "secretly 0 or 1," but a genuine blend
described by two complex amplitudes. The Bloch sphere position is computed
directly from `α` and `β` (`qubit_to_bloch` / `twoqubit_reduced_bloch`), so
watching the dot move *is* watching the amplitudes change. `H` puts a qubit
into equal superposition; `X/Y/Z` are the Pauli flips; `S/T` are phase gates
that only become visible once you're already in superposition (a phase gate
applied to a pure `|0⟩` state does nothing observable — try it and see).

### Entanglement (`C` for a Bell pair, `G` for GHZ)

Two qubits are entangled when their joint state can't be factored into two
independent single-qubit states. The visual signature: press `H` then `C` and
watch qubit 1's dot **shrink toward the center of its sphere**. That's not a
bug — an entangled qubit has no well-defined state of its own (`concurrence`
in the HUD reads `1.00`, confirmed via the reduced density matrix computed by
partial trace in `twoqubit_reduced_bloch`). Measure with `M` and the two
outcomes always agree, even though each one alone was 50/50 random the
instant before.

`G` extends this to three qubits (`ThreeQubit`), producing a GHZ state
`(|000⟩ + |111⟩)/√2`. Unlike a Bell pair, no two of the three qubits are
pairwise entangled with each other in isolation — the correlation only shows
up across all three at once. Measuring always yields `000` or `111`, never a
mix, which the HUD explicitly checks and reports.

### Interference (repeated `H`, and the amplitude bar panel)

Interference is what turns superposition from "a weird fact about particles"
into "a computational resource." Press `H` once — the `|00⟩` and `|10⟩` bars
both rise to `0.50`. Press `H` again and watch `|10⟩` collapse back to `0.00`
while `|00⟩` returns to `1.00` — not from measurement, but because the two
paths through the second Hadamard arrive with opposite signs and cancel. The
bar color (blue = positive amplitude, orange = negative) makes that sign
directly visible, which is normally the hardest part of interference to
convey without equations.

### The Deutsch-Jozsa algorithm (`J`)

The smallest algorithm that shows superposition + interference doing
something a classical computer can't do as cheaply: determine whether a
hidden function is *constant* or *balanced* using a single query, instead of
the two a classical approach would need in the worst case. A random oracle is
chosen and applied; the final measurement deterministically reveals which
type it was. The HUD reports the oracle definition and whether the algorithm's
verdict was correct — it always is.

### Quantum teleportation (`P` then `K`)

Transfers an arbitrary qubit state from one qubit to another using only a
shared entangled pair and two classical bits — without moving the physical
qubit and without cloning it (the original is destroyed by measurement the
instant the protocol runs). `P` prepares a message state on qubit 0 of a
3-qubit register (`ThreeQubit`); `K` runs the protocol (`teleport_run`):
entangling a Bell pair, performing a Bell-basis measurement on the message
qubit and Alice's half, then applying classically-conditioned `X`/`Z`
corrections to Bob's qubit. The HUD prints both Bloch vectors so you can
directly confirm they match — proof the state moved, not just a claim.

### Grover's search (`V`, `V` again)

Amplitude amplification: given a "marked" answer hidden among 4 possibilities
(2 qubits), classical guessing finds it with 25% confidence per try. The
oracle flips the marked amplitude's sign (invisible to probability alone,
since flipping a sign doesn't change `|amplitude|²`); the diffusion operator
then reflects every amplitude about their average, converting that hidden
sign flip into a real probability boost. For this small a search space, one
iteration is enough to bring the marked state to *exactly* 100% — watch the
amplitude bars jump from a uniform `0.25` each to one bar at `1.00`.

### Decoherence (`N`, `↑`/`↓`)

Real qubits aren't isolated from their environment, and that contact
destroys quantum behavior over time — no measurement required. `N` applies a
small random phase kick plus gradual amplitude damping to qubit 0 every
frame, model-simplified but representative of dephasing and energy
relaxation. Put a qubit in superposition (`H`), then turn noise on, and watch
the dot drift and shrink toward the sphere's center purely from simulated
environmental interaction — this is the practical enemy every real quantum
computer is built to fight off.

---

## Architecture

```
qsim/
├── Makefile
├── README.md
├── include/
│   ├── complex_math.h    # 2x2 complex matrix type + apply/multiply
│   ├── gates.h            # H, X, Y, Z, S, T + parametric Rx/Ry/Rz
│   ├── twoqubit.h         # 2-qubit state (4 amplitudes), CNOT, partial trace
│   ├── threequbit.h       # 3-qubit state (8 amplitudes), single-qubit
│   │                       #   partial measurement — needed for teleport/GHZ
│   ├── oracle.h / dj.h    # Deutsch-Jozsa oracles + algorithm runner
│   ├── teleport.h         # Teleportation protocol runner
│   ├── ghz.h              # GHZ state preparation
│   ├── grover.h           # Grover oracle + diffusion operator
│   ├── decoherence.h      # Per-frame noise injection
│   ├── bars.h             # Amplitude probability-bar visualization
│   └── render.h           # Bloch sphere rendering + spherical interpolation
├── src/                   # implementations, one file per header above
└── build/                 # compiled output (gitignored)
```

Two state representations coexist deliberately: `TwoQubit` (4 amplitudes)
drives the always-visible qubit 0 / qubit 1 spheres and everything built on
pairwise interaction (Bell pairs, Deutsch-Jozsa, Grover, decoherence).
`ThreeQubit` (8 amplitudes) exists specifically because teleportation and GHZ
need a third qubit — Alice's message, her half of a Bell pair, and Bob's
receiving qubit can't fit in a 2-qubit register. Both share the same
`Complex2x2` gate-application pattern, just generalized to more amplitude
indices via bit-masking (see `qbit_weight` in `threequbit.c`).

Every Bloch sphere position is a **reduced state** via partial trace
(`twoqubit_reduced_bloch` / `threequbit_reduced_bloch`), not a raw amplitude
readout — this is what makes entanglement visually honest: an entangled
qubit's reduced state is genuinely mixed (shrunk toward the sphere's center),
not just "displayed oddly."

Dots animate via `bloch_slerp`, which interpolates spherically (and
separately handles vector *length*, since reduced states can have length
less than 1) so state transitions read as continuous motion along the
sphere's surface rather than teleporting between frames.
