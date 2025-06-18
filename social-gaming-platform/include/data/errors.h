#pragma once

#include <stdexcept>
#include <string>

class data_node_of_wrong_type : public std::exception
{
private:
  const std::string message;

public:
  data_node_of_wrong_type(const std::string_view demanded_type, const std::string_view actual_type);

  const char *what() const noexcept override;
};

class data_node_vector_index_out_of_bounds : public std::exception
{
public:
  const char *what() const noexcept override;
};

class data_node_map_key_not_found : public std::exception
{
public:
  const char *what() const noexcept override;
};