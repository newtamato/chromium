// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/storage_partition_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

class StoragePartitionConfigTest : public testing::Test {
};

// Test that the Less comparison function is implemented properly to uniquely
// identify storage partitions used as keys in a std::map.
TEST_F(StoragePartitionConfigTest, OperatorLess) {
  StoragePartitionImpl::StoragePartitionConfig c1("", "", false);
  StoragePartitionImpl::StoragePartitionConfig c2("", "", false);
  StoragePartitionImpl::StoragePartitionConfig c3("", "", true);
  StoragePartitionImpl::StoragePartitionConfig c4("a", "", true);
  StoragePartitionImpl::StoragePartitionConfig c5("b", "", true);
  StoragePartitionImpl::StoragePartitionConfig c6("", "abc", false);
  StoragePartitionImpl::StoragePartitionConfig c7("", "abc", true);
  StoragePartitionImpl::StoragePartitionConfig c8("a", "abc", false);
  StoragePartitionImpl::StoragePartitionConfig c9("a", "abc", true);

  StoragePartitionImpl::StoragePartitionConfigLess less;

  // Let's ensure basic comparison works.
  EXPECT_TRUE(less(c1, c3));
  EXPECT_TRUE(less(c1, c4));
  EXPECT_TRUE(less(c3, c4));
  EXPECT_TRUE(less(c4, c5));
  EXPECT_TRUE(less(c4, c8));
  EXPECT_TRUE(less(c6, c4));
  EXPECT_TRUE(less(c6, c7));
  EXPECT_TRUE(less(c8, c9));

  // Now, ensure antisymmetry for each pair we've tested.
  EXPECT_FALSE(less(c3, c1));
  EXPECT_FALSE(less(c4, c1));
  EXPECT_FALSE(less(c4, c3));
  EXPECT_FALSE(less(c5, c4));
  EXPECT_FALSE(less(c8, c4));
  EXPECT_FALSE(less(c4, c6));
  EXPECT_FALSE(less(c7, c6));
  EXPECT_FALSE(less(c9, c8));

  // Check for irreflexivity.
  EXPECT_FALSE(less(c1, c1));

  // Check for transitivity.
  EXPECT_TRUE(less(c1, c4));

  // Let's enforce that two identical elements obey strict weak ordering.
  EXPECT_TRUE(!less(c1, c2) && !less(c2, c1));
}

}  // namespace content
