#pragma once
#include "Pad.h"
#include "Matrix.h"
#include "Vector.h"

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