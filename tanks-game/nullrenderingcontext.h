#pragma once
#include "renderingcontext.h"

class NullRenderableObject : public AbstractRenderableObject {

};

//only making this a thing because: 1. should be *very* easy; 2. Dolphin does it
class NullRenderingContext : public RenderingContext {

};
