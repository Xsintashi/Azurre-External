#pragma once

#include <optional>

struct GlobalVars
{
   float realTime;
   int frameCount;
   float absoluteFrameTime;
   float absoluteFrameStartTimeStdDev;
   float currentTime;
   float frameTime;
   int maxClients;
   int tickCount;
   float intervalPerTick;
   float interpolationAmount;
};

inline std::optional <GlobalVars> globalVars;