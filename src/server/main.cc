/*
 * Copyright (c) 2017-present ViyaDB Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "server/supervisor.h"
#include "server/viyad.h"
#include <boost/exception/diagnostic_information.hpp>
#include <glog/logging.h>

namespace server = viya::server;
namespace util = viya::util;

int main(int argc, char *argv[]) {
  try {
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    auto supervisor = std::make_unique<server::Supervisor>(
        std::vector<std::string>(argv, argv + argc));
    supervisor->Start();

  } catch (...) {
    LOG(ERROR) << boost::current_exception_diagnostic_information();
    exit(-1);
  }
}
