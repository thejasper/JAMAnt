/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Ant_H
#define Ant_H

#include <thrift/TDispatchProcessor.h>
#include "ant_types.h"

namespace robotics {

class AntIf {
 public:
  virtual ~AntIf() {}
  virtual bool init(const AntSettings& settings) = 0;
  virtual bool stop() = 0;
  virtual bool walk(const int32_t speed) = 0;
  virtual bool turn(const int32_t angle) = 0;
  virtual bool draw(const std::vector<std::vector<int32_t> > & points, const int32_t width, const int32_t height) = 0;
  virtual void getComPorts(std::vector<std::string> & _return, const std::string& wildcard) = 0;
  virtual int32_t ping() = 0;
};

class AntIfFactory {
 public:
  typedef AntIf Handler;

  virtual ~AntIfFactory() {}

  virtual AntIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(AntIf* /* handler */) = 0;
};

class AntIfSingletonFactory : virtual public AntIfFactory {
 public:
  AntIfSingletonFactory(const boost::shared_ptr<AntIf>& iface) : iface_(iface) {}
  virtual ~AntIfSingletonFactory() {}

  virtual AntIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(AntIf* /* handler */) {}

 protected:
  boost::shared_ptr<AntIf> iface_;
};

class AntNull : virtual public AntIf {
 public:
  virtual ~AntNull() {}
  bool init(const AntSettings& /* settings */) {
    bool _return = false;
    return _return;
  }
  bool stop() {
    bool _return = false;
    return _return;
  }
  bool walk(const int32_t /* speed */) {
    bool _return = false;
    return _return;
  }
  bool turn(const int32_t /* angle */) {
    bool _return = false;
    return _return;
  }
  bool draw(const std::vector<std::vector<int32_t> > & /* points */, const int32_t /* width */, const int32_t /* height */) {
    bool _return = false;
    return _return;
  }
  void getComPorts(std::vector<std::string> & /* _return */, const std::string& /* wildcard */) {
    return;
  }
  int32_t ping() {
    int32_t _return = 0;
    return _return;
  }
};

typedef struct _Ant_init_args__isset {
  _Ant_init_args__isset() : settings(false) {}
  bool settings;
} _Ant_init_args__isset;

class Ant_init_args {
 public:

  Ant_init_args() {
  }

  virtual ~Ant_init_args() throw() {}

  AntSettings settings;

  _Ant_init_args__isset __isset;

  void __set_settings(const AntSettings& val) {
    settings = val;
  }

