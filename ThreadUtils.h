/*
 * ThreadUtils.h
 *
 *  Created on: Aug 7, 2016
 *      Author: helospark
 */

#ifndef THREADUTILS_H_
#define THREADUTILS_H_
#include <map>
#include <string>
#include <thread>
#include "math.h"

namespace ThreadUtils {
  inline int getNumberOfCores() {
    int numberOfCores = std::thread::hardware_concurrency();
    if (numberOfCores <= 0) {
      numberOfCores = 1;
    }
    return numberOfCores;
  }

  inline int getThreadCountFromArgument(const std::map<std::string, std::string>& map) {
    int threads = 0;
    auto it = map.find("threads");
    int numberOfCores = getNumberOfCores();
    if (it != map.end()) {
      std::string threadCount = (*it).second;
      threads = Math::parseIntOrGet(threadCount, numberOfCores);
    }
    if (threads <= 0) {
      threads = numberOfCores;
    }
    std::cout << "T=" << threads << std::endl;
    return threads;
  }
}



#endif /* THREADUTILS_H_ */
