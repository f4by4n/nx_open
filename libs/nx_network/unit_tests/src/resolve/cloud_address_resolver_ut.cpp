// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include <gtest/gtest.h>

#include <nx/network/cloud/cloud_address_resolver.h>
#include <nx/utils/string.h>
#include <nx/utils/uuid.h>

namespace nx::network::test {

class CloudAddressResolver:
    public ::testing::Test
{
protected:
    void assertResolved(const std::string_view& str)
    {
        std::deque<AddressEntry> resolvedEntries;
        ASSERT_EQ(SystemError::noError, m_resolver.resolve(str, AF_INET, &resolvedEntries));
        ASSERT_EQ(1, resolvedEntries.size());
        ASSERT_EQ(AddressType::cloud, resolvedEntries.front().type);
        ASSERT_EQ(str, resolvedEntries.front().host.toString());
    }

    void assertNotResolved(const std::string_view& str)
    {
        std::deque<AddressEntry> resolvedEntries;
        ASSERT_EQ(SystemError::hostNotFound, m_resolver.resolve(str, AF_INET, &resolvedEntries));
        ASSERT_TRUE(resolvedEntries.empty());
    }

private:
    network::CloudAddressResolver m_resolver;
};

TEST_F(CloudAddressResolver, resolves_guid_as_a_cloud_address)
{
    auto guidStr = QnUuid::createUuid().toSimpleStdString();
    assertResolved(guidStr);

    nx::utils::toLower(&guidStr);
    assertResolved(guidStr);

    nx::utils::toUpper(&guidStr);
    assertResolved(guidStr);

    assertNotResolved(nx::utils::buildString(
        "raz-", QnUuid::createUuid().toSimpleStdString(), "-raz"));
    assertNotResolved("nxms.com");
}

} // namespace nx::network::test