  bool operator == (const Ant_init_args & rhs) const
  {
    if (!(settings == rhs.settings))
      return false;
    return true;
  }
  bool operator != (const Ant_init_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_init_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_init_pargs {
 public:


  virtual ~Ant_init_pargs() throw() {}

  const AntSettings* settings;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_init_result__isset {
  _Ant_init_result__isset() : success(false) {}
  bool success;
} _Ant_init_result__isset;

class Ant_init_result {
 public:

  Ant_init_result() : success(0) {
  }

  virtual ~Ant_init_result() throw() {}

  bool success;

  _Ant_init_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  bool operator == (const Ant_init_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_init_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_init_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_init_presult__isset {
  _Ant_init_presult__isset() : success(false) {}
  bool success;
} _Ant_init_presult__isset;

class Ant_init_presult {
 public:


  virtual ~Ant_init_presult() throw() {}

  bool* success;

  _Ant_init_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Ant_stop_args {
 public:

  Ant_stop_args() {
  }

  virtual ~Ant_stop_args() throw() {}


  bool operator == (const Ant_stop_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Ant_stop_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_stop_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_stop_pargs {
 public:


  virtual ~Ant_stop_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_stop_result__isset {
  _Ant_stop_result__isset() : success(false) {}
  bool success;
} _Ant_stop_result__isset;

class Ant_stop_result {
 public:

  Ant_stop_result() : success(0) {
  }

  virtual ~Ant_stop_result() throw() {}

  bool success;

  _Ant_stop_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  bool operator == (const Ant_stop_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_stop_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_stop_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_stop_presult__isset {
  _Ant_stop_presult__isset() : success(false) {}
  bool success;
} _Ant_stop_presult__isset;

class Ant_stop_presult {
 public:


  virtual ~Ant_stop_presult() throw() {}

  bool* success;

  _Ant_stop_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Ant_walk_args__isset {
  _Ant_walk_args__isset() : speed(false) {}
  bool speed;
} _Ant_walk_args__isset;

class Ant_walk_args {
 public:

  Ant_walk_args() : speed(0) {
  }

  virtual ~Ant_walk_args() throw() {}

  int32_t speed;

  _Ant_walk_args__isset __isset;

  void __set_speed(const int32_t val) {
    speed = val;
  }

  bool operator == (const Ant_walk_args & rhs) const
  {
    if (!(speed == rhs.speed))
      return false;
    return true;
  }
  bool operator != (const Ant_walk_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_walk_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_walk_pargs {
 public:


  virtual ~Ant_walk_pargs() throw() {}

  const int32_t* speed;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_walk_result__isset {
  _Ant_walk_result__isset() : success(false) {}
  bool success;
} _Ant_walk_result__isset;

class Ant_walk_result {
 public:

  Ant_walk_result() : success(0) {
  }

  virtual ~Ant_walk_result() throw() {}

  bool success;

  _Ant_walk_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  bool operator == (const Ant_walk_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_walk_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_walk_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_walk_presult__isset {
  _Ant_walk_presult__isset() : success(false) {}
  bool success;
} _Ant_walk_presult__isset;

class Ant_walk_presult {
 public:


  virtual ~Ant_walk_presult() throw() {}

  bool* success;

  _Ant_walk_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Ant_turn_args__isset {
  _Ant_turn_args__isset() : angle(false) {}
  bool angle;
} _Ant_turn_args__isset;

class Ant_turn_args {
 public:

  Ant_turn_args() : angle(0) {
  }

  virtual ~Ant_turn_args() throw() {}

  int32_t angle;

  _Ant_turn_args__isset __isset;

  void __set_angle(const int32_t val) {
    angle = val;
  }

  bool operator == (const Ant_turn_args & rhs) const
  {
    if (!(angle == rhs.angle))
      return false;
    return true;
  }
  bool operator != (const Ant_turn_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_turn_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_turn_pargs {
 public:


  virtual ~Ant_turn_pargs() throw() {}

  const int32_t* angle;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_turn_result__isset {
  _Ant_turn_result__isset() : success(false) {}
  bool success;
} _Ant_turn_result__isset;

class Ant_turn_result {
 public:

  Ant_turn_result() : success(0) {
  }

  virtual ~Ant_turn_result() throw() {}

  bool success;

  _Ant_turn_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  bool operator == (const Ant_turn_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_turn_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_turn_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_turn_presult__isset {
  _Ant_turn_presult__isset() : success(false) {}
  bool success;
} _Ant_turn_presult__isset;

class Ant_turn_presult {
 public:


  virtual ~Ant_turn_presult() throw() {}

  bool* success;

  _Ant_turn_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Ant_draw_args__isset {
  _Ant_draw_args__isset() : points(false), width(false), height(false) {}
  bool points;
  bool width;
  bool height;
} _Ant_draw_args__isset;

class Ant_draw_args {
 public:

  Ant_draw_args() : width(0), height(0) {
  }

  virtual ~Ant_draw_args() throw() {}

  std::vector<std::vector<int32_t> >  points;
  int32_t width;
  int32_t height;

  _Ant_draw_args__isset __isset;

  void __set_points(const std::vector<std::vector<int32_t> > & val) {
    points = val;
  }

  void __set_width(const int32_t val) {
    width = val;
  }

  void __set_height(const int32_t val) {
    height = val;
  }

  bool operator == (const Ant_draw_args & rhs) const
  {
    if (!(points == rhs.points))
      return false;
    if (!(width == rhs.width))
      return false;
    if (!(height == rhs.height))
      return false;
    return true;
  }
  bool operator != (const Ant_draw_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_draw_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_draw_pargs {
 public:


  virtual ~Ant_draw_pargs() throw() {}

  const std::vector<std::vector<int32_t> > * points;
  const int32_t* width;
  const int32_t* height;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_draw_result__isset {
  _Ant_draw_result__isset() : success(false) {}
  bool success;
} _Ant_draw_result__isset;

class Ant_draw_result {
 public:

  Ant_draw_result() : success(0) {
  }

  virtual ~Ant_draw_result() throw() {}

  bool success;

  _Ant_draw_result__isset __isset;

  void __set_success(const bool val) {
    success = val;
  }

  bool operator == (const Ant_draw_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_draw_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_draw_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_draw_presult__isset {
  _Ant_draw_presult__isset() : success(false) {}
  bool success;
} _Ant_draw_presult__isset;

class Ant_draw_presult {
 public:


  virtual ~Ant_draw_presult() throw() {}

  bool* success;

  _Ant_draw_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Ant_getComPorts_args__isset {
  _Ant_getComPorts_args__isset() : wildcard(true) {}
  bool wildcard;
} _Ant_getComPorts_args__isset;

class Ant_getComPorts_args {
 public:

  Ant_getComPorts_args() : wildcard("/dev/ttyACM*") {
  }

  virtual ~Ant_getComPorts_args() throw() {}

  std::string wildcard;

  _Ant_getComPorts_args__isset __isset;

  void __set_wildcard(const std::string& val) {
    wildcard = val;
  }

  bool operator == (const Ant_getComPorts_args & rhs) const
  {
    if (!(wildcard == rhs.wildcard))
      return false;
    return true;
  }
  bool operator != (const Ant_getComPorts_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_getComPorts_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_getComPorts_pargs {
 public:


  virtual ~Ant_getComPorts_pargs() throw() {}

  const std::string* wildcard;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_getComPorts_result__isset {
  _Ant_getComPorts_result__isset() : success(false) {}
  bool success;
} _Ant_getComPorts_result__isset;

class Ant_getComPorts_result {
 public:

  Ant_getComPorts_result() {
  }

  virtual ~Ant_getComPorts_result() throw() {}

  std::vector<std::string>  success;

  _Ant_getComPorts_result__isset __isset;

  void __set_success(const std::vector<std::string> & val) {
    success = val;
  }

  bool operator == (const Ant_getComPorts_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_getComPorts_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_getComPorts_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_getComPorts_presult__isset {
  _Ant_getComPorts_presult__isset() : success(false) {}
  bool success;
} _Ant_getComPorts_presult__isset;

class Ant_getComPorts_presult {
 public:


  virtual ~Ant_getComPorts_presult() throw() {}

  std::vector<std::string> * success;

  _Ant_getComPorts_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class Ant_ping_args {
 public:

  Ant_ping_args() {
  }

  virtual ~Ant_ping_args() throw() {}


  bool operator == (const Ant_ping_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Ant_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Ant_ping_pargs {
 public:


  virtual ~Ant_ping_pargs() throw() {}


  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_ping_result__isset {
  _Ant_ping_result__isset() : success(false) {}
  bool success;
} _Ant_ping_result__isset;

class Ant_ping_result {
 public:

  Ant_ping_result() : success(0) {
  }

  virtual ~Ant_ping_result() throw() {}

  int32_t success;

  _Ant_ping_result__isset __isset;

  void __set_success(const int32_t val) {
    success = val;
  }

  bool operator == (const Ant_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Ant_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Ant_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Ant_ping_presult__isset {
  _Ant_ping_presult__isset() : success(false) {}
  bool success;
} _Ant_ping_presult__isset;

class Ant_ping_presult {
 public:


  virtual ~Ant_ping_presult() throw() {}

  int32_t* success;

  _Ant_ping_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class AntClient : virtual public AntIf {
 public:
  AntClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  AntClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool init(const AntSettings& settings);
  void send_init(const AntSettings& settings);
  bool recv_init();
  bool stop();
  void send_stop();
  bool recv_stop();
  bool walk(const int32_t speed);
  void send_walk(const int32_t speed);
  bool recv_walk();
  bool turn(const int32_t angle);
  void send_turn(const int32_t angle);
  bool recv_turn();
  bool draw(const std::vector<std::vector<int32_t> > & points, const int32_t width, const int32_t height);
  void send_draw(const std::vector<std::vector<int32_t> > & points, const int32_t width, const int32_t height);
  bool recv_draw();
  void getComPorts(std::vector<std::string> & _return, const std::string& wildcard);
  void send_getComPorts(const std::string& wildcard);
  void recv_getComPorts(std::vector<std::string> & _return);
  int32_t ping();
  void send_ping();
  int32_t recv_ping();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class AntProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<AntIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (AntProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_init(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_stop(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_walk(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_turn(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_draw(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_getComPorts(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  AntProcessor(boost::shared_ptr<AntIf> iface) :
    iface_(iface) {
    processMap_["init"] = &AntProcessor::process_init;
    processMap_["stop"] = &AntProcessor::process_stop;
    processMap_["walk"] = &AntProcessor::process_walk;
    processMap_["turn"] = &AntProcessor::process_turn;
    processMap_["draw"] = &AntProcessor::process_draw;
    processMap_["getComPorts"] = &AntProcessor::process_getComPorts;
    processMap_["ping"] = &AntProcessor::process_ping;
  }

  virtual ~AntProcessor() {}
};

class AntProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  AntProcessorFactory(const ::boost::shared_ptr< AntIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< AntIfFactory > handlerFactory_;
};

class AntMultiface : virtual public AntIf {
 public:
  AntMultiface(std::vector<boost::shared_ptr<AntIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~AntMultiface() {}
 protected:
  std::vector<boost::shared_ptr<AntIf> > ifaces_;
  AntMultiface() {}
  void add(boost::shared_ptr<AntIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool init(const AntSettings& settings) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->init(settings);
    }
    return ifaces_[i]->init(settings);
  }

  bool stop() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->stop();
    }
    return ifaces_[i]->stop();
  }

  bool walk(const int32_t speed) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->walk(speed);
    }
    return ifaces_[i]->walk(speed);
  }

  bool turn(const int32_t angle) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->turn(angle);
    }
    return ifaces_[i]->turn(angle);
  }

  bool draw(const std::vector<std::vector<int32_t> > & points, const int32_t width, const int32_t height) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->draw(points, width, height);
    }
    return ifaces_[i]->draw(points, width, height);
  }

  void getComPorts(std::vector<std::string> & _return, const std::string& wildcard) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getComPorts(_return, wildcard);
    }
    ifaces_[i]->getComPorts(_return, wildcard);
    return;
  }

  int32_t ping() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping();
    }
    return ifaces_[i]->ping();
  }

};

} // namespace

#endif
