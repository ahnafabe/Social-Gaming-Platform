#include <gtest/gtest.h>

#include "logic/parsers/tree_sitter/TSParser.h"
#include "logic/rules/AssignmentRule.h"

namespace TestTSParser
{

    std::string_view sourceCodeBase =
        "configuration {\n"
        "name: \"Simple\"\n"
        "player range: (2, 4)\n"
        "audience: false\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n";

    struct MockDataNode : DataNode
    {
    };

}

TEST(TestTSParser, ValidSourceCode)
{
    std::string sourceCode = std::string(TestTSParser::sourceCodeBase);
    sourceCode.append(
        "rules {\n"
        "b <- true;\n"
        "y <- 3;\n"
        "x <- 1;\n"
        "}\n");
    EXPECT_NO_THROW({ logic::TSParser parser(sourceCode); });
};

TEST(TestTSParser, InvalidSourceCode)
{
    std::string sourceCode = std::string(TestTSParser::sourceCodeBase);
    sourceCode.append(
        "rules {\n"
        "b <- true\n" // missing semicolon
        "y <- 3;\n"
        "x <- 1;\n"
        "}\n");

    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, FloatAssignmentRule)
{
    std::string sourceCode = std::string(TestTSParser::sourceCodeBase);
    sourceCode.append(
        "rules {\n"
        "a <- 1.1;\n"
        "}\n");
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

// TODO: LOGIC-10: Check values
TEST(TestTSParser, AssignmentRule)
{
    std::string sourceCode = std::string(TestTSParser::sourceCodeBase);
    sourceCode.append(
        "rules {\n"
        "a <- true;\n"
        "b <- false;\n"
        "c <- -1;\n"
        "d <- 0;\n"
        "e <- 1;\n"
        "f <- \"\";\n"
        "g <- \"b\";\n"
        "h <- \"some value\";\n"
        "}\n");

    auto parser = logic::TSParser(sourceCode);

    auto actualSpecs = parser.parseRuleSpecs();
    EXPECT_TRUE(actualSpecs);

    std::vector<logic::AssignmentRuleSpecification> expectedSpecs = {
        {0, "a", logic::SocialGamingType::BOOLEAN, TestTSParser::MockDataNode()},
        {0, "b", logic::SocialGamingType::BOOLEAN, TestTSParser::MockDataNode()},
        {0, "c", logic::SocialGamingType::NUMBER, TestTSParser::MockDataNode()},
        {0, "d", logic::SocialGamingType::NUMBER, TestTSParser::MockDataNode()},
        {0, "e", logic::SocialGamingType::NUMBER, TestTSParser::MockDataNode()},
        {0, "f", logic::SocialGamingType::STRING, TestTSParser::MockDataNode()},
        {0, "g", logic::SocialGamingType::STRING, TestTSParser::MockDataNode()},
        {0, "h", logic::SocialGamingType::STRING, TestTSParser::MockDataNode()}};

    EXPECT_EQ((*actualSpecs).size(), expectedSpecs.size());

    // compare rule specifications  
    for (const auto& expectedSpec : expectedSpecs) {
        auto spec = std::move(actualSpecs->top());
        actualSpecs->pop();
    // // compare rule specifications
    // for (const auto &expectedSpec : expectedSpecs)
    // {
    //     std::unique_ptr<logic::BaseRuleSpecification> spec = std::move((*actualSpecs).top());
    //     (*actualSpecs).pop();

        const auto *actualSpec = dynamic_cast<const logic::AssignmentRuleSpecification *>(spec.get());
        EXPECT_NE(actualSpec, nullptr);
        EXPECT_EQ(*actualSpec, expectedSpec);
    }
};
TEST(TestTSParser, EmptySourceCode)
{
    std::string sourceCode = "";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, MissingConfigurationBlock)
{
    std::string sourceCode =
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "}\n";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, MissingRulesBlock)
{
    std::string sourceCode = std::string(TestTSParser::sourceCodeBase);
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};
TEST(TestTSParser, ValidConfigurationAndRules)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"Complex\"\n"
        "player range: (2, 6)\n"
        "audience: true\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "b <- 5;\n"
        "}\n";
    EXPECT_NO_THROW({ logic::TSParser parser(sourceCode); });
};

TEST(TestTSParser, InvalidConfigurationSyntax)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"Invalid\"\n"
        "player range: (2, 6)\n"
        "audience: true\n"
        "setup {}\n" // Missing colon
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "}\n";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, InvalidRulesSyntax)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"Invalid\"\n"
        "player range: (2, 6)\n"
        "audience: true\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true\n" // Missing semicolon
        "}\n";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, MissingPlayerRangeInConfiguration)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"MissingPlayerRange\"\n"
        "audience: true\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "}\n";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, MissingAudienceInConfiguration)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"MissingAudience\"\n"
        "player range: (2, 6)\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "}\n";
    EXPECT_THROW(
        { logic::TSParser parser(sourceCode); },
        std::invalid_argument);
};

TEST(TestTSParser, ValidConfigurationWithComplexRules)
{
    std::string sourceCode =
        "configuration {\n"
        "name: \"Complex\"\n"
        "player range: (2, 6)\n"
        "audience: true\n"
        "setup: {}\n"
        "}\n"
        "constants {}\n"
        "variables {}\n"
        "per-player {}\n"
        "per-audience {}\n"
        "rules {\n"
        "a <- true;\n"
        "b <- 5;\n"
        "c <- a && b;\n"
        "d <- c || !a;\n"
        "}\n";
    EXPECT_NO_THROW({ logic::TSParser parser(sourceCode); });
};