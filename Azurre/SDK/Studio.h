#pragma once
#include "Pad.h"
#include "Vector.h"

struct StudioBone {
	int nameIndex;
	int	parent;
	PAD(152)
	int flags;
	PAD(52)

	const char* getName() const noexcept {
		return nameIndex ? reinterpret_cast<const char*>(std::uintptr_t(this) + nameIndex) : nullptr;
	}
};

struct StudioHdr {
    int id;
    int version;
    int checksum;
    char name[64];
    int length;
    Vector eyePosition;
    Vector illumPosition;
    Vector hullMin;
    Vector hullMax;
    Vector bbMin;
    Vector bbMax;
    int flags;
    int numBones;
    int boneIndex;
    int numBoneControllers;
    int boneControllerIndex;
    int numHitboxSets;
    int hitboxSetIndex;

    const StudioBone* getBone(int i) const noexcept
    {
        return i >= 0 && i < numBones ? reinterpret_cast<StudioBone*>(std::uintptr_t(this) + boneIndex) + i : nullptr;
    }
};