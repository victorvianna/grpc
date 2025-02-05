// Copyright 2023 gRPC authors.
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

// Auto generated by tools/codegen/core/gen_experiments.py
//
// This file contains the autogenerated parts of the experiments API.
//
// It generates two symbols for each experiment.
//
// For the experiment named new_car_project, it generates:
//
// - a function IsNewCarProjectEnabled() that returns true if the experiment
//   should be enabled at runtime.
//
// - a macro GRPC_EXPERIMENT_IS_INCLUDED_NEW_CAR_PROJECT that is defined if the
//   experiment *could* be enabled at runtime.
//
// The function is used to determine whether to run the experiment or
// non-experiment code path.
//
// If the experiment brings significant bloat, the macro can be used to avoid
// including the experiment code path in the binary for binaries that are size
// sensitive.
//
// By default that includes our iOS and Android builds.
//
// Finally, a small array is included that contains the metadata for each
// experiment.
//
// A macro, GRPC_EXPERIMENTS_ARE_FINAL, controls whether we fix experiment
// configuration at build time (if it's defined) or allow it to be tuned at
// runtime (if it's disabled).
//
// If you are using the Bazel build system, that macro can be configured with
// --define=grpc_experiments_are_final=true

#ifndef GRPC_SRC_CORE_LIB_EXPERIMENTS_EXPERIMENTS_H
#define GRPC_SRC_CORE_LIB_EXPERIMENTS_EXPERIMENTS_H

#include <grpc/support/port_platform.h>

#include "src/core/lib/experiments/config.h"

