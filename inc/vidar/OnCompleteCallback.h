#pragma once

#include "vidar/Result.h"

namespace vidar
{

typedef void (*OnCompleteCallback)(Result result, void* data) -> void;

} // namespace vidar
