#pragma once

#include <string>

std::string parseBlameBasicTestData = R"(hash0hash0hash0  1 1 1
author A
	test line
hash1hash1hash1  2 2 1
author B
	  test line
hash2hash2hash1  3 3 1
author C
hash0hash0hash0 4 4 1
	
hash1hash1hash1 6 5 1
	  test line
hash0hash0hash0 6 6 3
	  test line
hash0hash0hash0 7 7
	  test line
hash0hash0hash0 8 8
	  test line
hash3hash3hash3 10 9 1
author D D
	
hash1hash1hash1 11 10 1
	  test line
hash0hash0hash0 11 11 3
	  test line
hash0hash0hash0 12 12
	  test line
hash0hash0hash0 13 13
	  test line
hash3hash3hash3 15 14 1
	
hash0hash0hash0 15 15 4
	  test line
hash0hash0hash0 16 16
	  test line
hash0hash0hash0 17 17
	test line
)";