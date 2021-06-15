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
  std::unordered_map<BinaryData /* gh */, std::unordered_map<uint64_t /* reco_id */, RecoStatItem>> reco_stat;
  while (gh_it.not_end()) {
    auto gh = gh_it.next();
    auto recoid = recoid_it.next();
    auto& reco_st = reco_stat[gh][recoid];
    reco_st.recall_num += recall_num_it.next();
    reco_st.out_num += out_num_it.next();
  }

  std::map<std::string /* gh */, std::vector<double>> recall_filter_ratio_stat;
  for (auto& gh_2_recos : reco_stat) {
    std::string gh_name(gh_2_recos.first.data, gh_2_recos.first.len);
    auto& ratios = recall_filter_ratio_stat[gh_name];
    ratios.push_back(0.);
    double ratio_sum = 0.;
    for (auto& reco_st : gh_2_recos.second) {
      if (reco_st.second.out_num <= 1) continue;
      double ratio = (double)reco_st.second.recall_num / (double)reco_st.second.out_num;
      ratios.push_back(ratio);
      ratio_sum += ratio;
    }
    std::sort(ratios.begin(), ratios.end());
    ratios[0] = ratio_sum / (double)(ratios.size() - 1);
  }
  for (auto& e : recall_filter_ratio_stat) {
    auto& ratios = e.second;
    int num = (int)ratios.size();
    LOG() << e.first
          << ", avg=" << setiosflags(ios::fixed) << setprecision(2) << ratios[0]
          << ",  p50=" << setiosflags(ios::fixed) << setprecision(2) << ratios[(num >> 1)]
          << ",  p90=" << setiosflags(ios::fixed) << setprecision(2) << ratios[int(num * 0.9)]
          << ",  max=" << setiosflags(ios::fixed) << setprecision(2) << ratios[(num - 1)];
  }

  std::map<std::string /* gh */, GhStatItem> gh_stat;
  for (auto& gh_2_recos : reco_stat) {
    std::string gh_name(gh_2_recos.first.data, gh_2_recos.first.len);
    auto& gh_st = gh_stat[gh_name];
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
    LOG() << gh_st.first
          << ", recall_num=[" << gh_st.second.recall_num_avg << ", " << gh_st.second.recall_num_max << "]"
          << ", out_num=[" << gh_st.second.out_num_avg << ", " << gh_st.second.out_num_max << "]"
          << ", reco_num=" << gh_st.second.reco_num;
  }

  /*
  ChannelFeed,    avg=2.73,   p50=2.16,   p90=4.17,   max=25.02
  CommunityFeed,  avg=3.93,   p50=3.21,   p90=5.19,   max=5.22
  FullScreenFeed, avg=15.41,  p50=15.08,  p90=18.67,  max=41.32
  Immersive,      avg=13.28,  p50=13.05,  p90=16.29,  max=23.05
  Radio,          avg=1.44,   p50=1.01,   p90=2.16,   max=7.42
  VideoFeed,      avg=4.75,   p50=5.11,   p90=5.39,   max=8.24
  VideoOffline,   avg=7.57,   p50=7.05,   p90=9.73,   max=57.37
  ZLeaf,          avg=5.12,   p50=5.05,   p90=6.90,   max=13.01

  ChannelFeed,    recall_num=[3790,   9489],   out_num=[1537,  2880],  reco_num=277
  CommunityFeed,  recall_num=[6909,   6914],   out_num=[1830,  2166],  reco_num=42
  FullScreenFeed, recall_num=[30238, 82537],   out_num=[2042,  4110],  reco_num=1281
  Immersive,      recall_num=[30740, 75614],   out_num=[2301,  3615],  reco_num=334
  Radio,          recall_num=[3631,  31775],   out_num=[2057,  5954],  reco_num=168
  VideoFeed,      recall_num=[11923, 33344],   out_num=[2531,  7407],  reco_num=563
  VideoOffline,   recall_num=[29990, 56547],   out_num=[4251,  9001],  reco_num=2689
  ZLeaf,          recall_num=[18320, 51148],   out_num=[3676, 10048],  reco_num=2689
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
