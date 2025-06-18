#include "tree_sitter/TSParser.h"
#include "tree_sitter/TSRuleSpecFactory.h"


namespace logic {

    TSParser::TSParser(std::string sourceCode)
        : sourceCode(sourceCode), 
          parser(socialGaming),
          syntaxTree(parser.parseString(sourceCode))
    {
        if (syntaxTree.hasError()) 
        {
            throw std::invalid_argument(
                std::string("Source code has an error: ")
                + syntaxTree.getRootNode().getSExpr().get()  
            );
        }
    } // end of TreeSitterParser()


    void TSParser::printTreeSitterDetails() const noexcept {
        ts::Children children{syntaxTree.getRootNode()};
        printNamedChildren(children);
    } // end of printTreeSitterDetails()


    void TSParser::printNamedChildren(ts::Children children) const noexcept {
        for (const ts::Node &child : children) {
            if (child.isNamed()) {
                std::cout << "Child Node Type: " << child.getType() << "\n";
                std::cout << "Child Node Symbol: " << child.getSymbol() << "\n";

                auto byteRange = child.getByteRange();
                std::cout << "Byte Range: " << byteRange.start << " - " << byteRange.end << "\n";
                std::cout << "Source Range: " << child.getSourceRange(sourceCode) << "\n\n\n";
            }
            printNamedChildren(ts::Children(child));
        }
    } // end of printNamedChildren()


    [[nodiscard]] std::expected<Configuration, std::string> 
    TSParser::parseConfigurationFieldImpl() const noexcept {
        ts::Node rootNode = syntaxTree.getRootNode();
        ts::Node configurationNode = rootNode.getChildByFieldName("configuration");
        if (configurationNode.isNull()) {
            return std::unexpected("Error: Configuration node is null or not found.");
        }
        auto rootDataNode = traverseTree(configurationNode);
        if (!rootDataNode) {
            return rootDataNode;
        }
        
        return Configuration(*rootDataNode);

    } // end of parseConfigurationFieldImpl()

     
    [[nodiscard]] std::expected<GameStateObject, std::string> 
    TSParser::parseGlobalConstantsFieldImpl() const noexcept {
        DataNode globalConstantsNode = create_map_node();
        std::vector<std::string> processedLines;

        ts::Node rootNode = syntaxTree.getRootNode();
        ts::Node globalConstants = rootNode.getChildByFieldName("constants");
        if (globalConstants.isNull()) {
            return std::unexpected("Error: Global constants node is null.");
        }

        auto rootDataNode = traverseTree(globalConstants);
        if (!rootDataNode) {
            return rootDataNode;
        }
        return GameStateObject(*rootDataNode);
    } // end of parseGlobalConstantsFieldImpl()


    [[nodiscard]] std::expected<GameStateObject, std::string> 
    TSParser::parseGlobalVariablesFieldImpl() const noexcept {
        GameStateObject variableNode = GameStateObject();
        std::vector<std::string> processedLines;

        ts::Node rootNode = syntaxTree.getRootNode();
        ts::Node globalVariables = rootNode.getChildByFieldName("variables");
        if (globalVariables.isNull()) {
            return std::unexpected("Error: Global variables node is null.");
        }

        auto rootDataNode = traverseTree(globalVariables);
        if (!rootDataNode) {
            return rootDataNode;
        }
        return GameStateObject(*rootDataNode);
    } // end of parseGlobalVariablesFieldImpl()


    [[nodiscard]] std::expected<GameStateObject, std::string>
    TSParser::parsePlayerFieldImpl() const noexcept {
        DataNode playerNode = create_map_node();
        std::vector<std::string> processedLines;

        ts::Node rootNode = syntaxTree.getRootNode();
        // Iterate over children and print field names
        uint32_t numChildren = rootNode.getNumChildren();
        ts::Node player = rootNode.getChildByFieldName("per_player");
        if (player.isNull()) {
            return std::unexpected("Error: Per-player node is null.");
            
        }
        auto rootDataNode = traverseTree(player);
        if (!rootDataNode) {
            return rootDataNode;
        }
        return GameStateObject(*rootDataNode);
    } // end of parserPlayerFieldImpl()


    [[nodiscard]] std::expected<GameStateObject, std::string> 
    TSParser::parseAudienceFieldImpl() const noexcept {
        DataNode audienceNode = create_map_node();
        std::vector<std::string> processedLines;

        ts::Node rootNode = syntaxTree.getRootNode();

        // Iterate over children and print field names
        uint32_t numChildren = rootNode.getNumChildren();
        ts::Node audience = rootNode.getChildByFieldName("per-audience");
        if (audience.isNull()) {
            return std::unexpected("Error: Audience node is null.");
        }
        auto rootDataNode = traverseTree(audience);
        if (!rootDataNode) {
            return rootDataNode;
        }
        return GameStateObject(*rootDataNode);
    } // end of parseAudienceFieldImpl()


    [[nodiscard]] std::expected<DataNode, std::string> 
    TSParser::parseRange(Cursor &cursor) const noexcept {
        int start = 0, end = 0;
        bool firstValueCaptured = false;

        while (cursor.hasNext()) {
            ts::Node part = cursor.next();
            std::string_view partContent = part.getSourceRange(sourceCode);

            if (partContent == "(" || partContent == ")" || partContent == ",") {
                continue; // Skip non-numeric parts
            }

            try {
                int number = std::stoi(std::string(partContent));
                if (!firstValueCaptured) {
                    start = number;
                    firstValueCaptured = true;
                }
                else {
                    end = number;
                    break;
                }
            }
            catch (const std::exception &e) {
                std::unexpected(std::string("Warning: Non-numeric range value encountered: ") 
                                + std::string(partContent));
            }
        }

        return create_range_node(std::make_pair(start, end));
    } // end of parserRange()

