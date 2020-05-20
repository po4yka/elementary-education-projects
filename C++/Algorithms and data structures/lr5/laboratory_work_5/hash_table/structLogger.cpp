#include "structLogger.h"

/* ----- STEP-BY-STEP ----- */

void StructLogger::loopLatency() {
    qDebug() << "Loop latency for step-by-step STARTED" << endl;
    for( ; ; ) {
        QApplication::processEvents();
        if(stepLoopSwitcher == false) break;
    }
    qDebug() << "Loop latency for step-by-step ENDED" << endl;
    stepLoopSwitcher = true;
}

void StructLogger::disableLoopLatency() {
    stepLoopSwitcher = false;
}

void StructLogger::enableLoopLatency() {
    stepLoopSwitcher = true;
}

bool StructLogger::getLogMode() const {
    return logMode;
}

void StructLogger::setLogMode(bool value) {
    logMode = value;
}

bool StructLogger::getStepByStepMode() const {
    return stepByStepMode;
}

void StructLogger::setStepByStepMode(bool value) {
    stepByStepMode = value;
}
