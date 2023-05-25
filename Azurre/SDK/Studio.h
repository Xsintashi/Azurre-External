#pragma once
#include "Pad.h"
#include "Matrix.h"
#include "Vector.h"

class Quaternion {};

struct Studiobone
{
	int sznameindex;
	int parent; // parent bone
	int bonecontroller[6];     // bone controller index, -1 == none
	Vector pos;
	Quaternion quat;
	Vector rot;
	Vector posscale;
	Vector rotscale;
	Matrix3x4 poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex;                   // procedural rule
	int physicsbone;                 // index into physically simulated bone
	int surfacepropidx;              // index into string tablefor property name
	int contents;                    // See BSPFlags.h for the contents flags
	int unused[8];             // remove as appropriat
};

struct StudioHdr {
	int id;
	int version;
	int checksum;
	PAD(64)
	int length;
	Vector eyeposition;
	Vector illumposition;
	Vector hull_min;
	Vector hull_max;
	Vector view_bbmin;
	Vector view_bbmax;
	int flags;
	int numbones;                // bones
	int boneindex;
	int numbonecontrollers;      // bone controllers
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
};