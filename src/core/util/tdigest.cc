// Copyright 2024 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "src/core/util/tdigest.h"

#include "absl/log/absl_check.h"
#include "absl/log/absl_log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"

namespace grpc_core {

namespace {

constexpr double kNan = std::numeric_limits<double>::quiet_NaN();
constexpr double kMaxCompression = 1e6;
constexpr double kPi = 3.14159265358979323846;

// Returns the minimum of compression and kMaxCompression.
double BoundedCompression(double compression) {
  static_assert(8 * kMaxCompression < std::numeric_limits<int64_t>::max(),
                "kMaxCompression must be smaller than max_int64/8.");
  return std::min(kMaxCompression, compression);
}

// Returns the maximum centroids that can be generated by the merging t-digest.
size_t MaxCentroids(double compression) {
  compression = BoundedCompression(compression);
  return 2 * static_cast<size_t>(std::ceil(compression));
}

double LinearInterpolate(double val1, double val2, double weight1,
                         double weight2) {
  ABSL_DCHECK_GE(weight1, 0);
  ABSL_DCHECK_GE(weight2, 0);
  ABSL_DCHECK_GT(weight1 + weight2, 0);
  return (val1 * weight1 + val2 * weight2) / (weight1 + weight2);
}

}  // namespace

TDigest::TDigest(double compression) { Reset(compression); }

void TDigest::Reset(double compression) {
  compression_ = BoundedCompression(compression);
  // Set the default batch_size to 4 times the number of centroids.
  batch_size_ = static_cast<int64_t>(4 * MaxCentroids(compression_));
  ABSL_DCHECK(compression_ == 0.0 || batch_size_ > 0);
  centroids_.reserve(MaxCentroids(compression_) + batch_size_);
  centroids_.clear();
  merged_ = 0;
  unmerged_ = 0;
  min_ = std::numeric_limits<double>::max();
  max_ = std::numeric_limits<double>::lowest();
  sum_ = 0;
  count_ = 0;
}

void TDigest::Add(double val, int64_t count) {
  if (count == 0) {
    return;
  }
  // Single sample is considered discrete.
  UpdateStats(/*min=*/val, /*max=*/val, /*sum=*/val * count, count);
  AddUnmergedCentroid(CentroidPod(val, count));
}

void TDigest::AddUnmergedCentroid(const CentroidPod& centroid) {
  ABSL_DCHECK_LT(unmerged_, batch_size_);

  centroids_.push_back(centroid);
  ++unmerged_;
  if (unmerged_ == batch_size_) {
    DoMerge();
  }
}

void TDigest::Merge(const TDigest& that) {
  if (compression_ == 0.0) {
    Reset(that.Compression());
  }

  UpdateStats(that.Min(), that.Max(), that.Sum(), that.Count());

  for (const auto& centroid : that.centroids_) {
    AddUnmergedCentroid(centroid);
  }
}

double TDigest::QuantileToCentroid(double quantile) const {
  return compression_ * (std::asin(2 * quantile - 1) + kPi / 2) / kPi;
}

double TDigest::CentroidToQuantile(double centroid) const {
  centroid = std::min(centroid, compression_);
  return (sin(centroid * kPi / compression_ - kPi / 2) + 1) / 2;
}

// TODO(ysseung): Maybe try bi-directional merge to lower 1p error rate. Error
// rates are consistently higher at 1p. This is likely due to greedy merge from
// left. But we care 99p more and this may be just fine.
void TDigest::DoMerge() {
  if (unmerged_ == 0) {
    return;
  }

  // We first sort the centroids, and assume the first centroid is merged,
  // and the rest are unmerged.
  ABSL_DCHECK(!centroids_.empty());
  std::sort(centroids_.begin(), centroids_.end());
  unmerged_ += merged_ - 1;
  merged_ = 1;

  const int64_t total_count = count_;

  double q0 = 0;
  // This is actually S * q_{limit} in the paper, not exactly q_limit.
  // But, keeping the scaled value results in eliminating the division in the
  // hotpath. Also, it is closer to the reference implementation.
  double q_limit = total_count * CentroidToQuantile(q0 + 1);

  // When non-discrete, the sum value may change due to floating point errors
  // every time centroids are merged. We must correct this each time by keeping
  // it as much in sync with current centroids as possible to keep this error
  // bounded.
  sum_ = 0;

  auto last_merged = centroids_.begin();
  auto first_unmerged = last_merged + 1;
  int64_t merged_count = last_merged->count;
  for (; unmerged_ > 0; --unmerged_, ++first_unmerged) {
    // Simply merge, if the last merged centroid has enough room for the last
    // unmerged element.
    if (first_unmerged->count + merged_count <= q_limit) {
      // Note that here we use the Welford's method, and
      // count must be updated before mean.
      last_merged->count += first_unmerged->count;
      last_merged->mean +=
          ((first_unmerged->mean - last_merged->mean) * first_unmerged->count) /
          last_merged->count;
      merged_count += first_unmerged->count;
      continue;
    }

    // Now we need to move onto the next centroid to merge the first unmerged.
    q0 = QuantileToCentroid(static_cast<double>(merged_count) / total_count);
    q_limit = total_count * CentroidToQuantile(q0 + 1);
    merged_count += first_unmerged->count;
    sum_ += last_merged->mean * last_merged->count;
    ++merged_;
    ++last_merged;
    *last_merged = *first_unmerged;
  }
  sum_ += last_merged->mean * last_merged->count;

  unmerged_ = 0;
  centroids_.resize(merged_);
  if (!centroids_.empty()) {
    min_ = std::min(min_, centroids_.front().mean);
    max_ = std::max(max_, centroids_.back().mean);
  }
  ABSL_DCHECK_LE(centroids_.size(), MaxCentroids(compression_));
}

// We use linear interpolation between mid points of centroids when calculating
// Cdf() and Percentile(). All unmerged centoirds are merged first so that they
// are strongly ordered, then we use linear interpolation with points:
//
//   (percentile, value) = (0, min), (count[0] / 2, mean[0]), ..
//                         ((count[i-1]+count[i])/2, mean[i]), ..
//                         (count[last], max)
//
// the CDF from centroids with interpolation points marked with '*':
//
//    count
//       |
//  +c[2]|                  --------*
// (=tot)|                  |       |
//       |                  *       |
//  +c[1]|          --------|       |
//       |          |               |
//       |          *               |
//  +c[0]|     -----|               |
//       |     |                    |
//       |     *                    |
//       |     |                    |
//     0 *----------------------------- value
//      min  m[0] m[1]     m[2]    max
//
double TDigest::Cdf(double val) {
  DoMerge();

  if (merged_ == 0) {
    return kNan;
  }

  if (val < min_) {
    return 0;
  }

  // We diverge from the spec here. If value == max == min, we return 1.
  if (val >= max_) {
    return 1;
  }
  ABSL_DCHECK_NE(min_, max_);

  if (merged_ == 1) {
    return (val - min_) / (min_ - max_);
  }

  if (val < centroids_[0].mean) {
    return LinearInterpolate(
        0.0, static_cast<double>(centroids_[0].count) / count_ / 2.0,
        centroids_[0].mean - val, val - min_);
  }

  if (val >= centroids_.back().mean) {
    return LinearInterpolate(
        1.0 - static_cast<double>(centroids_.back().count) / count_ / 2.0, 1,
        max_ - val, val - centroids_.back().mean);
  }

  double accum_count = centroids_[0].count / 2.0;
  for (size_t i = 0; i < centroids_.size(); ++i) {
    if (centroids_[i].mean == val) {
      double prev_accum_count = accum_count;
      // We may have centroids of the same mean. We need to sum their counts
      // and then interpolate.
      for (; centroids_[i + 1].mean == val; ++i) {
        accum_count += centroids_[i].count + centroids_[i + 1].count;
      }
      return (prev_accum_count + accum_count) / 2.0 / count_;
    }
    if (centroids_[i].mean <= val && val < centroids_[i + 1].mean) {
      auto mean1 = centroids_[i].mean;
      auto mean2 = centroids_[i + 1].mean;
      double mean_ratio;
      // guard against double madness.
      if (mean2 <= mean1) {
        mean_ratio = 1;
      } else {
        mean_ratio = (val - mean1) / (mean2 - mean1);
      }
      double delta_count =
          (centroids_[i].count + centroids_[i + 1].count) / 2.0;
      return (accum_count + delta_count * mean_ratio) / count_;
    }
    accum_count += (centroids_[i].count + centroids_[i + 1].count) / 2.0;
  }

  ABSL_LOG(DFATAL) << "Cannot measure CDF for: " << val;
  return kNan;
}

double TDigest::Quantile(double quantile) {
  ABSL_DCHECK_LE(quantile, 1);
  ABSL_DCHECK_GE(quantile, 0);

  DoMerge();

  if (merged_ == 0) {
    return kNan;
  }
  if (merged_ == 1) {
    return centroids_[0].mean;
  }

  const double quantile_count = quantile * count_;
  double prev_count = 0;
  double prev_val = min_;
  double this_count = centroids_[0].count / 2.0;
  double this_val = centroids_[0].mean;

  for (size_t i = 0; i < centroids_.size(); ++i) {
    if (quantile_count < this_count) {
      break;
    }

    prev_count = this_count;
    prev_val = this_val;

    if (i == centroids_.size() - 1) {
      // Interpolate between max and the last centroid.
      this_count = count_;
      this_val = max_;
    } else {
      this_count += (centroids_[i].count + centroids_[i + 1].count) / 2.0;
      this_val = centroids_[i + 1].mean;
    }
  }

  return LinearInterpolate(prev_val, this_val, this_count - quantile_count,
                           quantile_count - prev_count);
}

std::string TDigest::ToString() {
  std::string str = absl::StrCat(compression_);
  if (count_ <= 1) {
    if (count_ == 0) {
      // Note the string representation serializes min/max = 0 when empty.
      return absl::StrAppendFormat(&str, "/0/0/0/0");
    }
    return absl::StrAppendFormat(&str, "/%0.17g", centroids_.front().mean);
  }

  DoMerge();

  absl::StrAppendFormat(&str, "/%0.17g/%0.17g/%0.17g/%d", min_, max_, sum_,
                        count_);

  for (auto& centroid : centroids_) {
    absl::StrAppendFormat(&str, "/%0.17g:%d", centroid.mean, centroid.count);
  }
  return str;
}

absl::Status TDigest::FromString(absl::string_view string) {
  // Accept an empty string as 'not set'.
  // Although ToString() never produces an empty string, an empty string is
  // still expected when a t-Digest is missing.
  if (string.empty()) {
    Reset(0);
    return absl::OkStatus();
  }

  const std::vector<absl::string_view> tokens = absl::StrSplit(string, '/');
  auto iter = tokens.begin();

  // First token (compression and discrete).
  if (iter == tokens.end() || iter->empty()) {
    return absl::InvalidArgumentError("No compression/discrete.");
  }

  double double_val;
  if (!absl::SimpleAtod(iter->substr(0, iter->length()), &double_val) ||
      double_val < 0) {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid double_val/discrete: ", *iter));
  }

  Reset(double_val);

  if (++iter == tokens.end()) {
    return absl::InvalidArgumentError("Unexpected end of string.");
  }

  // Single-valued t-Digest.
  if ((iter + 1) == tokens.end()) {
    if (!absl::SimpleAtod(*iter, &double_val)) {
      return absl::InvalidArgumentError(
          absl::StrCat("Invalid single-value: ", *iter));
    }
    Add(double_val, 1);
    return absl::OkStatus();
  }

  // Parse min/max/sum/count.
  double min = 0.0, max = 0.0, sum = 0.0;
  int64_t count = 0;
  if (iter == tokens.end() || !absl::SimpleAtod(*iter, &min) ||
      ++iter == tokens.end() || !absl::SimpleAtod(*iter, &max) ||
      ++iter == tokens.end() || !absl::SimpleAtod(*iter, &sum) ||
      ++iter == tokens.end() || !absl::SimpleAtoi(*iter, &count)) {
    return absl::InvalidArgumentError("Invalid min, max, sum, or count.");
  }

  // Empty. Note the string representation serializes min/max = 0 when empty.
  if (++iter == tokens.end()) {
    if (min != 0 || max != 0 || count != 0 || sum != 0) {
      return absl::InvalidArgumentError(
          "Empty t-Digest with non-zero min, max, sum, or count.");
    }
    return absl::OkStatus();
  }

  // Parse centroids.
  int64_t int_val = 0;

  for (; iter != tokens.end(); ++iter) {
    const auto pos = iter->find_first_of(':');
    if (pos == absl::string_view::npos ||
        !absl::SimpleAtod(iter->substr(0, pos), &double_val) ||
        !absl::SimpleAtoi(iter->substr(pos + 1), &int_val)) {
      return absl::InvalidArgumentError(
          absl::StrCat("Invalid centroid: ", *iter));
    }
    Add(double_val, int_val);
  }

  DoMerge();
  min_ = min;
  max_ = max;

  if (centroids_.empty()) {
    return absl::OkStatus();
  }

  // Validate min/max/sum/count.
  ABSL_DCHECK_LT(std::abs(sum - sum_), 1e-10) << "Invalid sum value.";

  if (count != count_) {
    return absl::InvalidArgumentError("Invalid count value.");
  }

  return absl::OkStatus();
}

size_t TDigest::MemUsageBytes() const {
  return sizeof(TDigest) + (sizeof(CentroidPod) * centroids_.capacity());
}

}  // namespace grpc_core
