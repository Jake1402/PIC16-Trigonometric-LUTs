#include "fast_math.h"

const char sin_x[] = {
    0,
    5,
    10,
    16,
    21,
    26,
    31,
    36,
    41,
    45,
    50,
    54,
    59,
    63,
    67,
    71,
    74,
    78,
    81,
    84,
    87,
    89,
    91,
    93,
    95,
    97,
    98,
    99,
    99,
    100,
    100
};  // LUT of sinx DIVIDE BY 100
const char theta[] = {
    0,
    3,
    6,
    9,
    12,
    15,
    18,
    21,
    24,
    27,
    30,
    33,
    36,
    39,
    42,
    45,
    48,
    51,
    54,
    57,
    60,
    63,
    66,
    69,
    72,
    75,
    78,
    81,
    84,
    87,
    90
};  // Corrosponding theta values 

#ifdef PRECISION                                    // Check for precision mode

    int interp(int x, int x1, int x2, int y1, int y2){  // Linear interpolate between two points
        int denom = (x2 - x1);                          // Calculate the denominator
        if((x2 - x1) == 0) { return x; }                // If it is zero, return x
        return (                                        // Calculate the interpolated result
            y1 + (int)((y2 - y1)*(x - x1)/denom)        // this gives an estimate between
        );                                              // the two data points.
    }                                                   // Use sparingly.


    int lut_sin(int angle) {                        // Exactly the same but uses 
        while (angle >= 360) angle -= 360;          // Interpolation between two points
        while (angle < 0)    angle += 360; 
        if (angle <= 90) {
            return interp(
                angle, 
                theta[angle / LUT_STEP],
                theta[(angle / LUT_STEP) + 1],
                sin_x[angle / LUT_STEP],
                sin_x[(angle / LUT_STEP) + 1]
            );
        } else if (angle <= 180) {
            return interp(
                (180 - angle), 
                theta[(180 - angle) / LUT_STEP],
                theta[((180 - angle) / LUT_STEP) + 1],
                sin_x[(180 - angle) / LUT_STEP],
                sin_x[((180 - angle) / LUT_STEP) + 1]
            );
        } else if (angle <= 270) {
            return -interp(
                (angle - 180), 
                theta[(angle - 180) / LUT_STEP],
                theta[((angle - 180) / LUT_STEP) + 1],
                sin_x[(angle - 180) / LUT_STEP],
                sin_x[((angle - 180) / LUT_STEP) + 1]
            );
        } else {
            return -interp(
                (360 - angle), 
                theta[(360 - angle) / LUT_STEP],
                theta[(360 - angle) / LUT_STEP + 1],
                sin_x[(360 - angle) / LUT_STEP],
                sin_x[(360 - angle) / LUT_STEP + 1]
            );
        }
    }
    
    int lut_asin(int sin_val){
        char negative = 0;
        if (sin_val < 0) {          // Handle negative input
            negative = 1;
            sin_val = -sin_val;     // Work with positive value, flip at end
        }
        if (sin_val > 100) sin_val = 100;   // Clamp to valid range
        
        int i = 0;
        while (i < 30 && sin_x[i + 1] <= sin_val) i++;

        int angle = interp(
            sin_val,
            sin_x[i],
            sin_x[i + 1],
            theta[i],
            theta[i + 1]
        );
        
        return negative ? -angle : angle;
    }
    
#else                                                   // If we're not in precision use this one
    int lut_sin(int angle) {                            // Look-Up-Table-Sin
        while (angle >= 360) angle -= 360;              // Ensure the angle is in an appropriate
        while (angle < 0)    angle += 360;              // range. This method avoids modulo

        if (angle <= 90) {                              // If angle is in the first quadrant
            return sin_x[angle / LUT_STEP];             // Return the corresponding, divide by step
        } else if (angle <= 180) {                      // If angle is in the first quadrant
            return sin_x[(180 - angle) / LUT_STEP];     // Return the corresponding, divide by step
        } else if (angle <= 270) {                      // If angle is in the first quadrant
            return -sin_x[(angle - 180) / LUT_STEP];    // Return the corresponding, divide by step
        } else {                                        // If angle is in the first quadrant
            return -sin_x[(360 - angle) / LUT_STEP];    // Return the corresponding, divide by step
        }                                              
    }
    
    int lut_asin(int sin_val){
        char negative = 0;
        if (sin_val < 0) {          // Handle negative input
            negative = 1;
            sin_val = -sin_val;     // Work with positive value, flip at end
        }
        if (sin_val > 100) sin_val = 100;   // Clamp to valid range
        
        int i = 0;
        while (i < 30 && sin_x[i] < sin_val) i++;
        int angle = theta[i];
        
        return negative ? -angle : angle;
    }
    
#endif 

int lut_cos(int angle){         // Returns the cosine of an angle
    return lut_sin(angle + 90); // A cos is an identity of sin(x+90)
}                               

int fastRoot(int square){
    int iter = (square / 2) - (((square*square / 4) - square) / ((square / 2) * 2));
    iter = iter - (((iter*iter) - square) / (iter * 2));
    return (iter - (((iter*iter) - square) / (iter * 2)));
};

signed int magnitude(int x, int y, int z){
    return fastRoot(
            (x*x) + (y*y) + (z*z)
    );
};

