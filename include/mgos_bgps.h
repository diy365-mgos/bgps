/*
 * Copyright (c) 2022 DIY365
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MGOS_BGPS_H_
#define MGOS_BGPS_H_

#include <stdbool.h>

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */ 

#ifdef __cplusplus
extern "C" {
#endif

#define MGOS_BGPS_EVENT_BASE MGOS_EVENT_BASE('G', 'P', 'S')
#define MGOS_EV_BGPS_ANY MGOS_BGPS_EVENT_BASE
enum mgos_bthing_event {
  MGOS_EV_BGPS_POSITION_CHANGED = MGOS_BGPS_EVENT_BASE,
  MGOS_EV_BGPS_ALTITUDE_CHANGED
};

struct mgos_bgps_location {
  float latitude;
  float longitude;
};

struct mgos_bgps_position {
  struct mgos_bgps_location location;
  float accuracy;
};

struct mgos_bgps_position_changed {
  bool init;
  struct mgos_bgps_position prev_pos;
  struct mgos_bgps_position cur_pos;
};

bool mgos_bgps_get_position(struct mgos_bgps_position *position);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_BGPS_H_ */
