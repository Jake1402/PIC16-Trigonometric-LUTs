# PIC16-Trigonometric-LUTs
A simple trigonometric look-up table (LUT) for PIC microcontrollers that gets you good accuracy using **only integer maths**. On an 8-bit PIC, calling a floating point `sin()`/`cos()` is slow and eats a lot of RAM, so instead these functions read pre-computed values off a tiny table and — if you want the extra precision — linearly interpolate between the two nearest points.

Two options are available through a single macro definition (`PRECISION`) which lets you either prioritise **memory space** or **accuracy**. Both versions are pure integer so they'll run happily on any PIC in the 16-bit family.

## How it works
All of the trig values are stored as integers scaled by `SCALE` (which is `100`), so a "real" sine of `0.707` comes back as `70`. Angles are always passed in **degrees**.

The table only stores the first quadrant (`0°` to `90°`) — just 31 entries, one every `3°` — and then uses symmetry/reflection to map any angle from `0°` to `360°` back into that range. This is what keeps the whole thing so small in memory. `lut_cos` doesn't even need its own table, it's just `lut_sin(angle + 90)` thanks to the identity `cos(x) = sin(x + 90)`.

## The two modes
You pick your mode by defining (or not defining) `PRECISION` before you include the header.

**Memory mode (default)** — direct table lookup, no interpolation. This is the fastest and smallest option but the result "steps" between table points, so there's a small amount of error at most angles.

**Precision mode** — linear interpolation between the two nearest table points using the `interp()` helper. Noticeably more accurate, at the cost of a couple of extra integer multiply/divides per call.

To switch modes just uncomment this line in [fast_math.h](fast_math.h):
```c
#define PRECISION   // Decide to prefer precision or memory
```

## API
The main functions live in [fast_math.c](fast_math.c) and are declared in [fast_math.h](fast_math.h):
```c
int lut_sin(int angle);      // Sine of an angle (degrees), scaled by 100
int lut_cos(int angle);      // Cosine, implemented as lut_sin(angle + 90)
int lut_asin(int sin_val);   // Inverse sine, returns the angle in degrees
int lut_acos(int cos_val);   // Declared for now, not implemented yet

int fastRoot(int square);              // Integer square root (Newton-Raphson style)
signed int magnitude(int x, int y, int z);  // 3D vector length using fastRoot
```
There are also a handful of utility macros in the header (`min`, `max`, `clamp`, `abs`, `map`, `len`, `term_len`) that keep the LUT maths and any surrounding code a bit cleaner.

## Usage
Drop both files into your project, include the header, and call the functions like any other:
```c
#include "fast_math.h"

void main(void) {
    int s = lut_sin(45);    // ~70  (i.e. 0.70)
    int c = lut_cos(45);    // ~70  (i.e. 0.70)
    int a = lut_asin(100);  // 90

    // Remember everything is scaled by 100, so divide to get the real value:
    float real_sin = s / 100.0f;   // ~0.70 (only if you actually need a float)
}
```

## Notes
- **Scale:** every trig result is multiplied by `SCALE` (`100`). Divide by `100` to get the normal `-1.0` to `1.0` range.
- **Integer only:** no floating point anywhere, so it's cheap to run on a PIC.
- **Angles in degrees**, results scaled by 100.
- In precision mode there's an edge case right at `90°` / `270°` where the lookup index runs one past the end of the table — worth keeping in mind if you hit exactly those values (noted as a fix for later below).

## Future plans
A few things I plan to do with this:
- Fix the small out-of-bounds read at `90°` / `270°` in precision mode.
- Implement `lut_acos` (it's declared but not finished yet).
- Add a proper test/benchmark sketch so the accuracy of each mode is easy to check.
- Maybe add a higher-resolution table option for people who want even more precision.