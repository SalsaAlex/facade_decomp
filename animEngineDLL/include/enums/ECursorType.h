#ifndef ENUM_CURSORTYPE_H
#define ENUM_CURSORTYPE_H

//im not sure why these values 
enum ECursorType
{
    //there are so much more unused cursors that i wont even bother putting all them here
    //used cursors are labeled as icons in resource files, weirdly enough,
    //and the game only uses the cursors labeled as icons for some reason..?

    eCursorType_IDC_ARROW = -100,
    eCursorType_IDC_WAIT,
    eCursorType_IDC_SIZEWE,
    eCursorType_IDC_SIZENS,
    eCursorType_IDC_SIZENWSE,
    eCursorType_IDC_SIZENESW,


    eCursorType_Idle = 0,
    eCursorType_Holding,
    eCursorType_Comfort,
    eCursorType_Hug,
    eCursorType_Kiss,
    eCursorType_Throw,
    eCursorType_Knock,
    eCursorType_Sip,

    eCursorType_unknown1_animframe1 = 20,
    eCursorType_unknown1_animframe2,
    eCursorType_unknown1_animframe3,
    eCursorType_unknown1_animframe4,
    eCursorType_unknown1_animframe5,

    eCursorType_unknown2_animframe1 = 30,
    eCursorType_unknown2_animframe2,
    eCursorType_unknown2_animframe3,
    eCursorType_unknown2_animframe4,
    eCursorType_unknown2_animframe5,
    eCursorType_unknown2_animframe6,
    eCursorType_unknown2_animframe7,
    eCursorType_unknown2_animframe8,
};

#endif // ENUM_CURSORTYPE_H