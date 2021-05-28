#include "util/base.h"

#include <arrow/api.h>
#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/pretty_print.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>

#include "arrow_util/chunk_array_iter.h"

std::shared_ptr<arrow::Table> loadCsv(const char* csv_filepath);

struct RecoStatItem {
  uint64_t recall_num = 0u;
  uint64_t out_num = 0u;
};

struct GhStatItem {
  uint64_t recall_num_avg = 0u;
  uint64_t recall_num_max = 0u;
  uint64_t out_num_avg = 0u;
  uint64_t out_num_max = 0u;
  uint64_t reco_num = 0u;
};

int main() {
  const char* csv_filepath = "data/recall_filter.csv";
  LOG() << "csv file: " << csv_filepath;

  auto table = loadCsv(csv_filepath);
  if (table == nullptr) {
    return 1;
  }

  // create iter
  BinaryChunkArrayIter gh_it(table->GetColumnByName("gh"));
  ChunkArrayIter<uint64_t> recoid_it(table->GetColumnByName("recoid"));
  ChunkArrayIter<uint64_t> recall_num_it(table->GetColumnByName("recall_num"));
  ChunkArrayIter<uint64_t> out_num_it(table->GetColumnByName("out_num"));

  // stat
  std::unordered_map<BinaryData, std::unordered_map<uint64_t, RecoStatItem>> reco_stat;
  while (gh_it.not_end()) {
    auto gh = gh_it.next();
    auto recoid = recoid_it.next();
    auto& reco_st = reco_stat[gh][recoid];
    reco_st.recall_num += recall_num_it.next();
    reco_st.out_num += out_num_it.next();
  }
  std::unordered_map<BinaryData, GhStatItem> gh_stat;
  for (auto& gh_2_recos : reco_stat) {
    auto& gh_st = gh_stat[gh_2_recos.first];
    for (auto& reco_st : gh_2_recos.second) {
      gh_st.recall_num_avg += reco_st.second.recall_num;
      gh_st.recall_num_max = std::max(gh_st.recall_num_max, reco_st.second.recall_num);
      gh_st.out_num_avg += reco_st.second.out_num;
      gh_st.out_num_max = std::max(gh_st.out_num_max, reco_st.second.out_num);
      gh_st.reco_num++;
    }
  }
  for (auto& gh_st : gh_stat) {
    gh_st.second.recall_num_avg /= gh_st.second.reco_num;
    gh_st.second.out_num_avg /= gh_st.second.reco_num;
  }
  for (auto& gh_st : gh_stat) {
    LOG() << std::string(gh_st.first.data, gh_st.first.len)
          << ", recall_num=[" << gh_st.second.recall_num_avg << ", " << gh_st.second.recall_num_max << "]"
          << ", out_num=[" << gh_st.second.out_num_avg << ", " << gh_st.second.out_num_max << "]"
          << ", reco_num=" << gh_st.second.reco_num;
  }
  /*
  ChannelFeed, recall_num=[5400, 12987], out_num=[1345, 3198], reco_num=259
  FullScreenFeed, recall_num=[31748, 88339], out_num=[1808, 3262], reco_num=1141
  ZLeaf, recall_num=[28578, 41771], out_num=[3742, 4854], reco_num=1787
  Radio, recall_num=[13819, 49118], out_num=[2993, 8841], reco_num=76
  CommunityFeed, recall_num=[4329, 4332], out_num=[1845, 1858], reco_num=21
  VideoOffline, recall_num=[28549, 60074], out_num=[3289, 6141], reco_num=3197
  Immersive, recall_num=[27784, 51619], out_num=[2052, 3344], reco_num=190
  VideoFeed, recall_num=[14283, 30281], out_num=[3077, 6807], reco_num=105
  */

  return 0;
}

std::shared_ptr<arrow::Table> loadCsv(const char* csv_filepath) {
  Timer timer(TimeUnit::MILLISECOND);
  auto&& open_res = arrow::io::ReadableFile::Open(csv_filepath);
  if (!open_res.ok()) {
    LOG() << "open csv fail: " << open_res.status();
    return nullptr;
  }
  LOG() << "open cost: " << timer.interval();
  auto input = std::move(open_res).ValueUnsafe();
  timer.flush();
  arrow::csv::ConvertOptions convert_opt;
  convert_opt.column_types["gh"] = std::make_shared<arrow::BinaryType>();
  convert_opt.column_types["recoid"] = std::make_shared<arrow::UInt64Type>();
  convert_opt.column_types["uid"] = std::make_shared<arrow::UInt64Type>();
  convert_opt.column_types["recall_num"] = std::make_shared<arrow::UInt64Type>();
  convert_opt.column_types["out_num"] = std::make_shared<arrow::UInt64Type>();
  convert_opt.include_columns = {"gh", "recoid", "uid", "recall_num", "out_num"};
  auto read_maker_res = arrow::csv::TableReader::Make(
      arrow::io::default_io_context(),
      input,
      arrow::csv::ReadOptions::Defaults(),
      arrow::csv::ParseOptions::Defaults(),
      convert_opt);
  if (!read_maker_res.ok()) {
    LOG() << "make reader fail:" << read_maker_res.status();
    return nullptr;
  }
  auto read_res = read_maker_res.ValueUnsafe()->Read();
  if (!read_res.ok()) {
    LOG() << "read fail: " << read_res.status();
    return nullptr;
  }
  auto table = read_res.ValueUnsafe();
  LOG() << "read cost: " << timer.interval();

  // schema
  for (auto& fd : table->schema()->fields()) {
    LOG() << "field=" << fd->name() << ", type=" << fd->type()->name();
  }
  return table;
}
