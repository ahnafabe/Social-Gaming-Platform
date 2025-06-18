#include <gtest/gtest.h>

#include "data/data_node.h"

// Reference: GoogleTest Primer: Test Fixtures 
//https://google.github.io/googletest/primer.html

class DataNodeTest : public ::testing::Test {
protected:
    DataNode intNode{10};
    DataNode boolNode{true};
    DataNode stringNode{std::string("test")};
    DataNode vectorNode{std::vector<DataNode>{DataNode(1), DataNode(2)}};
    DataNode emptyVectorNode{std::vector<DataNode>{}};  
    DataNode mapNode{std::map<std::string, DataNode>{{"key1", DataNode(1)}, {"key2", DataNode(2)}}};
    DataNode emptyMapNode{std::map<std::string, DataNode>{}};
    DataNode monostateNode;
    DataNode rangeNode{Range(0, 10)};    
};

TEST_F(DataNodeTest, IntType) {
    // Getter tests
    EXPECT_TRUE(intNode.isInt());
    EXPECT_EQ(intNode.getInt(), 10);

    // Setter test
    intNode.setInt(100);
    EXPECT_EQ(intNode.getInt(), 100);
    EXPECT_THROW(intNode.getString(), data_node_of_wrong_type);
}

TEST_F(DataNodeTest, BoolType) {
    // Getter tests
    EXPECT_TRUE(boolNode.isBool());
    EXPECT_EQ(boolNode.getBool(), true);

    // Setter test
    boolNode.setBool(false);
    EXPECT_EQ(boolNode.getBool(), false);
    EXPECT_THROW(boolNode.getString(), data_node_of_wrong_type);
}

TEST_F(DataNodeTest, StringType) {
    // Getter tests
    EXPECT_TRUE(stringNode.isString());
    EXPECT_EQ(stringNode.getString(), "test");

    // Setter test
    stringNode.setString("Hachee");
    EXPECT_EQ(stringNode.getString(), "Hachee");
    EXPECT_THROW(stringNode.getInt(), data_node_of_wrong_type);
}

TEST_F(DataNodeTest, VectorType) {
    // Getter tests
    EXPECT_TRUE(vectorNode.isVector());
    EXPECT_EQ(vectorNode.getVector().size(), 2);
    EXPECT_EQ(vectorNode.getVectorValue(0).getInt(), 1);
    EXPECT_EQ(vectorNode.getVectorValue(1).getInt(), 2);
    EXPECT_TRUE(emptyVectorNode.isEmptyVector());
    
    // Setter test
    vectorNode.setVector({DataNode(3), DataNode(4)});
    std::vector<DataNode> expectedVector = {DataNode(3), DataNode(4)};
    EXPECT_EQ(vectorNode.getVector(), expectedVector);
    EXPECT_THROW(vectorNode.getString(), data_node_of_wrong_type);
}


TEST_F(DataNodeTest, MapType) {
    // Getter tests
    EXPECT_TRUE(mapNode.isMap());
    EXPECT_EQ(mapNode.getMap().size(), 2);
    EXPECT_EQ(mapNode.getMapValue("key1").getInt(), 1);
    EXPECT_EQ(mapNode.getMapValue("key2").getInt(), 2);
    EXPECT_TRUE(emptyMapNode.isEmptyMap());
    
    // Setter test
    mapNode.setMap({{"key3", DataNode(30)}, {"key4", DataNode(40)}});
    std::map<std::string, DataNode> expectedMap = {{"key3", DataNode(30)}, {"key4", DataNode(40)}};
    EXPECT_EQ(mapNode.getMap(), expectedMap);
    EXPECT_THROW(mapNode.getString(), data_node_of_wrong_type);
}

TEST_F(DataNodeTest, RangeType) {
    // Getter tests
    EXPECT_TRUE(rangeNode.isRange());
    Range expectedRange(0, 10);
    EXPECT_EQ(rangeNode.getRange(), expectedRange);
}

TEST_F(DataNodeTest, MonostateType) {
    // Getter tests
    EXPECT_TRUE(monostateNode.isMonostate());
    EXPECT_THROW(monostateNode.getInt(), data_node_of_wrong_type);
    EXPECT_THROW(monostateNode.getBool(), data_node_of_wrong_type);
    EXPECT_THROW(monostateNode.getString(), data_node_of_wrong_type);
}

TEST_F(DataNodeTest, MixedVector) {
    DataNode mixedVectorNode(std::vector<DataNode>{
        DataNode(30),
        DataNode{std::string("hi")},
        DataNode(true)
    });

    EXPECT_TRUE(mixedVectorNode.isVector());
    EXPECT_EQ(mixedVectorNode.getVector().size(), 3);
    auto vector = mixedVectorNode.getVector();
    EXPECT_TRUE(vector[0].isInt());
    EXPECT_EQ(vector[0].getInt(), 30);
    EXPECT_TRUE(vector[1].isString());
    EXPECT_EQ(vector[1].getString(), "hi");
    EXPECT_TRUE(vector[2].isBool());
    EXPECT_EQ(vector[2].getBool(), true);
}

TEST_F(DataNodeTest, ErrorHandling) {
    // Incorrect getters
    EXPECT_THROW(boolNode.getInt(), data_node_of_wrong_type);
    EXPECT_THROW(intNode.getBool(), data_node_of_wrong_type);
    EXPECT_THROW(intNode.getString(), data_node_of_wrong_type);
    EXPECT_THROW(vectorNode.getVectorValue(2), data_node_vector_index_out_of_bounds);
    EXPECT_THROW(mapNode.getMapValue("key3"), data_node_map_key_not_found);
    EXPECT_THROW(intNode.getRange(), data_node_of_wrong_type);

    // Accessing invalid index
    EXPECT_THROW(vectorNode.getVectorValue(-1), data_node_vector_index_out_of_bounds);
    EXPECT_THROW(vectorNode.getVectorValue(1000), data_node_vector_index_out_of_bounds);
    EXPECT_THROW(mapNode.getMapValue("void"), data_node_map_key_not_found);
}

TEST_F(DataNodeTest, LargeVectorTest) {
    std::vector<DataNode> largeVector;
    for (int i = 0; i < 100000; ++i) {
        largeVector.emplace_back(DataNode(i));
    }
    DataNode vectorNode(std::move(largeVector));
    ASSERT_TRUE(vectorNode.isVector());
    EXPECT_EQ(vectorNode.getVector().size(), 100000);
}

TEST_F(DataNodeTest, LargeMapTest) {
    std::map<std::string, DataNode> largeMap;
    for (int i = 0; i < 100000; ++i) {
        largeMap["key" + std::to_string(i)] = DataNode(i);
    }
    DataNode mapNode(std::move(largeMap));
    ASSERT_TRUE(mapNode.isMap());
    EXPECT_EQ(mapNode.getMap().size(), 100000);
    EXPECT_EQ(mapNode.getMapValue("key99999").getInt(), 99999);
}