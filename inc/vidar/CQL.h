#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#include <cassandra.h>
#pragma clang diagnostic pop

#include "vidar/Cluster.h"
#include "vidar/Client.h"
#include "vidar/ConnectionInfo.h"
#include "vidar/OnCompleteCallback.h"
#include "vidar/Prepared.h"
#include "vidar/Result.h"
#include "vidar/ResultIterator.h"
#include "vidar/Row.h"
#include "vidar/Column.h"
#include "vidar/Statement.h"
