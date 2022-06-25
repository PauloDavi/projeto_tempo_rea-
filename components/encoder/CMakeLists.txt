idf_component_register(SRCS "encoder.cpp"
  INCLUDE_DIRS "."
  REQUIRES driver
  PRIV_REQUIRES esp_timer)
