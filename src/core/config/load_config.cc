// Copyright 2022 gRPC authors.
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

#include "src/core/config/load_config.h"

#include <grpc/support/port_platform.h>
#include <stdio.h>

#include <optional>

#include "absl/log/absl_check.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "src/core/util/env.h"

namespace grpc_core {

namespace {
std::optional<std::string> LoadEnv(absl::string_view environment_variable) {
  return GetEnv(std::string(environment_variable).c_str());
}
}  // namespace

std::string LoadConfigFromEnv(absl::string_view environment_variable,
                              const char* default_value) {
  ABSL_CHECK(!environment_variable.empty());
  return LoadEnv(environment_variable).value_or(default_value);
}

int32_t LoadConfigFromEnv(absl::string_view environment_variable,
                          int32_t default_value) {
  auto env = LoadEnv(environment_variable);
  if (env.has_value()) {
    int32_t out;
    if (absl::SimpleAtoi(*env, &out)) return out;
    fprintf(stderr, "Error reading int from %s: '%s' is not a number",
            std::string(environment_variable).c_str(), env->c_str());
  }
  return default_value;
}

bool LoadConfigFromEnv(absl::string_view environment_variable,
                       bool default_value) {
  return default_value;
}

std::string LoadConfig(const std::vector<std::string>& from_flag,
                       absl::string_view environment_variable,
                       const std::optional<std::string>& override,
                       const char* default_value) {
  if (override.has_value()) return *override;
  if (!from_flag.empty()) return absl::StrJoin(from_flag, ",");
  return LoadConfigFromEnv(environment_variable, default_value);
}

}  // namespace grpc_core
