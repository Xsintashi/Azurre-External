#pragma once
#include "Matrix.h"
#include "Vector.h"

struct MStudioHitboxSet
{
    int nameIndex;
    int numHitboxes;
    int hitboxIndex;
};

struct MStudioBbox
{
    int bone;
    int group;                   // intersection group
    Vector bbmin;               // bounding box
    Vector bbmax;
    int szhitboxnameindex;       // offset to the name of the hitbox.
    int unused[3];
    float radius;                // when radius is -1 it's box, otherwise it's capsule (cylinder with spheres on the end)
    int pad[4];
};

struct MStudioBone
{
    int nameIndex;
    int parent; // parent bone
    int boneController[6];     // bone controller index, -1 == none
    Vector pos;
    float quat[4];
    Vector rot;
    Vector posscale;
    Vector rotscale;
    Matrix3x4 poseToBone;
    float alignment[4];
    int flags;
    int procType;
    int procIndex;                   // procedural rule
    int physicsBone;                 // index into physically simulated bone
    int surfacePropIdx;              // index into string tablefor property name
    int contents;                    // See BSPFlags.h for the contents flags
    int unused[8];             // remove as appropriat
};