#include "topology.hpp"

#if OPOSSUM_NUMA_SUPPORT
#include <numa.h>
#endif

#include <algorithm>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

namespace opossum {

std::shared_ptr<Topology> Topology::create_fake_numa_topology(uint32_t max_num_workers, uint32_t workers_per_node) {
  auto max_num_threads = std::thread::hardware_concurrency();

  /**
   * Leave one thread free so hopefully the system won't freeze - but if we only have one thread, use that one.
   */
  auto num_workers = std::max<uint32_t>(1, max_num_threads - 1);
  if (max_num_workers != 0) {
    num_workers = std::min(num_workers, max_num_workers);
  }

  auto num_nodes = num_workers / workers_per_node;
  if (num_workers % workers_per_node != 0) num_nodes++;

  std::vector<TopologyNode> nodes;
  nodes.reserve(num_nodes);

  auto cpuID = 0u;

  for (auto n = 0u; n < num_nodes; n++) {
    std::vector<TopologyCpu> cpus;

    for (auto w = 0u; w < workers_per_node && cpuID < num_workers; w++) {
      cpus.emplace_back(TopologyCpu(cpuID));
      cpuID++;
    }

    TopologyNode node(std::move(cpus));

    nodes.emplace_back(std::move(node));
  }

  return std::make_shared<Topology>(std::move(nodes), num_workers);
}

std::shared_ptr<Topology> Topology::create_numa_topology() {
#if !OPOSSUM_NUMA_SUPPORT
  return create_fake_numa_topology();
#else
  if (numa_available() < 0) {
    return create_fake_numa_topology();
  }

  auto max_node = numa_max_node();
  auto num_configured_cpus = numa_num_configured_cpus();
  auto cpu_bitmask = numa_bitmask_alloc(num_configured_cpus);

  std::vector<TopologyNode> nodes;
  nodes.reserve(max_node);

  for (int n = 0; n <= max_node; n++) {
    std::vector<TopologyCpu> cpus;

    numa_node_to_cpus(n, cpu_bitmask);

    for (int c = 0; c < num_configured_cpus; c++) {
      if (numa_bitmask_isbitset(cpu_bitmask, c)) {
        cpus.emplace_back(TopologyCpu(c));
      }
    }

    TopologyNode node(std::move(cpus));
    nodes.emplace_back(std::move(node));
  }

  return std::make_shared<Topology>(std::move(nodes), num_configured_cpus);
#endif
}

}  // namespace opossum
