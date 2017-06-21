/*
 * Andy's Workshop Brewery RTD controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * Encapsulate the RTD readings
   */

  struct RtdReadings {
    struct Result {
      uint8_t faultCode;
      double temperature;
      double directAdcReading;
      int32_t directAdcCode;
    } rtd1,rtd2;
  };
}
