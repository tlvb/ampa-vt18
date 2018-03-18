#ifndef _dmx_h_
#define _dmx_h_

#include "light.h"
#include "network.h"

void write_dmx_statics(conduit *con, const fixture *fix);
void write_dmx_dynamics(conduit *con, const fixture *fix);

#endif
