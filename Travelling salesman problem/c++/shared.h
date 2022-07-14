#ifndef INC_1_SHARED_H
#define INC_1_SHARED_H

// input for Quick
#define inFileQuick "../test11.txt"
// input for для NonEff
#define inFileNonEff "../test11.txt"
// output for Quick
#define outFileQuick "../out_Quick.txt"
// output for Noneff
#define outFileNonEff "../out_NonEff.txt"
// compare for double
#define COMPARE_RATE 10e-5
// depth of for loop in sim alg. Change to optimize results, mainly time of execution
#define CYCLE_RATE 100
// zero division error
#define DIVISTION_ERROR 5
// colling rate of temperature. Change to optimize results
#define COOLING_RATE 0.1
// temperature max. Probably optimal
#define TEMP_MAX DBL_MAX
// temperature mim. Probably optimal
#define TEMP_MIN 0.00001
// depth of for loop to repeat simAnnealing() exec. Change to optimize results, mainly time of execution
#define REPEAT_RATE 10

#endif //INC_1_SHARED_H

#include <vector>
#include <algorithm>

void inputMatrix(std::vector<std::vector<double>> &matrix);

void MeasureFuncExecTime(const std::function<void()> &FuncToMeasure);

