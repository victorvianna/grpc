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

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/log/absl_check.h"
#include "absl/log/absl_log.h"
#include "src/core/util/crash.h"
#include "src/proto/grpc/testing/echo.grpc.pb.h"
#include "test/cpp/util/test_config.h"

ABSL_FLAG(std::string, address, "", "Address to connect to");
ABSL_FLAG(std::string, mode, "", "Test mode to use");

using grpc::testing::EchoRequest;
using grpc::testing::EchoResponse;

int main(int argc, char** argv) {
  grpc::testing::InitTest(&argc, &argv, true);
  auto stub = grpc::testing::EchoTestService::NewStub(grpc::CreateChannel(
      absl::GetFlag(FLAGS_address), grpc::InsecureChannelCredentials()));

  EchoRequest request;
  EchoResponse response;
  grpc::ClientContext context;
  context.set_wait_for_ready(true);

  if (absl::GetFlag(FLAGS_mode) == "bidi") {
    auto stream = stub->BidiStream(&context);
    for (int i = 0;; i++) {
      std::ostringstream msg;
      msg << "Hello " << i;
      request.set_message(msg.str());
      ABSL_CHECK(stream->Write(request));
      ABSL_CHECK(stream->Read(&response));
      ABSL_CHECK(response.message() == request.message());
    }
  } else if (absl::GetFlag(FLAGS_mode) == "response") {
    EchoRequest request;
    request.set_message("Hello");
    auto stream = stub->ResponseStream(&context, request);
    for (;;) {
      ABSL_CHECK(stream->Read(&response));
    }
  } else {
    ABSL_LOG(ERROR) << "invalid test mode '" << absl::GetFlag(FLAGS_mode) << "'";
    return 1;
  }
}
