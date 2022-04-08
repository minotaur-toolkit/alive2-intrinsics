#pragma once

// Copyright (c) 2021-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: April 7, 2022

#include <iostream>

struct ProgressBar {
  ProgressBar(int lowerBound, int upperBound) {
    // x>=9 && x<18 are mmx instructions, which must be ignored
    // Progress bar must have accurate progress accounting for this
    if (lowerBound < 9 && upperBound >= 18)
      maxProgress = (upperBound - lowerBound) - 9;
    else if (lowerBound < 9 && upperBound < 9)
      maxProgress = upperBound - lowerBound;
    else if (lowerBound >= 18 && upperBound >= 18)
      maxProgress = upperBound - lowerBound;
    else if (lowerBound >= 9 && upperBound >= 18)
      maxProgress = upperBound - 18;
    else if (lowerBound < 9 && upperBound < 18)
      maxProgress = 9 - lowerBound;
    else // Both are in MMX range, no work to be done
      maxProgress = 0;

    update(0);
  }

  void update(int currProgress) {
    progress = currProgress;

    std::cerr << "[";
    int pos = ((float)progress / maxProgress) * barWidth;

    for (int i = 0; i < barWidth; ++i) {
      if (i < pos)
        std::cerr << "=";
      else if (i == pos)
        std::cerr << ">";
      else
        std::cerr << " ";
    }

    std::cerr << "] " << int(((float)progress / maxProgress) * 100.0) << " %\r";
    std::cerr.flush();
  }

private:
  int progress = 0;
  int maxProgress;
  const int barWidth = 70;
};
