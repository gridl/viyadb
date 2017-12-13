/*
 * Copyright (c) 2017 ViyaDB Group
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

#ifndef VIYA_CLUSTER_CONTROLLER_H_
#define VIYA_CLUSTER_CONTROLLER_H_

#include <memory>
#include <map>
#include "util/config.h"
#include "util/schedule.h"
#include "cluster/consul/consul.h"
#include "cluster/plan.h"
#include "cluster/partitioning.h"
#include "cluster/feeder.h"
#include "cluster/batch_info.h"

namespace viya {
namespace cluster {

namespace util = viya::util;

class Controller {
  public:
    Controller(const util::Config& config);
    Controller(const Controller& other) = delete;

    const consul::Consul& consul() const { return consul_; }
    const util::Config& cluster_config() const { return cluster_config_; }
    const std::map<std::string, util::Config>& tables_configs() const { return tables_configs_; }
    const std::map<std::string, util::Config>& workers_configs() const { return workers_configs_; }
    const std::map<std::string, util::Config>& indexers_configs() const { return indexers_configs_; }
    const std::map<std::string, std::unique_ptr<BatchInfo>>& indexers_batches() const { return indexers_batches_; }
    const std::map<std::string, Plan>& tables_plans() const { return tables_plans_; }
    const std::map<std::string, Partitioning>& tables_partitioning() const { return tables_partitioning_; }

  private:
    void ReadClusterConfig();
    bool ReadWorkersConfigs();
    void FetchLatestBatchInfo();
    void Initialize();
    void InitializePartitioning();
    void InitializePlan();
    void AssignPartitionsToWorkers();
    bool ReadPlan();
    bool GeneratePlan();

  private:
    util::Config config_;
    const std::string cluster_id_;
    const consul::Consul consul_;
    util::Config cluster_config_;
    std::unique_ptr<consul::Session> session_;
    std::unique_ptr<consul::LeaderElector> le_;
    std::unique_ptr<util::Later> initializer_;
    std::map<std::string, util::Config> tables_configs_;
    std::map<std::string, util::Config> workers_configs_;
    std::map<std::string, util::Config> indexers_configs_;
    std::map<std::string, std::unique_ptr<BatchInfo>> indexers_batches_;
    std::map<std::string, Plan> tables_plans_;
    std::map<std::string, Partitioning> tables_partitioning_;
    std::unique_ptr<Feeder> feeder_;
};

}}

#endif // VIYA_CLUSTER_CONTROLLER_H_
