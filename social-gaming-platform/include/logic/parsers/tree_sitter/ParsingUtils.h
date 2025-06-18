#pragma once

#include "data/configuration.h"
#include "data/data_node.h"
#include <cpp-tree-sitter.h>
namespace logic
{

    class Cursor {
    public:
        Cursor(ts::Node node) : node(node), index(0) {}

        bool hasNext() const {
            return index < node.getNumChildren();
        }

        ts::Node next() {
            return node.getChild(index++);
        }

    private:
        ts::Node node;
        uint32_t index;
    };

    void printConfiguration(const Configuration &config);
    void printDataNode(const DataNode &dataNode, int depth = 0);

}