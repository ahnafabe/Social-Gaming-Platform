#include "data/errors.h"

data_node_of_wrong_type::data_node_of_wrong_type(const std::string_view demanded_type, const std::string_view actual_type) : message("A value of type '" + std::string(demanded_type) + "' was requested from a DataNode of type '" + std::string(actual_type) + "'")
  {
  }

const char *data_node_of_wrong_type::what() const noexcept { 
  return message.c_str(); 
  }

const char *data_node_vector_index_out_of_bounds::what() const noexcept { 
  return "Vector index is out of bounds"; 
}

const char *data_node_map_key_not_found::what() const noexcept { 
  return "Key not found in map"; 
}