
#include <QtCore/QCoreApplication>
#include "gamesdatabase.h"
#include "gtest/gtest.h"

int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}


TEST(gamesdatabase_getDataFromId, DataFormCorrect)
{
	double id = 2083;

	TwitchApi::GamesDatabase * db = new TwitchApi::GamesDatabase();
	TwitchApi::dataMap2d testdata = db->getDataFromId(id);//TODO other ids
	EXPECT_EQ(testdata.first.size(), testdata.second.size());
}

TEST(gamesdatabase_nameToIdToNameLoop, LoopClosed)
{
	double id = 2083; //TODO other IDs

	TwitchApi::GamesDatabase * db = new TwitchApi::GamesDatabase();
	TwitchApi::dataMap2d testdata = db->getDataFromId(id);//TODO other ids
	EXPECT_EQ(id , db->nametoId(db->idToName(id)));
}