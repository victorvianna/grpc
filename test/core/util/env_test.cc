//
//
// Copyright 2015 gRPC authors.
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
//
//

#include "src/core/util/env.h"

#include "absl/log/absl_log.h"
#include "gtest/gtest.h"
#include "test/core/test_util/test_config.h"

TEST(EnvTest, SetenvGetenv) {
  const char* name = "FOO";
  const char* value = "BAR";

  ABSL_LOG(INFO) << "test_setenv_getenv";

  grpc_core::SetEnv(name, value);
  auto retrieved_value = grpc_core::GetEnv(name);
  ASSERT_EQ(value, retrieved_value);
}

TEST(EnvTest, Unsetenv) {
  const char* name = "FOO";
  const char* value = "BAR";

  ABSL_LOG(INFO) << "test_unsetenv";

  grpc_core::SetEnv(name, value);
  grpc_core::UnsetEnv(name);
  auto retrieved_value = grpc_core::GetEnv(name);
  ASSERT_FALSE(retrieved_value.has_value());
}

int main(int argc, char** argv) {
  grpc::testing::TestEnvironment env(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
