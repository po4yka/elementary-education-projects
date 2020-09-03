#ifndef STRUCTLOGGER_H
#define STRUCTLOGGER_H

#include "allheaders.h"

class StructLogger : public QObject
{
    Q_OBJECT

    public:
        /**
         * @brief makeLogMessage
         * Simple message like for constructor
         */
        virtual void makeLogMessage(std::string) = 0;

        /**
         * @brief makeLogMessage
         */
        virtual void makeLogMessage(std::string, std::string) = 0;

        virtual void makeLogMessage(std::string, size_t, size_t) = 0;

        // STEP-BY-STEP
        void loopLatency();
        bool getLogMode() const;
        void setLogMode(bool);
        bool getStepByStepMode() const;
        void setStepByStepMode(bool);

    private:
        // STEP-BY-STEP
        bool logMode{false};            // mode of logging
        bool stepByStepMode{false};     // switch on/off

        char m_padding[5];

    protected:
        bool stepLoopSwitcher{false};   // switch loop state

    signals:
        void printOutput();

    public slots:
        void disableLoopLatency();
        void enableLoopLatency();
};

#endif // STRUCTLOGGER_H
