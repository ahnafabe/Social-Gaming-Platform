#pragma once

#include "data/errors.h"

#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <string>
#include <variant>
#include <cstddef>
#include <sstream>
#include <type_traits>
#include <stdexcept>
#include <list>

// From https://stackoverflow.com/questions/505021/get-bytes-from-stdstring-in-c#comment319982_505080

/*This struct acts sort of like an enum, in that it is defining valid types that should be passed by value instead of reference
    -Used when obtaining a value in a vector or map, where the type wouldn't be known beforehand
    -Reasoning: Primitive and simple types like string_view should be passed by value. Vectors and Maps should be passed by reference to preserve performance.
template <typename T>
struct return_by_value_types;
*/
using Range = std::pair<int, int>;

class DataNode
{
private:
  std::variant<std::monostate, int, bool, Range, std::string, std::vector<DataNode>, std::map<std::string, DataNode>> data_node;
  
  std::string get_type_name() const;
  void assert_is_map() const;
  void assert_is_vector() const;

public:
  // Constructors declaration
  DataNode();
  DataNode(int value);
  DataNode(bool value);
  DataNode(Range value);

  DataNode(const std::string& value);
  DataNode(const std::vector<DataNode>& value);
  DataNode(const std::map<std::string, DataNode>& value);

  //Functions declaration
  //Type checking 
  bool isVector() const;
  bool isMap() const;
  bool isInt() const;
  bool isBool() const;
  bool isString() const;
  bool isRange() const;
  bool isMonostate() const;

  bool isEmptyMap() const;
  bool isEmptyVector() const;

  //Getters
  int getInt() const;
  bool getBool() const;
  Range getRange() const;
  std::string_view getString() const;

  std::vector<DataNode>& getVector();
  const std::vector<DataNode>& getVector() const;   //Read only version
  std::map<std::string, DataNode>& getMap();
  const std::map<std::string, DataNode>& getMap() const;   //Read only version

  DataNode& getVectorValue(size_t index);
  const DataNode& getVectorValue(size_t index) const; //Read only version
  DataNode& getMapValue(std::string_view key);
  const DataNode& getMapValue(std::string_view key) const; //Read only version

  //Setters
  void setMonostate();
  void setInt(int value);
  void setBool(bool value);
  void setString(const std::string& value);
  void setVector(const std::vector<DataNode>& value);
  void setMap(const std::map<std::string, DataNode>& value);

  void addVectorValue(const DataNode& value);
  void setVectorValue(size_t index, const DataNode& value);
  void setMapValue(std::string_view key, const DataNode& value);

  void removeVectorValue(size_t index);
  void removeMapValue(std::string_view key);
  bool operator==(const DataNode& other) const;
};

DataNode create_monostate_node();
DataNode create_int_node(int value);
DataNode create_bool_node(bool value);
DataNode create_string_node(std::string_view value);
DataNode create_range_node(Range value);
DataNode create_vector_node(std::vector<DataNode>& value);
DataNode create_map_node(std::map<std::string, DataNode>& value);
DataNode create_map_node();
DataNode create_vector_node();