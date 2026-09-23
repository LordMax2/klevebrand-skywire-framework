#pragma once

#ifndef CONCEPTS_H
#define CONCEPTS_H

template<typename T, typename U>
concept same_as = __is_same(T, U);

#endif