    [[nodiscard]] std::expected<DataNode, std::string> 
    TSParser::parseList(Cursor &cursor) const noexcept {
        DataNode listNode = create_vector_node();
        while (cursor.hasNext()) {
            ts::Node element = cursor.next();
            std::string_view elementContent = element.getSourceRange(sourceCode);
            if (elementContent == "]")
                break;
            
            if (elementContent != "[" && elementContent != ",") {
                auto itemNode = traverseTree(element);
                if (!itemNode) {
                    return itemNode;
                }
                listNode.addVectorValue(*itemNode);
            }
        }
        return listNode;
    } // end of parseList()


    [[nodiscard]] std::expected<DataNode, std::string> 
    TSParser::traverseTree(const ts::Node &node) const noexcept {
        DataNode dataNode = create_map_node();
        Cursor cursor(node);
        std::string_view previousContent; // Track the previous key-like content

        while (cursor.hasNext()) {
            ts::Node child = cursor.next();
            std::string_view content = child.getSourceRange(sourceCode);
            /*
            std::cout << "Node type: " << child.getType() << std::endl;
            std::cout << "Node content: " << child.getSourceRange(sourceCode) << std::endl;
            std::cout << "Node has " << child.getNumChildren() << " children.\n===============================\n"<< std::endl;
            */

            // Check if the previous content was a key we're interested in
            /*
            if (previousContent == NAME_KEY) {
                if (!content.empty()) {
                    dataNode.setMapValue("name", create_string_node(std::string(content)));
                    std::cout << "\nCaptured Name: " << content << "\n"
                              << std::endl; // Logging
                }
                previousContent = ""; // Clear after use
                continue;
            }

            if (previousContent == AUDIENCE_KEY) {
                bool boolValue = (content == "true");
                dataNode.setMapValue("audience", create_bool_node(boolValue));
                std::cout << "Captured Audience: " << (boolValue ? "true" : "false") << "\n"
                          << std::endl; // Logging
                previousContent = "";   // Clear after use
                continue;
            }

            if (previousContent == PLAYER_RANGE_KEY) {
                Cursor rangeCursor(child);
                auto rangeNode = parseRange(rangeCursor);
                if (!rangeNode) {
                    return rangeNode;
                }

                // Assuming parseRange returns a DataNode with type Range.
                if (rangeNode->isRange()) {
                    dataNode.setMapValue(std::string(previousContent), *rangeNode);
                    Range capturedRange = rangeNode->getRange();
                    std::cout << "Captured Range: (" << capturedRange.first << ", " << capturedRange.second << ")" << std::endl;
                }
                else {
                    std::cerr << "Error: Range data not captured correctly" << std::endl;
                }

                previousContent = ""; // Clear after use
                continue;
            }

            // If we haven't found a value, store the current node as the key
            if (content == NAME_KEY || content == AUDIENCE_KEY || content == PLAYER_RANGE_KEY || content == RANGE_KEY)
            {
                previousContent = content;
                continue;
            }
            */

            // Recursively process any other nested structures
            auto childDataNode = traverseTree(child);
            if (!childDataNode) {
                return childDataNode;
            }

            if (!childDataNode->isEmptyMap()) {
                dataNode.setMapValue(std::string(child.getType()), *childDataNode);
            }
        }

        return dataNode;
    } // end of traverseTree


    [[nodiscard]] std::expected<RuleSpecStack, std::string> 
    TSParser::parseRuleSpecsImpl() noexcept {
        auto ruleSpecs = RuleSpecs();
        auto ruleSpecFactory = TSRuleSpecFactory(sourceCode);

        // find the rules node in the syntaxTree
        ts::Node tsRules = syntaxTree.getRootNode()
                                     .getChildByFieldName("rules")
                                     .getChildByFieldName("body");

        for (const ts::Node& tsRule : ts::Children(tsRules)) {
            if (tsRule.getType() == "rule") {
                const ts::Node child = tsRule.getNamedChild(0);
                
                // determine the type of rule to be parsed
                auto ruleType = toRuleType(child.getType());
                if (!ruleType) {
                    return std::unexpected(std::string("Unknown/not implemented rule for ") 
                                           + std::string(child.getType()));
                }

                // create the rule
                auto ruleSpec = ruleSpecFactory.createRuleSpec(*ruleType, child);
                if (!ruleSpec) {
                    return std::unexpected(std::string("Unknown/not implemented rule for ") 
                                           + std::string(child.getType()));
                }
                ruleSpecs.push_back(std::move(*ruleSpec));
            }
        }

        // move the rulesSpec into a stack
        auto ruleSpecStack = RuleSpecStack();
        for (auto spec = ruleSpecs.rbegin(); spec != ruleSpecs.rend(); spec++) {
            ruleSpecStack.push(std::move(*spec));
        }

        return std::move(ruleSpecStack);
    } // end of parseRuleSpecsImpl()


    [[nodiscard]] std::optional<RuleType> 
    TSParser::toRuleType(std::string_view ruleType) const noexcept {
        if (ruleType == "assignment") return RuleType::ASSIGNMENT;
        return std::nullopt;
    } // end of toRuleType()


    /* OVERLOADED METHODS */

    std::ostream &operator<<(std::ostream &os, const ts::Node &node) {
        os << "Node Type: " << node.getType(); // Adjust this to match your `ts::Node` API
        return os;
    }

    bool operator==(const ts::Node &lhs, const ts::Node &rhs) {
        return ts_node_eq(lhs.impl, rhs.impl);
    }

}