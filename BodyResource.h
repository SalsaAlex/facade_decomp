#pragma once

enum BodyResource : int
{
    eBodyResourceID_NOTARESOURCE = -1,
    eBodyResourceID_legsBody = 0,
    eBodyResourceID_armL = 1,
    eBodyResourceID_armR = 2,
    eBodyResourceID_gaze = 3,
    eBodyResourceID_gazeTorso = 4,
    eBodyResourceID_voice = 5,
    eBodyResourceID_faceExpressionBase = 6,
    eBodyResourceID_faceExpressionMood = 7,
    eBodyResourceID_facialMod = 8,
    eBodyResourcePriority_NOTAPRIORITY = -1,
    eBodyResourcePriority_ifNotBusy = 0,
    eBodyResourcePriority_always = 2147483647, //Integer.MAX_VALUE
    eBodyResourceAction_fail = 0,
    eBodyResourceAction_block = 1,
};