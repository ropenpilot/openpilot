#include <sys/resource.h>

#include "inc.h"
//#include <boost/interprocess/mapped_region.hpp>
//#include <boost/interprocess/shared_memory_object.hpp>
//using namespace boost::interprocess;
#include "config.h"
#include "listener.h"

#define IOC net::io_context

// unused
////class Shared {
 // string name_;
 // std::size_t size_;
 // shared_memory_object shm_;
 // mapped_region region_;
 // unsigned char * address_;
//p//ublic:
 // ~Shared() { shared_memory_object::remove(name_.c_str()); }
 // Shared(const char *name, std::size_t size)
 //   : name_(name)
 //   , size_(size)
 //   , shm_(create_only, name, read_write) {
 //     shm_.truncate(size);
 //     region_ = mapped_region(shm_, read_write);
 //     address_ = (decltype(address_)) region_.get_address();
 // }
 // void setByte(unsigned char &c) {
 //   if (size_ < 1) { return; }
 //   *address_ = c;
 // }

//}//;

#include "cereal/messaging/messaging.h"
#include "selfdrive/common/swaglog.h"
#include "selfdrive/common/timing.h"
#include "selfdrive/common/util.h"
const char * service = "newService";
#define msgGen(x) MessageBuilder msg; msg.initEvent().initNewService().setSliderone(x);
unsigned long long ftom (float f) { return (unsigned long long)(1.0 / f * 1000.0); }
void sMil(unsigned long long millis) {
  std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}
void sHz(float f) { sMil(ftom(f)); }
void sendMsg (PubMaster &pm, MessageBuilder &msg) {
  pm.send(service, msg);
}

int main(int argc, char *argv[]) {
  const char *host = std::getenv("HOST");
  const char *port = std::getenv("PORT");
  char shmem = 0, shmPrev = 0;
  //setpriority(PRIO_PROCESS, 0, -18);
  IOC ioc;
  ssl::context ctx{ssl::context::tlsv12};
   ///try {
   // catch (std::exception &e) {}

  //SignalHook s(ioc);
  // This holds the self-signed certificate used by the server
  load_server_certificate(ctx);
  const char *h = host ? host : "192.168.43.1";
  unsigned short p = port ? atoi(port) : 443;
  std::printf("[wsServer]\nserving on %s port %d\n", h, p);
  std::make_shared<listener>(
    ioc, ctx, tcp::endpoint{net::ip::make_address(h), p},
    &shmem
  )->run();
  bool thread_go = true;
  std::thread thread = std::thread([&]() {
    PubMaster pm({service});
    //for(;;!do_exit) { try { ShmClient client(name); break; } catch (std::exception &e) { pNoClient(pm); sHz(noShmServerHz); } }
    float update_frequency = 20.0;
    auto update_millis = ftom(update_frequency);
    while (thread_go) {
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

      MessageBuilder msg;
      auto new_service = msg.initEvent().initNewService();

      char byte = shmem;
      if (byte != shmPrev) {
        new_service.setSliderone(byte);
        pm.send(service, msg);
        shmPrev = byte;
      }

      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      std::this_thread::sleep_for(std::chrono::milliseconds(update_millis) - (end - begin));
    }
    MessageBuilder msg;
    auto new_service = msg.initEvent().initNewService();

    shmem = 0; shmPrev = shmem;
    char byte = shmem;
    new_service.setSliderone(byte);
    pm.send(service, msg);
  });
  ioc.run();
  thread_go = false;
  std::printf("clean stop, about to join\n");
  thread.join();
  std::printf("joined\n");
  return 0;
}
