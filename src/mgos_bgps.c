#include "mgos.h"
#include "mgos_bgps.h"

#ifdef MGOS_HAVE_MJS
#include "mjs.h"
#endif /* MGOS_HAVE_MJS */

static bool s_initialized = false;
static struct mgos_bgps_position s_position;

static void mg_bgps_update_timer_cb(void *arg) {
  struct mgos_bgps_position prev_position;
  memcpy(&prev_position, &s_position, sizeof(struct mgos_bgps_position));

  if (!mgos_bgps_get_position(&s_position)) {
    LOG(LL_ERROR, ("Error updating GPS position"));
    return;
  }

  bool is_changed = (s_position.location.latitude != prev_position.location.latitude ||
                     s_position.location.longitude != prev_position.location.longitude);

  if (!s_initialized || is_changed) {
    struct mgos_bgps_position_changed position_changed;
    memcpy(&(position_changed.prev_pos), &prev_position, sizeof(struct mgos_bgps_position));
    memcpy(&(position_changed.cur_pos), &s_position, sizeof(struct mgos_bgps_position));
    mgos_event_trigger(MGOS_EV_BGPS_POSITION_CHANGED, &position_changed);

    LOG(LL_INFO, ("NEW GPS: lat %f, lng %f (accuracy %f)",
      position_changed.cur_pos.location.latitude,
      position_changed.cur_pos.location.longitude,
      position_changed.cur_pos.accuracy));
    LOG(LL_INFO, ("   PREV: lat %f, lng %f (accuracy %f)",
      position_changed.prev_pos.location.latitude,
      position_changed.prev_pos.location.longitude,
      position_changed.prev_pos.accuracy));
  } else {
    LOG(LL_INFO, ("GPS: lat %f, lng %f (accuracy %f)",
    s_position.location.latitude,
    s_position.location.longitude,
    s_position.accuracy));
  }

  if (!s_initialized) s_initialized = true;

  (void) arg;
}

#ifdef MGOS_HAVE_MJS

#endif /* MGOS_HAVE_MJS */

bool mgos_bgps_init() {
  // Initializa position
  s_position.location.latitude = 0.0;
  s_position.location.longitude = 0.0;
  s_position.accuracy = 0.0;

  if (!mgos_event_register_base(MGOS_BGPS_EVENT_BASE, "bGPS events")) return false;

  // Start the polling interval
  if (mgos_sys_config_get_gps_update_interval() > 0) {
    mgos_set_timer(mgos_sys_config_get_gps_update_interval(),
      MGOS_TIMER_REPEAT, mg_bgps_update_timer_cb, NULL);
  } else {
    LOG(LL_ERROR,("Update timer disabled"));
  }

  return true;
}