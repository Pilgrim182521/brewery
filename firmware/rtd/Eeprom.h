/*
 * Andy's Workshop NXA66 controller ATMega328p firmware
 * Copyright (c) 2017 Andy Brown. http://www.andybrown.me.uk
 * Please see website for licensing terms.
 */

#pragma once


namespace brewery {

  /*
   * General utilities to read/write configuration values to the EEPROM. Functionality
   * is divided between Reader and Writer classes.
   */
   
  struct Eeprom {

    /*
     * EEPROM locations
     */

    enum class Location : uint8_t {
      MAGIC       = 0,      // 2 byte signature
      INTENSITY   = 2,      // 1 byte (8)
      DISPLAYS    = 3,      // 1 byte (RED | BLUE = 0x3)
      REDCAL      = 4,      // 4 bytes (0)
      REDCALDATE  = 8,      // 4 bytes (0)
      BLUECAL     = 12,     // 4 bytes (0)
      BLUECALDATE = 16      // 4 bytes (0)
    };


    /*
     * The magic number that indicates valid EEPROM content
     */

    enum {
      MAGIC_NUMBER = 0x55aa
    };


    static void verifyState();
    static void writeDefaults();

    /*
     * High level read access
     */

    class Reader {
      
      protected:
        static uint16_t validate(uint16_t value);

      public:
        static uint16_t magic();
        static uint16_t constant(Location l);
        static uint8_t readByte(Location l);
        static uint32_t readUint32(Location l);
        static double readDouble(Location l);

        static uint8_t intensity();
        static uint8_t displays();
        static double blueCal();
        static uint32_t blueCalDate();
        static double redCal();
        static uint32_t redCalDate();
    };

    /*
     * High level write access
     */

    struct Writer {
      static void magic();
      static void constant(Location l,uint16_t c);
      static void writeByte(Location l,uint8_t b);
      static void writeUint32(Location l,uint32_t u);
      static void writeDouble(Location l,double d);

      static void intensity(uint8_t i);
      static void displays(uint8_t displays);
      static void blueCal(double d);
      static void blueCalDate(uint32_t u);
      static void redCal(double d);
      static void redCalDate(uint32_t u);
    };
  };


  /*
   * Read a byte
   */

  inline uint8_t Eeprom::Reader::readByte(Location l) {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(l));
  }


  /*
   * Read a uint32_t
   */

  inline uint32_t Eeprom::Reader::readUint32(Location l) {
    return eeprom_read_dword(reinterpret_cast<uint32_t *>(l));
  }


  /*
   * Read a double/float
   */

  inline double Eeprom::Reader::readDouble(Location l) {
    static_assert(sizeof(double)==sizeof(float),"unexpected float/double size mismatch");
    static_assert(sizeof(double)==4,"unexpected double size");

    return eeprom_read_float(reinterpret_cast<float *>(l));
  }


  /*
   * Read a constant value
   */

  inline uint16_t Eeprom::Reader::constant(Location l) {
    return eeprom_read_word(reinterpret_cast<uint16_t *>(l));
  }


  /*
   * Read the magic number
   */

  inline uint16_t Eeprom::Reader::magic() {
    return eeprom_read_word(reinterpret_cast<uint16_t *>(Location::MAGIC));
  }


  /*
   * Read the LCD intensity value (0..15)
   */

  inline uint8_t Eeprom::Reader::intensity() {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(Location::INTENSITY));
  }


  /*
   * Read the blue calibration
   */

  inline double Eeprom::Reader::blueCal() {
    return readDouble(Location::BLUECAL);
  }


  /*
   * Read the blue calibration date
   */

  inline uint32_t Eeprom::Reader::blueCalDate() {
    return readUint32(Location::BLUECALDATE);
  }


  /*
   * Read the red calibration
   */

  inline double Eeprom::Reader::redCal() {
    return readDouble(Location::REDCAL);
  }


  /*
   * Read the red calibration date
   */

  inline uint32_t Eeprom::Reader::redCalDate() {
    return readUint32(Location::REDCALDATE);
  }


  /*
   * Read the displays to use
   */

  inline uint8_t Eeprom::Reader::displays() {
    return eeprom_read_byte(reinterpret_cast<uint8_t *>(Location::DISPLAYS));
  }


  /*
   * Write a byte to the location
   */

  inline void Eeprom::Writer::writeByte(Location l,uint8_t b) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(l),b);
  }


  /*
   * Write a double to the location
   */

  inline void Eeprom::Writer::writeDouble(Location l,double d) {
    eeprom_write_float(reinterpret_cast<float *>(l),d);
  }


  /*
   * Write a uint32_t to the location
   */

  inline void Eeprom::Writer::writeUint32(Location l,uint32_t u) {
    eeprom_write_dword(reinterpret_cast<uint32_t *>(l),u);
  }


  /*
   * Write a constant value
   */

  inline void Eeprom::Writer::constant(Location l,uint16_t c) {
    eeprom_write_word(reinterpret_cast<uint16_t *>(l),c);
  }


  /*
   * Write the magic number
   */

  inline void Eeprom::Writer::magic() {
    eeprom_write_word(reinterpret_cast<uint16_t *>(Location::MAGIC),MAGIC_NUMBER);
  }


  /*
   * Write the intensity
   */

  inline void Eeprom::Writer::intensity(uint8_t i) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(Location::INTENSITY),i);
  }


  /*
   * Write the displays to use
   */

  inline void Eeprom::Writer::displays(uint8_t displays) {
    eeprom_write_byte(reinterpret_cast<uint8_t *>(Location::DISPLAYS),displays);
  }


  /*
   * Write the blue calibration
   */

  inline void Eeprom::Writer::blueCal(double d) {
    writeDouble(Location::BLUECAL,d);
  }


  /*
   * Write the blue calibration date
   */

  inline void Eeprom::Writer::blueCalDate(uint32_t u) {
    writeUint32(Location::BLUECALDATE,u);
  }


  /*
   * Write the red calibration
   */

  inline void Eeprom::Writer::redCal(double d) {
    writeDouble(Location::REDCAL,d);
  }


  /*
   * Write the red calibration date
   */

  inline void Eeprom::Writer::redCalDate(uint32_t u) {
    writeDouble(Location::REDCALDATE,u);
  }


  /*
   * Verify the content and default it if invalid
   */

  inline void Eeprom::verifyState() {
   
    // check for the signature and return if OK

    if(Reader::magic()==MAGIC_NUMBER)
      return;

    writeDefaults();
  }


  /*
   * Write the default values to EEPROM
   */

  inline void Eeprom::writeDefaults() {

    // write out default content

    Writer::intensity(12);
    Writer::displays(0x3);
    Writer::blueCal(0);
    Writer::redCal(0);
    Writer::blueCalDate(0);
    Writer::redCalDate(0);

    Writer::magic();
  }
}

