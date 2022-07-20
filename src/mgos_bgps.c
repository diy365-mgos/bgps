#include "mgos.h"
#include "mgos_bgps.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

static struct mgos_bgps_position_changed s_pos_changed;

static void mg_bgps_auto_update_timer_cb(void *arg) {
  struct mgos_bgps_position tmp_pos;
  if (!mgos_bgps_get_position(&tmp_pos)) {
    LOG(LL_ERROR, ("Error updating GPS position"));
    return;
  }

  bool is_changed = (s_pos_changed.cur_pos.location.latitude != tmp_pos.location.latitude ||
                     s_pos_changed.cur_pos.location.longitude != tmp_pos.location.longitude);

  if (s_pos_changed.init || is_changed) {
    // save current position into the prev one
    memcpy(&s_pos_changed.prev_pos, &s_pos_changed.cur_pos, sizeof(struct mgos_bgps_position));
    // update the current position
    memcpy(&s_pos_changed.cur_pos, &tmp_pos, sizeof(struct mgos_bgps_position));

    // trigger the event
    mgos_event_trigger(MGOS_EV_BGPS_POSITION_CHANGED, &s_pos_changed);
  }

  if (s_pos_changed.init) s_pos_changed.init = false;

  (void) arg;
}

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */

bool mgos_bgps_init() {
  // Initialize static s_pos_changed
  memset(&s_pos_changed, 0, sizeof(struct mgos_bgps_position_changed));
  s_pos_changed.init = true;

  if (!mgos_event_register_base(MGOS_BGPS_EVENT_BASE, "bGPS events")) return false;

  // Start the auto-update interval
  if (mgos_sys_config_get_gps_auto_update_enable()) {
    if (mgos_sys_config_get_gps_auto_update_interval() > 0) {
      mgos_set_timer(mgos_sys_config_get_gps_auto_update_interval(),
        MGOS_TIMER_REPEAT, mg_bgps_auto_update_timer_cb, NULL);
    } else {
      LOG(LL_ERROR,("Invalid uato-update timer's interval (%d ms)",
        mgos_sys_config_get_gps_auto_update_interval()));
    }
  }

  return true;
}