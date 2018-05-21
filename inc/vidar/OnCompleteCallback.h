#pragma once

#include "vidar/Result.h"

namespace vidar
{

class Result;

/**
 * All queries that complete successfully or timeout will have this callback signature called.
 * @param result The query's result.
 * @param user_data Custom user data passed through Client::ExecuteQuery for this query.
 */
using OnCompleteCallback = auto (*)(Result result, void* user_data) -> void;

} // namespace vidar
