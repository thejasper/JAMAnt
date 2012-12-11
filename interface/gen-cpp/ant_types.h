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
  _AntSettings__isset() : baudrate(true) {}
  bool baudrate;
} _AntSettings__isset;

class AntSettings {
 public:

  static const char* ascii_fingerprint; // = "18B162B1D15D8D46509D3911A9F1C2AA";
  static const uint8_t binary_fingerprint[16]; // = {0x18,0xB1,0x62,0xB1,0xD1,0x5D,0x8D,0x46,0x50,0x9D,0x39,0x11,0xA9,0xF1,0xC2,0xAA};

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
    __isset.baudrate = true;
  }

  bool operator == (const AntSettings & rhs) const
  {
    if (!(port == rhs.port))
      return false;
    if (__isset.baudrate != rhs.__isset.baudrate)
      return false;
    else if (__isset.baudrate && !(baudrate == rhs.baudrate))
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