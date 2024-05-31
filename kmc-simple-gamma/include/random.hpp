#pragma once

#include <cstddef>
#include <string>

std::size_t generate_uuid(void);
std::size_t uniform_random_sizet(void);

double uniform_random_real(void);
double uniform_random_real(double a, double b);

// a number string, base = 16
std::string generate_token(std::size_t len);