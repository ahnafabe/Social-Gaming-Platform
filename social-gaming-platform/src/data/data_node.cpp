#include "data/data_node.h"

//DataNode Constructor defintions
DataNode::DataNode() : data_node(std::monostate{})
{}

DataNode::DataNode(int value) : data_node(value)
{}

DataNode::DataNode(bool value) : data_node(value)
{}

DataNode::DataNode(Range value) : data_node(value)
{}

DataNode::DataNode(const std::string& value) : data_node(value)
{}

DataNode::DataNode(const std::vector<DataNode>& value) : data_node(value) 
{}

DataNode::DataNode(const std::map<std::string, DataNode>& value) : data_node(value) 
{}

// DataNode Function definitions
bool DataNode::isVector() const
  {
    return std::holds_alternative<std::vector<DataNode>>(data_node);
  }

bool DataNode::isMap() const
  {
    return std::holds_alternative<std::map<std::string, DataNode>>(data_node);
  }

bool DataNode::isInt() const
  {
    return std::holds_alternative<int>(data_node);
  }

bool DataNode::isBool() const
  {
    return std::holds_alternative<bool>(data_node);
  }

bool DataNode::isString() const
  {
    return std::holds_alternative<std::string>(data_node);
  }

bool DataNode::isRange() const
  {
    return std::holds_alternative<Range>(data_node);
  }

bool DataNode::isMonostate() const
  {
    return std::holds_alternative<std::monostate>(data_node);
  }

bool DataNode::isEmptyVector() const
  {
    if (!isVector())
    {
      throw data_node_of_wrong_type("vector", get_type_name());
    }

    auto &vec = getVector();
    return vec.empty();
  }

bool DataNode::isEmptyMap() const
  {
    if (!isMap())
    {
      throw data_node_of_wrong_type("map", get_type_name());
    }

    auto &map = getMap();
    return map.empty();
  }

int DataNode::getInt() const
  {
    if(isInt()){
      return std::get<int>(data_node);
    }
    else{
      throw data_node_of_wrong_type("integer", get_type_name());
    }
  }

bool DataNode::getBool() const
  {
    if(isBool()){
      return std::get<bool>(data_node);
    }
    else{
      throw data_node_of_wrong_type("boolean", get_type_name());
    }
  }

Range DataNode::getRange() const
  {
    if(isRange()){
      return std::get<Range>(data_node);
    }
    else{
      throw data_node_of_wrong_type("range", get_type_name());
    }
  }

std::string_view DataNode::getString() const
  {
    if(isString()){
      return std::get<std::string>(data_node);
    }
    else{
      throw data_node_of_wrong_type("string", get_type_name());
    }
  }

std::vector<DataNode>& DataNode::getVector()
  {
    if(isVector()){
      return std::get<std::vector<DataNode>>(data_node);
    } else{
      throw data_node_of_wrong_type("vector", get_type_name());
    }
  }

const std::vector<DataNode>& DataNode::getVector() const    //Read only version
  {
    if(isVector()){
      return std::get<std::vector<DataNode>>(data_node);
    } else{
      throw data_node_of_wrong_type("vector", get_type_name());
    }
  }

std::map<std::string, DataNode>& DataNode::getMap()
  {
     if(isMap()){
      return std::get<std::map<std::string, DataNode>>(data_node);
    }
    else{
      throw data_node_of_wrong_type("map", get_type_name());
    }
  }

const std::map<std::string, DataNode>& DataNode::getMap() const    //Read only version
  {
    if(isMap()){
      return std::get<std::map<std::string, DataNode>>(data_node);
    }
    else{
      throw data_node_of_wrong_type("map", get_type_name());
    }
  }

DataNode& DataNode::getVectorValue(size_t index){
  assert_is_vector();
  auto& vec = getVector();

  if(index >= vec.size()){
    throw data_node_vector_index_out_of_bounds();
  }

  return vec[index];
}

