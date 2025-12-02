#pragma once

namespace djnn {
class FatProcess;
extern FatProcess* default_audio_listener;

void init_audio ();
void clear_audio ();
} // namespace djnn
