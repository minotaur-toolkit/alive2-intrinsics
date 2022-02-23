// Copyright (c) 2021-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.
// Version: February 23, 2022

#include <iostream>

struct ProgressBar {
  ProgressBar(int max) : maxProgress(max) { update(0); }

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
  const int maxProgress;
  const int barWidth = 70;
};
