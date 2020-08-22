/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   background_png;
    const int            background_pngSize = 65490;

    extern const char*   combo_box_png;
    const int            combo_box_pngSize = 12243;

    extern const char*   display_area_png;
    const int            display_area_pngSize = 12243;

    extern const char*   knob_png;
    const int            knob_pngSize = 70946;

    extern const char*   slider_gap_png;
    const int            slider_gap_pngSize = 1602;

    extern const char*   slider_handle_png;
    const int            slider_handle_pngSize = 11735;

    extern const char*   switch_knob_png;
    const int            switch_knob_pngSize = 44818;

    extern const char*   toggle_button_png;
    const int            toggle_button_pngSize = 7301;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
