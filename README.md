# bGPS Library
## Overview
This Mongoose OS library is the base for implementing GPS libraries.
## GET STARTED
```c
```
## Configuration
The library adds a `gps` configuration entry which contains configuration settings.
```json
"gps": {
  "auto_update": {      // GPS auto-update config
    "enable": true,     // Enable the auto-update
    "interval": 5000    // Auto-update interval (in milliseconds)
  }
}
```
## C/C++ APIs Reference
### mgos_bthing_event
```c
enum mgos_bthing_event {
  MGOS_EV_BGPS_POSITION_CHANGED
};
```
Events triggered in case the auto-update is enabled and properly [configured](#configuration). Use following functions to subscribe to these events:
* [mgos_event_add_handler()](https://mongoose-os.com/docs/mongoose-os/api/core/mgos_event.h.md#mgos_event_add_handler)
* [mgos_event_add_group_handler(MGOS_EV_BGPS_ANY, ...)](https://mongoose-os.com/docs/mongoose-os/api/core/mgos_event.h.md#mgos_event_add_group_handler)

|Event||
|--|--|
|MGOS_EV_BGPS_POSITION_CHANGED|Triggered when the GPS position is changed. The event-data passed to the handler is a `struct mgos_bgps_position_changed *`.|
### mgos_bgps_position_changed
```c
struct mgos_bgps_position_changed {
  bool init;
  struct mgos_bgps_position prev_pos;
  struct mgos_bgps_position cur_pos;
};
```
Event-data passed to `MGOS_EV_BGPS_POSITION_CHANGED` event's handlers (see [mgos_event_handler_t](https://mongoose-os.com/docs/mongoose-os/api/core/mgos_event.h.md#mgos_event_handler_t)).

|Field||
|--|--|
|init|Initialization flag: 'true' if 'cur_pos' has been initialized.|
|prev_pos|Previous [GPS position](#mgos_bgps_position). The position is set to '0' the 'init' flag is 'true'.|
|cur_pos|Current [GPS position](#mgos_bgps_position).|
### mgos_bgps_location
```c
struct mgos_bgps_location {
  float latitude;
  float longitude;
};
```
A GPS location, in degrees.

|Field||
|--|--|
|latitude|The latitude value, in degrees (e.g.: 37.421875199999995).|
|longitude|The longitude value, in degrees (e.g.: -122.0851173).|
### mgos_bgps_position
```c
struct mgos_bgps_position {
  struct mgos_bgps_location location;
  float accuracy;
};
```
A GPS position.

|Field||
|--|--|
|location|The GPS [location](#mgos_bgps_location)).|
|accuracy|The accuracy of the estimated location, in meters. This represents the radius of a circle around the given location.|
### mgos_bgps_get_position
```c
bool mgos_bgps_get_position(struct mgos_bgps_position *position);
```
Reads the current GPS position. Returns `false` if error, or `true` otherwise.

|Parameter||
|--|--|
|position|The current [GPS position](#mgos_bgps_position).|

**Remarks**

This function is automatically invoked if the auto-update is enabled and properly [configured](#configuration).
## To Do
- Implement javascript APIs for [Mongoose OS MJS](https://github.com/mongoose-os-libs/mjs).