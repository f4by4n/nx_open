// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <gtest/gtest.h>

#include <nx/vms/api/data/user_group_data.h>
#include <nx/vms/common/test_support/test_context.h>
#include <nx/vms/common/user_management/predefined_user_groups.h>
#include <nx/vms/common/user_management/user_management_helpers.h>

using namespace nx::vms::api;

namespace nx::vms::common {
namespace test {

class UserManagementHelpersTest: public ContextBasedTest
{
    virtual void SetUp() override
    {
        ContextBasedTest::SetUp();
        d.administrator = addUser(kAdministratorsGroupId, "administrator");
        d.powerUser = addUser(kPowerUsersGroupId, "power user");
        d.viewer = addUser(kViewersGroupId, "viewer");

        d.specialWorkers = createUserGroup(NoGroup, "Special Workers");
        d.specialPowerUsers = createUserGroup(
            {d.specialWorkers.id, kPowerUsersGroupId}, "Special Power Users");
        d.specialViewers = createUserGroup(
            {d.specialWorkers.id, kViewersGroupId}, "Special Viewers");

        d.specialPowerUser = addUser(d.specialPowerUsers.id, "special power user");
        d.specialViewer = addUser(d.specialViewers.id, "special viewer");

        d.specialAdministrator = addUser({d.specialWorkers.id, kAdministratorsGroupId},
            "special administrator");
    }

    virtual void TearDown() override
    {
        d = {};
        ContextBasedTest::TearDown();
    }

protected:
    struct
    {
        QnUserResourcePtr administrator;
        QnUserResourcePtr powerUser;
        QnUserResourcePtr viewer;
        QnUserResourcePtr specialPowerUser;
        QnUserResourcePtr specialViewer;
        QnUserResourcePtr specialAdministrator;

        UserGroupData specialWorkers;
        UserGroupData specialPowerUsers;
        UserGroupData specialViewers;
    } d;
};

TEST_F(UserManagementHelpersTest, getUsersAndGroups)
{
    const std::vector<QnUuid> requestedIds{{
        QnUuid::createUuid(),
        QnUuid::createUuid(),
        d.powerUser->getId(),
        QnUuid{},
        QnUuid::createUuid(),
        d.specialPowerUsers.id,
        kPowerUsersGroupId,
        QnUuid::createUuid(),
        QnUuid::createUuid(),
        d.specialPowerUser->getId(),
        QnUuid{},
        QnUuid::createUuid()}};

    QnUserResourceList users;
    UserGroupDataList groups;
    getUsersAndGroups(systemContext(), requestedIds, users, groups);
    EXPECT_EQ(users, QnUserResourceList({d.powerUser, d.specialPowerUser}));
    EXPECT_EQ(groups, UserGroupDataList(
        {d.specialPowerUsers, *PredefinedUserGroups::find(kPowerUsersGroupId)}));

    QList<QnUuid> groupIds;
    getUsersAndGroups(systemContext(), requestedIds, users, groupIds);
    EXPECT_EQ(users, QnUserResourceList({d.powerUser, d.specialPowerUser}));
    EXPECT_EQ(groupIds, QList<QnUuid>({d.specialPowerUsers.id, kPowerUsersGroupId}));
}

TEST_F(UserManagementHelpersTest, allGroupsExist)
{
    EXPECT_TRUE(allUserGroupsExist(systemContext(), std::initializer_list<QnUuid>({
        kLiveViewersGroupId, kAdministratorsGroupId, d.specialPowerUsers.id, d.specialViewers.id})));

    EXPECT_FALSE(allUserGroupsExist(systemContext(), std::initializer_list<QnUuid>({
        d.specialPowerUsers.id, /*invalid*/ QnUuid{}})));

    EXPECT_FALSE(allUserGroupsExist(systemContext(), std::initializer_list<QnUuid>({
        d.specialPowerUsers.id, /*unknown*/ QnUuid::createUuid()})));

    EXPECT_FALSE(allUserGroupsExist(systemContext(), std::initializer_list<QnUuid>({
        d.specialPowerUsers.id, /*user*/ d.specialViewer->getId()})));
}

TEST_F(UserManagementHelpersTest, groupNames)
{
    EXPECT_EQ(userGroupNames(systemContext(), std::initializer_list<QnUuid>({
        d.specialWorkers.id, /*invalid*/ QnUuid{}, d.specialPowerUsers.id, d.specialViewers.id})),
        QStringList({d.specialWorkers.name, d.specialPowerUsers.name, d.specialViewers.name}));

    EXPECT_EQ(userGroupNames(d.specialAdministrator), QStringList(
        {d.specialWorkers.name, PredefinedUserGroups::find(kAdministratorsGroupId)->name}));
}

TEST_F(UserManagementHelpersTest, groupsWithParents)
{   /*
          Group 1         *Power Users*          Special Workers          *Viewers*
            |                    |                  |       |                  |
       +----+----+               +---------+--------+       +---------+--------+
       |         |                         |                          |
       v         v                         v                          v
     Group 2    Group 3          Special Power Users          Special Viewers
       |         |  |
       +----+----+  |
            |       |
            v       v
         Group 4  Group 5
    */

    const auto group1 = createUserGroup(NoGroup, "Group 1");
    const auto group2 = createUserGroup(group1.id, "Group 2");
    const auto group3 = createUserGroup(group1.id, "Group 3");
    const auto group4 = createUserGroup({group2.id, group3.id}, "Group 4");
    const auto group5 = createUserGroup(group3.id, "Group 5");

    EXPECT_EQ(userGroupsWithParents(systemContext(),
        std::initializer_list<QnUuid>({group4.id, d.specialPowerUsers.id})),
        QSet<QnUuid>({group4.id, group3.id, group2.id, group1.id,
            d.specialPowerUsers.id, d.specialWorkers.id, kPowerUsersGroupId}));

    EXPECT_EQ(userGroupsWithParents(systemContext(),
        std::initializer_list<QnUuid>({group5.id, d.specialViewers.id})),
        QSet<QnUuid>({group5.id, group3.id, group1.id,
            d.specialViewers.id, d.specialWorkers.id, kViewersGroupId}));
}

} // namespace test
} // namespace nx::vms::common
