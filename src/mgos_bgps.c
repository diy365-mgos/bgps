#include "mgos.h"
#include "mgos_bgps.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

static void mg_bgps_timer_cb(void *arg) {
  float latitude;
  float longitude;
  float accuracy;

  if (mgos_bgps_get_position(&latitude, &longitude, &accuracy)) {
    LOG(LL_INFO, ("GPS: lat %f, lng %f, accuracy %f", latitude, longitude, accuracy));
  } else {
    LOG(LL_ERROR, ("Error updating GPS position"));
  }

  (void) arg;
}

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */

bool mgos_bgps_init() {

  // Start the polling interval
  if (mgos_sys_config_get_gps_update_interval() > 0) {
    mgos_set_timer(mgos_sys_config_get_gps_update_interval(),
      MGOS_TIMER_REPEAT, mg_bgps_timer_cb, NULL);
  }

  return true;
}