namespace grpc_core {

#ifdef GRPC_EXPERIMENTS_ARE_FINAL

#if defined(GRPC_CFSTREAM)
#define GRPC_EXPERIMENT_IS_INCLUDED_BACKOFF_CAP_INITIAL_AT_MAX
inline bool IsBackoffCapInitialAtMaxEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_CALL_TRACER_IN_TRANSPORT
inline bool IsCallTracerInTransportEnabled() { return true; }
inline bool IsCallv3ClientAuthFilterEnabled() { return false; }
inline bool IsDisableBufferHintOnHighMemoryPressureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_APPLICATION_CALLBACKS
inline bool IsEventEngineApplicationCallbacksEnabled() { return true; }
inline bool IsEventEngineClientEnabled() { return false; }
inline bool IsEventEngineDnsEnabled() { return false; }
inline bool IsEventEngineDnsNonClientChannelEnabled() { return false; }
inline bool IsEventEngineListenerEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CALLBACK_CQ
inline bool IsEventEngineCallbackCqEnabled() { return true; }
inline bool IsFreeLargeAllocatorEnabled() { return false; }
inline bool IsKeepAlivePingTimerBatchEnabled() { return false; }
inline bool IsLocalConnectorSecureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MAX_PINGS_WO_DATA_THROTTLE
inline bool IsMaxPingsWoDataThrottleEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MONITORING_EXPERIMENT
inline bool IsMonitoringExperimentEnabled() { return true; }
inline bool IsMultipingEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_PICK_FIRST_NEW
inline bool IsPickFirstNewEnabled() { return true; }
inline bool IsPosixEeSkipGrpcInitEnabled() { return false; }
inline bool IsPrioritizeFinishedRequestsEnabled() { return false; }
inline bool IsPromiseBasedHttp2ClientTransportEnabled() { return false; }
inline bool IsPromiseBasedHttp2ServerTransportEnabled() { return false; }
inline bool IsPromiseBasedInprocTransportEnabled() { return false; }
inline bool IsRetryInCallv3Enabled() { return false; }
inline bool IsRqFastRejectEnabled() { return false; }
inline bool IsScheduleCancellationOverWriteEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_SERVER_LISTENER
inline bool IsServerListenerEnabled() { return true; }
inline bool IsTcpFrameSizeTuningEnabled() { return false; }
inline bool IsTcpRcvLowatEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_TRACE_RECORD_CALLOPS
inline bool IsTraceRecordCallopsEnabled() { return true; }
inline bool IsUnconstrainedMaxQuotaBufferSizeEnabled() { return false; }

#elif defined(GPR_WINDOWS)
#define GRPC_EXPERIMENT_IS_INCLUDED_BACKOFF_CAP_INITIAL_AT_MAX
inline bool IsBackoffCapInitialAtMaxEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_CALL_TRACER_IN_TRANSPORT
inline bool IsCallTracerInTransportEnabled() { return true; }
inline bool IsCallv3ClientAuthFilterEnabled() { return false; }
inline bool IsDisableBufferHintOnHighMemoryPressureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_APPLICATION_CALLBACKS
inline bool IsEventEngineApplicationCallbacksEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CLIENT
inline bool IsEventEngineClientEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_DNS
inline bool IsEventEngineDnsEnabled() { return true; }
inline bool IsEventEngineDnsNonClientChannelEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_LISTENER
inline bool IsEventEngineListenerEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CALLBACK_CQ
inline bool IsEventEngineCallbackCqEnabled() { return true; }
inline bool IsFreeLargeAllocatorEnabled() { return false; }
inline bool IsKeepAlivePingTimerBatchEnabled() { return false; }
inline bool IsLocalConnectorSecureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MAX_PINGS_WO_DATA_THROTTLE
inline bool IsMaxPingsWoDataThrottleEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MONITORING_EXPERIMENT
inline bool IsMonitoringExperimentEnabled() { return true; }
inline bool IsMultipingEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_PICK_FIRST_NEW
inline bool IsPickFirstNewEnabled() { return true; }
inline bool IsPosixEeSkipGrpcInitEnabled() { return false; }
inline bool IsPrioritizeFinishedRequestsEnabled() { return false; }
inline bool IsPromiseBasedHttp2ClientTransportEnabled() { return false; }
inline bool IsPromiseBasedHttp2ServerTransportEnabled() { return false; }
inline bool IsPromiseBasedInprocTransportEnabled() { return false; }
inline bool IsRetryInCallv3Enabled() { return false; }
inline bool IsRqFastRejectEnabled() { return false; }
inline bool IsScheduleCancellationOverWriteEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_SERVER_LISTENER
inline bool IsServerListenerEnabled() { return true; }
inline bool IsTcpFrameSizeTuningEnabled() { return false; }
inline bool IsTcpRcvLowatEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_TRACE_RECORD_CALLOPS
inline bool IsTraceRecordCallopsEnabled() { return true; }
inline bool IsUnconstrainedMaxQuotaBufferSizeEnabled() { return false; }

#else
#define GRPC_EXPERIMENT_IS_INCLUDED_BACKOFF_CAP_INITIAL_AT_MAX
inline bool IsBackoffCapInitialAtMaxEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_CALL_TRACER_IN_TRANSPORT
inline bool IsCallTracerInTransportEnabled() { return true; }
inline bool IsCallv3ClientAuthFilterEnabled() { return false; }
inline bool IsDisableBufferHintOnHighMemoryPressureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_APPLICATION_CALLBACKS
inline bool IsEventEngineApplicationCallbacksEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CLIENT
inline bool IsEventEngineClientEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_DNS
inline bool IsEventEngineDnsEnabled() { return true; }
inline bool IsEventEngineDnsNonClientChannelEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_LISTENER
inline bool IsEventEngineListenerEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CALLBACK_CQ
inline bool IsEventEngineCallbackCqEnabled() { return true; }
inline bool IsFreeLargeAllocatorEnabled() { return false; }
inline bool IsKeepAlivePingTimerBatchEnabled() { return false; }
inline bool IsLocalConnectorSecureEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MAX_PINGS_WO_DATA_THROTTLE
inline bool IsMaxPingsWoDataThrottleEnabled() { return true; }
#define GRPC_EXPERIMENT_IS_INCLUDED_MONITORING_EXPERIMENT
inline bool IsMonitoringExperimentEnabled() { return true; }
inline bool IsMultipingEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_PICK_FIRST_NEW
inline bool IsPickFirstNewEnabled() { return true; }
inline bool IsPosixEeSkipGrpcInitEnabled() { return false; }
inline bool IsPrioritizeFinishedRequestsEnabled() { return false; }
inline bool IsPromiseBasedHttp2ClientTransportEnabled() { return false; }
inline bool IsPromiseBasedHttp2ServerTransportEnabled() { return false; }
inline bool IsPromiseBasedInprocTransportEnabled() { return false; }
inline bool IsRetryInCallv3Enabled() { return false; }
inline bool IsRqFastRejectEnabled() { return false; }
inline bool IsScheduleCancellationOverWriteEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_SERVER_LISTENER
inline bool IsServerListenerEnabled() { return true; }
inline bool IsTcpFrameSizeTuningEnabled() { return false; }
inline bool IsTcpRcvLowatEnabled() { return false; }
#define GRPC_EXPERIMENT_IS_INCLUDED_TRACE_RECORD_CALLOPS
inline bool IsTraceRecordCallopsEnabled() { return true; }
inline bool IsUnconstrainedMaxQuotaBufferSizeEnabled() { return false; }
#endif

#else
enum ExperimentIds {
  kExperimentIdBackoffCapInitialAtMax,
  kExperimentIdCallTracerInTransport,
  kExperimentIdCallv3ClientAuthFilter,
  kExperimentIdDisableBufferHintOnHighMemoryPressure,
  kExperimentIdEventEngineApplicationCallbacks,
  kExperimentIdEventEngineClient,
  kExperimentIdEventEngineDns,
  kExperimentIdEventEngineDnsNonClientChannel,
  kExperimentIdEventEngineListener,
  kExperimentIdEventEngineCallbackCq,
  kExperimentIdFreeLargeAllocator,
  kExperimentIdKeepAlivePingTimerBatch,
  kExperimentIdLocalConnectorSecure,
  kExperimentIdMaxPingsWoDataThrottle,
  kExperimentIdMonitoringExperiment,
  kExperimentIdMultiping,
  kExperimentIdPickFirstNew,
  kExperimentIdPosixEeSkipGrpcInit,
  kExperimentIdPrioritizeFinishedRequests,
  kExperimentIdPromiseBasedHttp2ClientTransport,
  kExperimentIdPromiseBasedHttp2ServerTransport,
  kExperimentIdPromiseBasedInprocTransport,
  kExperimentIdRetryInCallv3,
  kExperimentIdRqFastReject,
  kExperimentIdScheduleCancellationOverWrite,
  kExperimentIdServerListener,
  kExperimentIdTcpFrameSizeTuning,
  kExperimentIdTcpRcvLowat,
  kExperimentIdTraceRecordCallops,
  kExperimentIdUnconstrainedMaxQuotaBufferSize,
  kNumExperiments
};
#define GRPC_EXPERIMENT_IS_INCLUDED_BACKOFF_CAP_INITIAL_AT_MAX
inline bool IsBackoffCapInitialAtMaxEnabled() {
  return IsExperimentEnabled<kExperimentIdBackoffCapInitialAtMax>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_CALL_TRACER_IN_TRANSPORT
inline bool IsCallTracerInTransportEnabled() {
  return IsExperimentEnabled<kExperimentIdCallTracerInTransport>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_CALLV3_CLIENT_AUTH_FILTER
inline bool IsCallv3ClientAuthFilterEnabled() {
  return IsExperimentEnabled<kExperimentIdCallv3ClientAuthFilter>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_DISABLE_BUFFER_HINT_ON_HIGH_MEMORY_PRESSURE
inline bool IsDisableBufferHintOnHighMemoryPressureEnabled() {
  return IsExperimentEnabled<
      kExperimentIdDisableBufferHintOnHighMemoryPressure>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_APPLICATION_CALLBACKS
inline bool IsEventEngineApplicationCallbacksEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineApplicationCallbacks>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CLIENT
inline bool IsEventEngineClientEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineClient>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_DNS
inline bool IsEventEngineDnsEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineDns>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_DNS_NON_CLIENT_CHANNEL
inline bool IsEventEngineDnsNonClientChannelEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineDnsNonClientChannel>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_LISTENER
inline bool IsEventEngineListenerEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineListener>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_EVENT_ENGINE_CALLBACK_CQ
inline bool IsEventEngineCallbackCqEnabled() {
  return IsExperimentEnabled<kExperimentIdEventEngineCallbackCq>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_FREE_LARGE_ALLOCATOR
inline bool IsFreeLargeAllocatorEnabled() {
  return IsExperimentEnabled<kExperimentIdFreeLargeAllocator>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_KEEP_ALIVE_PING_TIMER_BATCH
inline bool IsKeepAlivePingTimerBatchEnabled() {
  return IsExperimentEnabled<kExperimentIdKeepAlivePingTimerBatch>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_LOCAL_CONNECTOR_SECURE
inline bool IsLocalConnectorSecureEnabled() {
  return IsExperimentEnabled<kExperimentIdLocalConnectorSecure>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_MAX_PINGS_WO_DATA_THROTTLE
inline bool IsMaxPingsWoDataThrottleEnabled() {
  return IsExperimentEnabled<kExperimentIdMaxPingsWoDataThrottle>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_MONITORING_EXPERIMENT
inline bool IsMonitoringExperimentEnabled() {
  return IsExperimentEnabled<kExperimentIdMonitoringExperiment>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_MULTIPING
inline bool IsMultipingEnabled() {
  return IsExperimentEnabled<kExperimentIdMultiping>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_PICK_FIRST_NEW
inline bool IsPickFirstNewEnabled() {
  return IsExperimentEnabled<kExperimentIdPickFirstNew>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_POSIX_EE_SKIP_GRPC_INIT
inline bool IsPosixEeSkipGrpcInitEnabled() {
  return IsExperimentEnabled<kExperimentIdPosixEeSkipGrpcInit>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_PRIORITIZE_FINISHED_REQUESTS
inline bool IsPrioritizeFinishedRequestsEnabled() {
  return IsExperimentEnabled<kExperimentIdPrioritizeFinishedRequests>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_PROMISE_BASED_HTTP2_CLIENT_TRANSPORT
inline bool IsPromiseBasedHttp2ClientTransportEnabled() {
  return IsExperimentEnabled<kExperimentIdPromiseBasedHttp2ClientTransport>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_PROMISE_BASED_HTTP2_SERVER_TRANSPORT
inline bool IsPromiseBasedHttp2ServerTransportEnabled() {
  return IsExperimentEnabled<kExperimentIdPromiseBasedHttp2ServerTransport>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_PROMISE_BASED_INPROC_TRANSPORT
inline bool IsPromiseBasedInprocTransportEnabled() {
  return IsExperimentEnabled<kExperimentIdPromiseBasedInprocTransport>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_RETRY_IN_CALLV3
inline bool IsRetryInCallv3Enabled() {
  return IsExperimentEnabled<kExperimentIdRetryInCallv3>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_RQ_FAST_REJECT
inline bool IsRqFastRejectEnabled() {
  return IsExperimentEnabled<kExperimentIdRqFastReject>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_SCHEDULE_CANCELLATION_OVER_WRITE
inline bool IsScheduleCancellationOverWriteEnabled() {
  return IsExperimentEnabled<kExperimentIdScheduleCancellationOverWrite>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_SERVER_LISTENER
inline bool IsServerListenerEnabled() {
  return IsExperimentEnabled<kExperimentIdServerListener>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_TCP_FRAME_SIZE_TUNING
inline bool IsTcpFrameSizeTuningEnabled() {
  return IsExperimentEnabled<kExperimentIdTcpFrameSizeTuning>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_TCP_RCV_LOWAT
inline bool IsTcpRcvLowatEnabled() {
  return IsExperimentEnabled<kExperimentIdTcpRcvLowat>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_TRACE_RECORD_CALLOPS
inline bool IsTraceRecordCallopsEnabled() {
  return IsExperimentEnabled<kExperimentIdTraceRecordCallops>();
}
#define GRPC_EXPERIMENT_IS_INCLUDED_UNCONSTRAINED_MAX_QUOTA_BUFFER_SIZE
inline bool IsUnconstrainedMaxQuotaBufferSizeEnabled() {
  return IsExperimentEnabled<kExperimentIdUnconstrainedMaxQuotaBufferSize>();
}

extern const ExperimentMetadata g_experiment_metadata[kNumExperiments];

#endif
}  // namespace grpc_core

#endif  // GRPC_SRC_CORE_LIB_EXPERIMENTS_EXPERIMENTS_H
