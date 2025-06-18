#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "tree_sitter/ParsingUtils.h"   // Header for printDataNode and printConfiguration
#include "data/data_node.h" // Assuming this file defines DataNode and Range

namespace logic
{

    void printDataNode(const DataNode &dataNode, int depth)
    {
        std::string indent(depth * 2, ' ');

        if (dataNode.isMap())
        {
            const auto &mapData = dataNode.getMap();
            for (const auto &[key, value] : mapData)
            {
                std::cout << indent << key << ": ";

                if (value.isString())
                {
                    std::cout << "\"" << value.getString() << "\"" << std::endl;
                }
                else if (value.isBool())
                {
                    std::cout << (value.getBool() ? "true" : "false") << std::endl;
                }
                else if (value.isInt())
                {
                    std::cout << value.getInt() << std::endl;
                }
                else if (value.isRange())
                {
                    Range range = value.getRange();
                    std::cout << "(" << range.first << ", " << range.second << ")" << std::endl;
                }
                else if (value.isVector())
                {
                    const auto &vectorData = value.getVector();
                    std::cout << "[";
                    for (size_t i = 0; i < vectorData.size(); ++i)
                    {
                        if (vectorData[i].isInt())
                        {
                            std::cout << vectorData[i].getInt();
                        }
                        else if (vectorData[i].isString())
                        {
                            std::cout << "\"" << vectorData[i].getString() << "\"";
                        }
                        if (i < vectorData.size() - 1)
                        {
                            std::cout << ", ";
                        }
                    }
                    std::cout << "]" << std::endl;
                }
                else if (value.isMap())
                {
                    std::cout << std::endl;
                    printDataNode(value, depth + 1);
                }
                else
                {
                    std::cout << "Unknown type" << std::endl;
                }
            }
        }
        else if (dataNode.isString())
        {
            std::cout << indent << "\"" << dataNode.getString() << "\"" << std::endl;
        }
    }

    void printConfiguration(const Configuration &config)
    {
        const DataNode &rootDataNode = config.getDataNode();
        printDataNode(rootDataNode);
    }

} // namespace logic