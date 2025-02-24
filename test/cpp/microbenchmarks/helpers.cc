//
//
// Copyright 2017 gRPC authors.
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

#include "test/cpp/microbenchmarks/helpers.h"

#include <string.h>

#include "absl/log/absl_check.h"

static LibraryInitializer* g_libraryInitializer;

LibraryInitializer::LibraryInitializer() {
  ABSL_CHECK_EQ(g_libraryInitializer, nullptr);
  g_libraryInitializer = this;
}

LibraryInitializer::~LibraryInitializer() { g_libraryInitializer = nullptr; }

LibraryInitializer& LibraryInitializer::get() {
  ABSL_CHECK_NE(g_libraryInitializer, nullptr);
  return *g_libraryInitializer;
}
