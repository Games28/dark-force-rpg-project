#ifndef  DEFS_H
#define DEFS_H

#define PI 3.1415926535f

// Screen and pixel constants - keep the screen sizes constant and vary the resolution by adapting the pixel size
// to prevent accidentally defining too large a window
#define SCREEN_X   1280
#define SCREEN_Y    800
#define PIXEL_X       2
#define PIXEL_Y       2


#define STRETCHED_TEXTURING  false  // if true, multiple levels are stretched textured. If false: per 1x1x1 block
#define MULTIPLE_LEVELS      true
#define RENDER_CEILING       !MULTIPLE_LEVELS    // render ceilings only for single level world

#define MOUSE_CONTROL        false

// shading constants
#define RENDER_SHADED        true
#define OBJECT_INTENSITY       4.5f        // Joseph21 - changed from org value 0.5f
#define MULTIPLIER_INTENSITY   5.0f
#define INTENSITY_SPEED        1.0f

#define SHADE_FACTOR_MIN  0.1f      // the shade factor is clamped between these two values
#define SHADE_FACTOR_MAX  1.0f

// colour constants
#define ROOF_COLOUR  olc::RED
#define TEXT_COLOUR  olc::YELLOW
#define SHADE_COLOUR olc::BLACK    // white for fog effect, black for night effect

// constants for speed movements - all movements are modulated with fElapsedTime
#define SPEED_ROTATE      60.0f   //                            60 degrees per second
#define SPEED_MOVE         5.0f   // forward and backward    -   5 units per second
#define SPEED_STRAFE       5.0f   // left and right strafing -   5 units per second
#define SPEED_LOOKUP     200.0f   // looking up or down      - 200 pixels per second
#define SPEED_STRAFE_UP    1.0f   // flying or chroucing     -   1.0 block per second

// mini map constants

#define MINIMAP_TILE_SIZE 32
#define MINIMAP_SCALE_FACTOR 0.2

#define SIGNIFICANCE 3
#define SIG_POW10 1000



    // constants for the different block types
#define GRND_FLOOR '.'     // no block
#define FRST_FLOOR '#'     // block of height 1
#define SCND_FLOOR '@'     //                 2
#define THRD_FLOOR '*'     //                 3
#define FRTH_FLOOR '-'     //                 4
#define FFTH_FLOOR '+'     //                 5
#define SXTH_FLOOR '='     //                 6

#define FLOOR_1QRTR 'Q'    // block of height 1/4
#define FLOOR_HALVE 'H'    //                 2/4
#define FLOOR_3QRTR 'T'    //                 3/4



// var's and initial values for shading



#endif // ! DEFS_H
