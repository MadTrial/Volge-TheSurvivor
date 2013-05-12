#pragma once

#include "../stdafx.h"

#include "Interface/IGame.hpp"
#include "Interface/IPhysicsSystem.hpp"

#include "Manager/GameState.hpp"
#include "Manager/Resource.hpp"
#include "Manager/Controller.hpp"
#include "Manager/Exceptions.hpp"

#include "Utility/ConfigParser.hpp"
#include "Utility/DrawBatch.hpp"

#include "Graphics/Sprite.hpp"

#include "3rdParty/FrameClock/FrameClock.hpp"
#include "3rdParty/FrameClock/ClockHUD.hpp"
#include "3rdParty/b2DebugDraw/DebugDraw.hpp"

#define DEGTORAD (b2_pi /180.0f)
#define RADTODEG (180.0f/b2_pi)
#define RATIO 30.f
#define UNRATIO (1.0f/30.f)
