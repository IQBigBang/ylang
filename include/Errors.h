#ifndef ERRORS_H_
#define ERRORS_H_

#include <string>

void err_signal_handle(int signo);

namespace err {
    // throw, but don't terminate unless too many exceptions have already been thrown
    void throwNonfatal(std::string error, std::string description = "", int line = -1);
    // throw and terminate
    void throwFatal(std::string error, std::string description = "", int line = -1);
}

#endif /* ERRORS_H_ */