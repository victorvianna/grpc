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

#include "src/core/lib/surface/completion_queue_factory.h"

#include <grpc/grpc.h>
#include <grpc/support/port_platform.h>

#include "absl/log/absl_check.h"
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/surface/completion_queue.h"

//
// == Default completion queue factory implementation ==
//

static grpc_completion_queue* default_create(
    const grpc_completion_queue_factory* /*factory*/,
    const grpc_completion_queue_attributes* attr) {
  return grpc_completion_queue_create_internal(
      attr->cq_completion_type, attr->cq_polling_type, attr->cq_shutdown_cb);
}

static grpc_completion_queue_factory_vtable default_vtable = {default_create};

static const grpc_completion_queue_factory g_default_cq_factory = {
    "Default Factory", nullptr, &default_vtable};

//
// == Completion queue factory APIs
//

const grpc_completion_queue_factory* grpc_completion_queue_factory_lookup(
    const grpc_completion_queue_attributes* attributes) {
  ABSL_CHECK(attributes->version >= 1 &&
        attributes->version <= GRPC_CQ_CURRENT_VERSION);

  // The default factory can handle version 1 of the attributes structure. We
  // may have to change this as more fields are added to the structure
  return &g_default_cq_factory;
}

//
// == Completion queue creation APIs ==
//

grpc_completion_queue* grpc_completion_queue_create_for_next(void* reserved) {
  grpc_core::ExecCtx exec_ctx;
  ABSL_CHECK(!reserved);
  grpc_completion_queue_attributes attr = {1, GRPC_CQ_NEXT,
                                           GRPC_CQ_DEFAULT_POLLING, nullptr};
  return g_default_cq_factory.vtable->create(&g_default_cq_factory, &attr);
}

grpc_completion_queue* grpc_completion_queue_create_for_pluck(void* reserved) {
  grpc_core::ExecCtx exec_ctx;
  ABSL_CHECK(!reserved);
  grpc_completion_queue_attributes attr = {1, GRPC_CQ_PLUCK,
                                           GRPC_CQ_DEFAULT_POLLING, nullptr};
  return g_default_cq_factory.vtable->create(&g_default_cq_factory, &attr);
}

grpc_completion_queue* grpc_completion_queue_create_for_callback(
    grpc_completion_queue_functor* shutdown_callback, void* reserved) {
  grpc_core::ExecCtx exec_ctx;
  ABSL_CHECK(!reserved);
  grpc_completion_queue_attributes attr = {
      2, GRPC_CQ_CALLBACK, GRPC_CQ_DEFAULT_POLLING, shutdown_callback};
  return g_default_cq_factory.vtable->create(&g_default_cq_factory, &attr);
}

grpc_completion_queue* grpc_completion_queue_create(
    const grpc_completion_queue_factory* factory,
    const grpc_completion_queue_attributes* attr, void* reserved) {
  grpc_core::ExecCtx exec_ctx;
  ABSL_CHECK(!reserved);
  return factory->vtable->create(factory, attr);
}
