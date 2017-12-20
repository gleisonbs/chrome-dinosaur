#ifndef SESSION_HPP
#define SESSION_HPP

#include <iostream>

#include "population.hpp"

void save_session(const Population&, const std::vector<int>&);
Population load_session(const std::string&);

#endif // SESSION_HPP
