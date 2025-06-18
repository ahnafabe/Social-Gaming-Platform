#include <gtest/gtest.h>

#include "logic/parsers/tree_sitter/TSRuleSpecFactory.h"


namespace TestTSRuleSpecFactory {

    extern "C" { TSLanguage *tree_sitter_socialgaming(); }
    ts::Language socialGaming = tree_sitter_socialgaming();
    ts::Parser parser{socialGaming};

    struct MockDataNode : DataNode {};

    std::string sourceCodeBase = 
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
}


/* HELPER FUNCTIONS */


logic::RuleSpecs 
parseRulesForSpecs(std::string sourceCode, logic::RuleType ruleType) {
    ts::Tree syntaxTree = TestTSRuleSpecFactory::parser.parseString(sourceCode);
    auto factory = logic::TSRuleSpecFactory(sourceCode);

    // find the rules node in the syntaxTree
    ts::Node tsRules = syntaxTree.getRootNode()
                                 .getChildByFieldName("rules")
                                 .getChildByFieldName("body");

    logic::RuleSpecs ruleSpecs;
    for (const ts::Node& tsRule : ts::Children(tsRules)) {
        if (tsRule.getType() == "rule") {
            const ts::Node child = tsRule.getNamedChild(0);
            auto ruleSpec = factory.createRuleSpec(ruleType, child);
            EXPECT_NE(ruleSpec, std::nullopt);
            ruleSpecs.push_back(std::move(*ruleSpec));                
        }
    }
    return ruleSpecs;
}


std::vector<logic::AssignmentRuleSpecification> 
extractAssignmentRuleSpecs(logic::RuleSpecs& ruleSpecs) {
    std::vector<logic::AssignmentRuleSpecification> specs;
    specs.reserve(ruleSpecs.size());

    for (const auto& baseSpec : ruleSpecs) {
        const auto *spec = dynamic_cast<const logic::AssignmentRuleSpecification*>(baseSpec.get());
        EXPECT_NE(spec, nullptr);
        specs.push_back(std::move(*spec));
    }
    return std::move(specs);
}


/* TEST ASSIGNMENT RULES */


TEST(TestTSRuleSpecFactory, TestBooleanAssignmentRule) {
    std::string sourceCode = std::string(TestTSRuleSpecFactory::sourceCodeBase);
    sourceCode.append( 
        "rules {\n"
            "a <- true;\n"
            "b <-false;\n"
        "}\n"
    );

    logic::RuleSpecs actualSpecs = parseRulesForSpecs(sourceCode, logic::RuleType::ASSIGNMENT);
    std::vector<logic::AssignmentRuleSpecification> expectedSpecs = {
        {0, "a", logic::SocialGamingType::BOOLEAN, TestTSRuleSpecFactory::MockDataNode()},
        {0, "b", logic::SocialGamingType::BOOLEAN, TestTSRuleSpecFactory::MockDataNode()}
    };
    
    EXPECT_EQ(expectedSpecs, extractAssignmentRuleSpecs(actualSpecs));
};


TEST(TestTSRuleSpecFactory, TestNumberAssignmentRule) {
    std::string sourceCode = std::string(TestTSRuleSpecFactory::sourceCodeBase);
    sourceCode.append( 
        "rules {\n"
            "a <- -1;\n"
            "b <- 0;\n"
            "c <- 1;\n"
        "}\n"
    );

    logic::RuleSpecs actualSpecs = parseRulesForSpecs(sourceCode, logic::RuleType::ASSIGNMENT);
    std::vector<logic::AssignmentRuleSpecification> expectedSpecs = {
        {0, "a", logic::SocialGamingType::NUMBER, TestTSRuleSpecFactory::MockDataNode()},
        {0, "b", logic::SocialGamingType::NUMBER, TestTSRuleSpecFactory::MockDataNode()},
        {0, "c", logic::SocialGamingType::NUMBER, TestTSRuleSpecFactory::MockDataNode()}
    };
    
    EXPECT_EQ(expectedSpecs, extractAssignmentRuleSpecs(actualSpecs));
};


// TODO: min int. Types will be defined by the data team
// TODO: max int. Types wil be defined by the data team


TEST(TestTSRuleSpecFactory, TestStringAssignmentRule) {
    std::string sourceCode = std::string(TestTSRuleSpecFactory::sourceCodeBase);
    sourceCode.append( 
        "rules {\n"
            "a <- \"\";\n"
            "b <- \"b\";\n"
            "c <- \"some value\";\n"
        "}\n"
    );

    logic::RuleSpecs actualSpecs = parseRulesForSpecs(sourceCode, logic::RuleType::ASSIGNMENT);
    std::vector<logic::AssignmentRuleSpecification> expectedSpecs = {
        {0, "a", logic::SocialGamingType::STRING, TestTSRuleSpecFactory::MockDataNode()},
        {0, "b", logic::SocialGamingType::STRING, TestTSRuleSpecFactory::MockDataNode()},
        {0, "c", logic::SocialGamingType::STRING, TestTSRuleSpecFactory::MockDataNode()}
    };
    
    EXPECT_EQ(expectedSpecs, extractAssignmentRuleSpecs(actualSpecs));
};
