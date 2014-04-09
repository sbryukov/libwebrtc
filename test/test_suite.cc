/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "test/test_suite.h"

#include "gflags/gflags.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "test/testsupport/fileutils.h"
#include "test/testsupport/trace_to_stderr.h"

DEFINE_bool(logs, false, "print logs to stderr");

namespace webrtc {
namespace test {

TestSuite::TestSuite(int argc, char** argv) {
  SetExecutablePath(argv[0]);
  testing::InitGoogleMock(&argc, argv);  // Runs InitGoogleTest() internally.
  // AllowCommandLineParsing allows us to ignore flags passed on to us by
  // Chromium build bots without having to explicitly disable them.
  google::AllowCommandLineReparsing();
  google::ParseCommandLineFlags(&argc, &argv, true);
}

TestSuite::~TestSuite() {
}

int TestSuite::Run() {
  Initialize();
  int result = RUN_ALL_TESTS();
  Shutdown();
  return result;
}

void TestSuite::Initialize() {
  if (FLAGS_logs)
    trace_to_stderr_.reset(new TraceToStderr);
}

void TestSuite::Shutdown() {
}

}  // namespace test
}  // namespace webrtc
