#pragma once
#include <string>
#include <vector>
#include <iostream>
#define UESE_PRIVATE_LOG
#ifdef USER_PRIVATE_LOG
#define ERR_LOG     std::cout << "PRIVATE_LOG_ERR__|"
#define DEBUG_LOG   std::cout << "PRIVATE_LOG_DEBUG|"
#define INFO_LOG    std::cout << "PRIVATE_LOG_INFO_|"
#define FATAL_LOG   std::cout << "PRIVATE_LOG_FATAL|"
#endif
