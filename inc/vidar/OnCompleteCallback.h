#pragma once

#include "vidar/Result.h"

namespace vidar
{

class Result;

using OnCompleteCallback = auto (*)(Result result, void* data) -> void;

} // namespace vidar
