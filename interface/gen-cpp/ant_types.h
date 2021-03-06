/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ant_TYPES_H
#define ant_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace robotics {

typedef struct _AntSettings__isset {
  _AntSettings__isset() : port(false), baudrate(true) {}
  bool port;
  bool baudrate;
} _AntSettings__isset;

class AntSettings {
 public:

  static const char* ascii_fingerprint; // = "EEBC915CE44901401D881E6091423036";
  static const uint8_t binary_fingerprint[16]; // = {0xEE,0xBC,0x91,0x5C,0xE4,0x49,0x01,0x40,0x1D,0x88,0x1E,0x60,0x91,0x42,0x30,0x36};

  AntSettings() : port(), baudrate(115200) {
  }

  virtual ~AntSettings() throw() {}

  std::string port;
  int32_t baudrate;

  _AntSettings__isset __isset;

  void __set_port(const std::string& val) {
    port = val;
  }

  void __set_baudrate(const int32_t val) {
    baudrate = val;
  }

  bool operator == (const AntSettings & rhs) const
  {
    if (!(port == rhs.port))
      return false;
    if (!(baudrate == rhs.baudrate))
      return false;
    return true;
  }
  bool operator != (const AntSettings &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const AntSettings & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(AntSettings &a, AntSettings &b);

} // namespace

#endif
