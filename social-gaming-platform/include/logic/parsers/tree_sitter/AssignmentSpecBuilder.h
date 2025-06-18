#pragma once

#include <optional>
#include <string_view>

#include <cpp-tree-sitter.h>

#include "data/data_node.h"
#include "rule_specifications/AssignmentRuleSpecification.h"
#include "Grammar.h"


namespace logic {


    class AssignmentSpecBuilder {
        public:
        // TODO: LOGIC-10 to complete the implementation

        /**
         * Create a AssignmentNode given a tree-sitter assignment node
         * 
         * @param assignment Tree-sitter node with an assignment rule.
         */
        [[nodiscard]] std::optional<std::shared_ptr<AssignmentRuleSpecification>> 
        createSpec(std::string_view sourceCode, const ts::Node& assignment) noexcept;
    };

}