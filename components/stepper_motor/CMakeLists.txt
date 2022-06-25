idf_component_register(SRCS "stepper_motor.cpp"
  INCLUDE_DIRS "."
  REQUIRES driver
  PRIV_REQUIRES esp_timer)
