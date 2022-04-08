#pragma once

// Copyright (c) 2021-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: April 7, 2022

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <ir/instr.h>
#include <string>
#include <vector>

struct CommandLineUtil {
  static bool useDebugMode;
  static unsigned lowerInclusiveBound;
  static unsigned upperExclusiveBound;
  static unsigned numberOfRepetitions;

  CommandLineUtil(int argc, char **argv) {
    if (argc < 2) {
      std::cerr << "ERROR: Needs atleast one argument. Add -h for help."
                << std::endl;
      exit(1);
    }

    for (int i = 1; i < argc; ++i) {
      std::string arg(argv[i]);
      // All args are made lowercase
      std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
      arguments.push_back(arg);
    }

    if (containsArg("-h") || containsArg("-help")) {
      printHelpMessage();
      exit(0);
    }

    setNumberOfRepetitions();
    useDebugMode = containsArg("-d") ? true : false;
    setRangeOfFunctionsToTest();
  }

private:
  // Arguments does not contain the name of the program as first argument
  std::vector<std::string> arguments;

  bool containsArg(std::string arg) {
    if (std::find(arguments.begin(), arguments.end(), arg) != arguments.end())
      return true;
    else
      return false;
  }

  inline bool isInteger(const std::string &s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
      return false;

    char *p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
  }

  void setNumberOfRepetitions() {
    if (!isInteger(arguments.at(0))) {
      std::cerr << "ERROR: First argument must be an integer." << std::endl;
      exit(1);
    }

    numberOfRepetitions = std::stoi(arguments.at(0));
  }

  void setRangeOfFunctionsToTest() {
    // Argument is in style of -r=10-40
    // Will test intrinsics between 10 inclusive and 40 exclusive

    unsigned indexOfArg = 10000;
    for (unsigned i = 0; i < arguments.size(); ++i) {
      if (arguments.at(i).at(0) == '-' && arguments.at(i).at(1) == 'r' &&
          arguments.at(i).at(2) == '=') {
        indexOfArg = i;
        break;
      }
    }

    // If not found
    if (indexOfArg == 10000) {
      lowerInclusiveBound = 0;
      upperExclusiveBound = IR::X86IntrinBinOp::numOfX86Intrinsics;
      return;
    }

    std::string arg = arguments[indexOfArg];

    // Find '-' character that seperates the beginning and end of range
    // +3 because first 3 characters are "-r="
    auto dashIterator = std::find(arg.begin() + 3, arg.end(), '-');
    if (dashIterator == arg.end()) {
      std::cerr << "Intrinsic range unable to be parsed, no '-' found."
                << std::endl;
      exit(1);
    }

    std::string lowerBound(arg.begin() + 3, dashIterator);
    std::string upperBound(dashIterator + 1, arg.end());

    lowerInclusiveBound = std::stoi(lowerBound);
    upperExclusiveBound = std::stoi(upperBound);

    if (upperExclusiveBound < lowerInclusiveBound) {
      std::cerr
          << "ERROR: Upper bound can not be lower or equal to the lower bound."
          << std::endl;
      exit(1);
    } else if (upperExclusiveBound > IR::X86IntrinBinOp::numOfX86Intrinsics) {
      std::cerr << "ERROR: Upper bound can not be higher than total number of "
                   "intrinsics."
                << std::endl;
      exit(1);
    }
  }

  void printHelpMessage() {
    std::cout
        << "This program runs random tests on the intrinsic semantics\n"
        << "implemented in Alive2. It compares the semantics against\n"
        << "the true output of the intrinsic call, and asserts that\n"
        << "they must be equal. If Alive2 states that they are not equal,\n"
        << "then there is a semantic issue with the implementation that\n"
        << "must be addressed.\n\n\n"
        << "First argument: number of tests to perform for each intrinsic.\n"
        << "Number of intrinsics available for testing: "
        << IR::X86IntrinBinOp::numOfX86Intrinsics << "\n\nFlags:\n\n"
        << "  -d\t\tDebug mode: prints the LLVM IR of all the "
           "function\n\t\tintrinsics, and counts how many duplicate "
           "(non-interesting)\n\t\tresults occurred. Disables progres bar.\n\n"
        << "  -r=x-y\tSets a range of the intrinsics to test,\n\t\tuseful for "
           "scripting. Usage: -r=40-50 (tests intrinsics\n\t\tbetween 40 "
           "inclusive and 50 exclusive)\n\n"
        << "  -h\t\tHelp information\n"
        << std::endl;
  }
};

// Must define static variables outside of class
bool CommandLineUtil::useDebugMode = false;
unsigned CommandLineUtil::lowerInclusiveBound = 0;
unsigned CommandLineUtil::upperExclusiveBound =
    IR::X86IntrinBinOp::numOfX86Intrinsics;
unsigned CommandLineUtil::numberOfRepetitions = 100;
