#pragma once

constexpr long FALSE{0};
constexpr long TRUE{-1};

#define LOGICAL ? TRUE : FALSE
#define LOWER(x, y) ((unsigned long)(x) < (unsigned long)(y))
