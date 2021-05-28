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

void stat(ChunkArrayIter<int64_t>& iter);

std::shared_ptr<arrow::Table> loadCsv(const char* csv_filepath);

int main() {
  const char* csv_filepath = "data/user_info_size.csv";
  LOG() << "csv file: " << csv_filepath;

  auto table = loadCsv(csv_filepath);
  if (table == nullptr) {
    return 1;
  }

  // create iter
  ChunkArrayIter<int64_t> total_it(table->GetColumnByName("total"));
  ChunkArrayIter<int64_t> profile_it(table->GetColumnByName("profile"));
  ChunkArrayIter<int64_t> shown_history_it(table->GetColumnByName("shown_history"));
  ChunkArrayIter<int64_t> real_shown_history_it(table->GetColumnByName("real_shown_history"));
  ChunkArrayIter<int64_t> recent_click_it(table->GetColumnByName("recent_click"));

  // stat
  stat(total_it);
  stat(profile_it);
  stat(shown_history_it);
  stat(real_shown_history_it);
  stat(recent_click_it);

  // total
  // avg 1.4 MB, p50 1.7 MB, p90 2.0 MB, p99 2.3 MB, max 2.7 MB

  return 0;
}

void stat(ChunkArrayIter<int64_t>& it) {
  vector<int64_t> all;
  int64_t sum = 0;
  while (it.not_end()) {
    auto v = it.next();
    sum += v;
    all.push_back(v);
  }
  std::sort(all.begin(), all.end());

  LOG() << "avg=" << sum / (int64_t)all.size() / 1024
        << ", p50=" << all[all.size() * 0.5] / 1024
        << ", p90=" << all[all.size() * 0.9] / 1024
        << ", p99=" << all[all.size() * 0.99] / 1024
        << ", max=" << all[all.size() - 1] / 1024;
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
  convert_opt.column_types["total"] = std::make_shared<arrow::Int64Type>();
  convert_opt.column_types["profile"] = std::make_shared<arrow::Int64Type>();
  convert_opt.column_types["shown_history"] = std::make_shared<arrow::Int64Type>();
  convert_opt.column_types["real_shown_history"] = std::make_shared<arrow::Int64Type>();
  convert_opt.column_types["recent_click"] = std::make_shared<arrow::Int64Type>();
  convert_opt.include_columns = {"total", "profile", "shown_history", "real_shown_history", "recent_click"};
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
  LOG() << "";
  for (auto& fd : table->schema()->fields()) {
    LOG() << "field=" << fd->name() << ", type=" << fd->type()->name();
  }
  LOG() << "";
  return table;
}
