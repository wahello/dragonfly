// Copyright 2022, DragonflyDB authors.  All rights reserved.
// See LICENSE for licensing terms.
//

#pragma once

#include "server/cluster/cluster_defs.h"
#include "server/conn_context.h"

namespace dfly {

namespace cluster {
class ClusterFamily;
}

class EngineShardSet;
class ServerFamily;

class DebugCmd {
 private:
  struct PopulateOptions {
    uint64_t total_count = 0;
    std::string_view prefix{"key"};
    uint32_t val_size = 16;
    bool populate_random_values = false;
    std::string type{"STRING"};
    uint32_t elements = 1;

    std::optional<cluster::SlotRange> slot_range;
    std::optional<std::pair<uint32_t, uint32_t>> expire_ttl_range;
  };

 public:
  DebugCmd(ServerFamily* owner, cluster::ClusterFamily* cf, ConnectionContext* cntx);

  void Run(CmdArgList args, facade::SinkReplyBuilder* builder);

  static void Shutdown();

 private:
  void Populate(CmdArgList args, facade::SinkReplyBuilder* builder);
  static std::optional<PopulateOptions> ParsePopulateArgs(CmdArgList args,
                                                          facade::SinkReplyBuilder* builder);
  void PopulateRangeFiber(uint64_t from, uint64_t count, const PopulateOptions& opts);

  void Reload(CmdArgList args, facade::SinkReplyBuilder* builder);
  void Replica(CmdArgList args, facade::SinkReplyBuilder* builder);
  void Migration(CmdArgList args, facade::SinkReplyBuilder* builder);

  void Exec(facade::SinkReplyBuilder* builder);
  void Inspect(std::string_view key, CmdArgList args, facade::SinkReplyBuilder* builder);
  void Watched(facade::SinkReplyBuilder* builder);
  void TxAnalysis(facade::SinkReplyBuilder* builder);
  void ObjHist(facade::SinkReplyBuilder* builder);
  void Stacktrace(facade::SinkReplyBuilder* builder);
  void Shards(facade::SinkReplyBuilder* builder);
  void LogTraffic(CmdArgList, facade::SinkReplyBuilder* builder);
  void RecvSize(std::string_view param, facade::SinkReplyBuilder* builder);
  void Topk(CmdArgList args, facade::SinkReplyBuilder* builder);
  void Keys(CmdArgList args, facade::SinkReplyBuilder* builder);
  void Compression(CmdArgList args, facade::SinkReplyBuilder* builder);
  void IOStats(CmdArgList args, facade::SinkReplyBuilder* builder);
  void Segments(CmdArgList args, facade::SinkReplyBuilder* builder);
  struct PopulateBatch {
    DbIndex dbid;
    uint64_t index[32];
    uint64_t sz = 0;

    explicit PopulateBatch(DbIndex id) : dbid(id) {
    }
  };

  void DoPopulateBatch(const PopulateOptions& options, const PopulateBatch& batch);

  ServerFamily& sf_;
  cluster::ClusterFamily& cf_;
  ConnectionContext* cntx_;
};

}  // namespace dfly