const DataNode& DataNode::getVectorValue(size_t index) const{
  assert_is_vector();
  auto& vec = getVector();

  if (index >= vec.size())
  {
    throw data_node_vector_index_out_of_bounds();
  }

  return vec[index];
}

DataNode& DataNode::getMapValue(std::string_view key){
  assert_is_map();
  auto& map = getMap();

  auto it = map.find(std::string(key));
  if (it == map.end())
  {
    throw data_node_map_key_not_found();
  }

  return it->second;
}

const DataNode& DataNode::getMapValue(std::string_view key) const{
  assert_is_map();
  const auto& map = getMap();

  auto it = map.find(std::string(key));
  if(it == map.end()){
    throw data_node_map_key_not_found();
  }

  return it->second;
}

void DataNode::setInt(int value){
  data_node = value;
}

void DataNode::setBool(bool value){
  data_node = value;
}

void DataNode::setString(const std::string& value){
  data_node = value;
}

void DataNode::setVector(const std::vector<DataNode>& value){
  data_node = value;
}

void DataNode::setMap(const std::map<std::string, DataNode>& value){
  data_node = value;
}

void DataNode::addVectorValue(const DataNode& value){
  if(isMonostate()){
    data_node = std::vector<DataNode>();
  }

  assert_is_vector();
  auto& vec = getVector();
  vec.push_back(value);
}

void DataNode::setVectorValue(size_t index, const DataNode& value){
  assert_is_vector();
  auto& vec = getVector();

  if(index >= vec.size()){
    throw data_node_vector_index_out_of_bounds();
  }

  vec[index] = value;
}

void DataNode::setMapValue(std::string_view key, const DataNode& value){
  if(isMonostate()){
    data_node = std::map<std::string, DataNode>();
  }

  assert_is_map();
  auto& map = getMap();

  map[std::string(key)] = value;
}

void DataNode::removeVectorValue(size_t index){
  assert_is_vector();

  auto& vec = getVector();

  if(index >= vec.size()){
    throw data_node_vector_index_out_of_bounds();
  }

  vec.erase(vec.begin() + index);
}

void DataNode::removeMapValue(std::string_view key){
  assert_is_map();

  auto& map = getMap();
  map.erase(std::string(key));
}

std::string DataNode::get_type_name() const
{
  if (isInt())
  {
    return "int";
  }
  else if (isBool())
  {
    return "bool";
  }
  else if (isString())
  {
    return "string";
  }
  else if (isRange())
  {
    return "range";
  }
  else if (isVector())
  {
    return "vector";
  }
  else if (isMap())
  {
    return "map";
  }
  else if (isMonostate())
  {
    return "monostate";
  }
  else
  {
    return "unknown";
  }
}

void DataNode::assert_is_map() const
{
  if (!isMap())
  {
    throw data_node_of_wrong_type("map", get_type_name());
  }
}

void DataNode::assert_is_vector() const
{
  if (!isVector())
  {
    throw data_node_of_wrong_type("vector", get_type_name());
  }
}

// Additional functions: functions to create nodes
DataNode create_int_node(int value)
{
  return DataNode(value);
}

DataNode create_bool_node(bool value)
{
  return DataNode(value);
}

DataNode create_string_node(std::string_view value)
{
  return DataNode(std::string(value));
}

DataNode create_range_node(Range value)
{
  return DataNode(value);
}

DataNode create_vector_node(const std::vector<DataNode>& value)
{
  return DataNode(value);
}

DataNode create_map_node(const std::map<std::string, DataNode>& value)
{
  return DataNode(value);
}

DataNode create_map_node()
{
  return DataNode(std::map<std::string,DataNode>{});
}

DataNode create_vector_node()
{
  return DataNode(std::vector<DataNode>{});
}

DataNode create_monostate_node()
{
  return DataNode();
}

bool DataNode::operator==(const DataNode& other) const 
{
    return data_node == other.data_node;
}