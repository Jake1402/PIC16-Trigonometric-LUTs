#ifndef FAST_MATH_H
#define	FAST_MATH_H

#define SCALE 100   // Scale of trig function
//#define PRECISION   // Decide to prefer precision or memory

#define min(x, y) x<y?x:y                                                       // Returns min of two values
#define max(x, y) x>y?x:y                                                       // Returns max of two values
#define clamp(x, lB, uB) max(lB, min(x, uB))                                    // Clamps a value to given range
#define abs(x) ((x) < 0 ? -(x) : (x))                                           // Returns the absolute of a value
#define map(x, fL, fH, tL, tH) (x - fL) * (tH - tL) / (fH - fL) + tL            // Maps a value to a given range
#define len(x)   (sizeof(x) / sizeof(x[0]))                                     // Returns length of an array
#define term_len(x) ((sizeof(x) / sizeof(x[0])) - 1)                            // Returns length of an array - 1

const char sin_x[];                              // LUT of sinx scaled by 100 (int math)
const char theta[];                              // Corrosponding theta values of sin
#define LUT_STEP  (90 / (term_len(sin_x) - 1))   // Calculates correct step size
#define SIN_SIZE len(sin_x)                      // Size of LUT

int interpolate(int x, int x1, int x2, int y1, int y2); // Interpolation function returns an approximation between two points
int lut_sin(int angle);                                 // Returns an interpolated sin value from a look up table
int lut_asin(int sin_val);
int lut_cos(int angle);                                 // Calls lut_sin(angle + 90) gives result as a cosine  
int lut_acos(int cos_val);

int fastRoot(int square);
signed int magnitude(int x, int y, int z);

#endif	/* FAST_MATH_H */

