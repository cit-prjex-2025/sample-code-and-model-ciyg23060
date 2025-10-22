#include "app.h"
#include "util.h"

const int bumper_sensor = EV3_PORT_1;
const int linemon_sensor = EV3_PORT_3;
const int left_motor = EV3_PORT_A;
const int right_motor = EV3_PORT_C;

const int carrier_sensor = EV3_PORT_2;

int carrier_cargo_is_loaded(void) {
  return ev3_touch_sensor_is_pressed(carrier_sensor);
}

const int walldetector_sensor = EV3_PORT_4;
#define WD_DISTANCE 10
int wd_distance = WD_DISTANCE;

int wall_detector_is_detected(void) {
  return ev3_ultrasonic_sensor_get_distance(walldetector_sensor)
    < wd_distance;
}

int bumper_is_pushed(void) {
  return ev3_touch_sensor_is_pressed(bumper_sensor);
}

#define LM_THRESHOLD 20
int lm_threshold = LM_THRESHOLD;

int linemon_is_online(void) {
  return ev3_color_sensor_get_reflect(linemon_sensor) < lm_threshold;
}

#define DR_POWER 20
int dr_power = DR_POWER;

void driver_turn_left(void) {
  ev3_motor_set_power(EV3_PORT_A, 0);
  ev3_motor_set_power(EV3_PORT_C, dr_power);
}

void driver_turn_right(void) {
  ev3_motor_set_power(EV3_PORT_A, dr_power);
  ev3_motor_set_power(EV3_PORT_C, 0);
}

void driver_stop(void) {
  ev3_motor_stop(left_motor, false);
  ev3_motor_stop(right_motor, false);
}

void tracer_run(void) {
  if( linemon_is_online() ) {
    driver_turn_left();
  } else {
    driver_turn_right();
  }
}

void tracer_stop(void) {
  driver_stop();
}


void main_task(intptr_t unused) {
  static int is_initialized = false;
  if(! is_initialized ) {
    is_initialized = true;
    init_f("sample04");
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    ev3_sensor_config(walldetector_sensor, ULTRASONIC_SENSOR);
    ev3_sensor_config(linemon_sensor, COLOR_SENSOR);
    ev3_sensor_config(bumper_sensor, TOUCH_SENSOR);
    ev3_sensor_config(carrier_sensor, TOUCH_SENSOR);
  }

  porter_transport();
  ext_tsk();
}
