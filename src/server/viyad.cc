#include <sched.h>
#include <unistd.h>
#include <limits.h>
#include <sstream>
#include <json.hpp>
#include "server/viyad.h"

namespace viya {
namespace server {

namespace db = viya::db;
namespace cluster = viya::cluster;
namespace server = viya::server;

using json = nlohmann::json;

Viyad::Viyad(const util::Config& config):config_(config) {
}

void Viyad::Start() {
#ifdef __linux__
  SetCpuAffinity();
#endif

  db::Database database(config_);
  server::Http http_service(config_, database);
  
  worker_ = std::make_unique<cluster::Worker>(config);

  http_service.Start();
}

#ifdef __linux__
void Viyad::SetCpuAffinity() {
  if (config_.exists("cpu_list")) {
    pid_t pid= getpid();
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    for (auto cpu : config_.numlist("cpu_list")) {
      CPU_SET(cpu, &cpu_set);
    }
    sched_setaffinity(pid, sizeof(cpu_set), &cpu_set);
  }
}
#endif

}}
