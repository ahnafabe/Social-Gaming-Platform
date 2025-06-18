#include "tree_sitter/AssignmentSpecBuilder.h"


namespace logic {


    [[nodiscard]] std::optional<std::shared_ptr<AssignmentRuleSpecification>> 
    AssignmentSpecBuilder::createSpec(std::string_view sourceCode, const ts::Node& assignment) noexcept {
        // TODO: LOGIC-10
        int nestedRuleCount = 0;
        ts::Node variableNode = assignment.getChildByFieldName("target");
        std::string variableName = std::string(variableNode.getSourceRange(sourceCode));        
        ts::Node variableExpression = assignment.getChildByFieldName("value");

        if (variableExpression.getNumChildren() == 1) { // simple assignment rule
            const ts::Node child = variableExpression.getChild(0); 
            std::string_view data = child.getSourceRange(sourceCode);
            DataNode variableValue;

            // determine the assignment type
            auto type = toSocialGamingType(child.getType());
            if (!type) {
                return std::nullopt;
            }
            
            // create the data node based on the type
            switch(*type) {
                case SocialGamingType::BOOLEAN:
                    variableValue = create_bool_node(data == "true");
                    break;
                
                case SocialGamingType::NUMBER:
                    variableValue = create_int_node(std::stoi(std::string(data)));
                    break;

                case SocialGamingType::STRING:
                    if (child.getNumChildren() == 2) { // child has no string text (empty string)
                        variableValue = create_string_node("");
                    } else { // child has three children. the middle child is the string text 
                        variableValue = create_string_node(
                            std::string(child.getChild(1).getSourceRange(sourceCode)));
                    }
                    break;

                // TODO: LOGIC-10: Add support for variables (identifiers, list and maps)
    
                default: // No matching SocialGamingType
                    return std::nullopt;
            }

            return std::make_shared<AssignmentRuleSpecification>(
                nestedRuleCount, 
                variableName, 
                *type, 
                variableValue);
        } else { // complex assignment rule
            // TODO: LOGIC-11: Add support of numerical operations
            return std::nullopt;
        }
    } // end of assignmentRule()

}