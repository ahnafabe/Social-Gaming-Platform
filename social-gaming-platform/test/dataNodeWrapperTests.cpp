#include <gtest/gtest.h>

#include "data/data.h"

class ConfigurationTest : public ::testing::Test {
protected:
    Configuration config;

    void SetUp() override {
        DataNode configNode;
        configNode.setMapValue("name", create_string_node("Rock, Paper, Scissors"));
        configNode.setMapValue("players", create_range_node(std::make_pair(0, 2)));
        configNode.setMapValue("audience", create_bool_node(true));

        config = Configuration(configNode);
    }
};

TEST_F(ConfigurationTest, GetName) {
    std::string_view name = config.getName();
    EXPECT_EQ(name, "Rock, Paper, Scissors");
}

TEST_F(ConfigurationTest, GetPlayerRange) {
    Range players = config.getPlayerRange();
    EXPECT_EQ(players.first, 0);
    EXPECT_EQ(players.second, 2);
}

TEST_F(ConfigurationTest, IsAudienceEnabled) {
    bool audienceEnabled = config.isAudienceEnabled();
    EXPECT_TRUE(audienceEnabled);
}

class GlobalConstantsTest : public ::testing::Test{
protected:
    GameStateObject constants;

    void SetUp() override{
        DataNode weapon1Node;
        weapon1Node.setMapValue("name", create_string_node("Rock"));
        weapon1Node.setMapValue("beats", create_string_node("Scissors"));
        DataNode weapon2Node;
        weapon2Node.setMapValue("name", create_string_node("Paper"));
        weapon2Node.setMapValue("beats", create_string_node("Rock"));
        DataNode weapon3Node;
        weapon3Node.setMapValue("name", create_string_node("Scissors"));
        weapon3Node.setMapValue("beats", create_string_node("Paper"));
        DataNode weaponVectorNode;
        weaponVectorNode.addVectorValue(weapon1Node);
        weaponVectorNode.addVectorValue(weapon2Node);
        weaponVectorNode.addVectorValue(weapon3Node);
        DataNode constantsNode;
        constantsNode.setMapValue("weapons", weaponVectorNode);

        constants = GameStateObject(constantsNode);
    }
};

TEST_F(GlobalConstantsTest, GetConstant) {
    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(0).getMapValue("name").getString(), "Rock");
    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(0).getMapValue("beats").getString(), "Scissors");

    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(1).getMapValue("name").getString(), "Paper");
    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(1).getMapValue("beats").getString(), "Rock");

    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(2).getMapValue("name").getString(), "Scissors");
    EXPECT_EQ(constants.getObjectByName("weapons").getVectorValue(2).getMapValue("beats").getString(), "Paper");
}

class GlobalVariablesTest : public ::testing::Test{
protected:
    GameStateObject variables;

    void SetUp() override{
        DataNode winnersNode;
        winnersNode.addVectorValue(create_string_node("Player1"));
        winnersNode.addVectorValue(create_string_node("Player3"));
        winnersNode.addVectorValue(create_string_node("Player5"));



        DataNode variablesNode;
        variablesNode.setMapValue("winners", winnersNode);

        variables = GameStateObject(variablesNode);
    }
};

TEST_F(GlobalVariablesTest, GetVariable) {
    EXPECT_EQ(variables.getObjectByName("winners").getVectorValue(0).getString(), "Player1");
    EXPECT_EQ(variables.getObjectByName("winners").getVectorValue(1).getString(), "Player3");
    EXPECT_EQ(variables.getObjectByName("winners").getVectorValue(2).getString(), "Player5");
}

TEST_F(GlobalVariablesTest, SetVariable) {
    DataNode losersNode;
    losersNode.addVectorValue(create_string_node("Player2"));
    losersNode.addVectorValue(create_string_node("Player4"));
    losersNode.addVectorValue(create_string_node("Player6"));

    variables.setObject("losers", losersNode);
    EXPECT_EQ(variables.getObjectByName("losers").getVectorValue(0).getString(), "Player2");
    EXPECT_EQ(variables.getObjectByName("losers").getVectorValue(1).getString(), "Player4");
    EXPECT_EQ(variables.getObjectByName("losers").getVectorValue(2).getString(), "Player6");
}

TEST_F(GlobalVariablesTest, RemoveVariable) {
    variables.removeObject("winners");
    EXPECT_THROW(variables.getObjectByName("winners"),data_node_map_key_not_found);
}

class PerPlayerStateTest : public ::testing::Test{
protected:
    GameStateObject playerStates;

    void SetUp() override{
        DataNode player1Node;
        player1Node.setMapValue("wins", create_int_node(5));

        DataNode player2Node;
        player2Node.setMapValue("wins", create_int_node(0));

        DataNode playerStatesNode;

        playerStatesNode.setMapValue("player1", player1Node);
        playerStatesNode.setMapValue("player2", player2Node);

        playerStates = GameStateObject(playerStatesNode);
    }
};

TEST_F(PerPlayerStateTest, GetState) {
    EXPECT_EQ(playerStates.getObjectByName("player1").getMapValue("wins").getInt(), 5);
    EXPECT_EQ(playerStates.getObjectByName("player2").getMapValue("wins").getInt(), 0);
}

class PerAudienceStateTest : public ::testing::Test{
protected:
    GameStateObject audienceStates;

    void SetUp() override{
        DataNode audience1Node;
        audience1Node.setMapValue("votes", create_int_node(5));

        DataNode audience2Node;
        audience2Node.setMapValue("votes", create_int_node(0));

        DataNode audienceStatesNode;

        audienceStatesNode.setMapValue("audience1", audience1Node);
        audienceStatesNode.setMapValue("audience2", audience2Node);

        audienceStates = GameStateObject(audienceStatesNode);
    }
};

TEST_F(PerAudienceStateTest, GetState) {
    EXPECT_EQ(audienceStates.getObjectByName("audience1").getMapValue("votes").getInt(), 5);
    EXPECT_EQ(audienceStates.getObjectByName("audience2").getMapValue("votes").getInt(), 0);
}