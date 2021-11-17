#ifndef INCLUDE_FLAMEGPU_SIM_SIMLOGGER_H_
#define INCLUDE_FLAMEGPU_SIM_SIMLOGGER_H_

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>

#include "flamegpu/sim/LogFrame.h"

namespace flamegpu {

class RunPlanVector;

/**
 * This class is used by CUDAEnsemble::simulate() to collect logs generated by each of the SimRunner instances executing in different threads and write them to disk
 */
class SimLogger {
    friend class CUDAEnsemble;
    /**
     * Constructs a new SimLogger
     * @param run_logs Reference to the vector to store generate run logs
     * @param run_plans Reference to the vector of run configurations to be executed
     * @param out_directory The directory to write logs to disk
     * @param out_format The format to write logs to disk.
     * @param log_export_queue The queue of logs to exported to disk
     * @param log_export_queue_mutex This mutex must be locked to access log_export_queue
     * @param log_export_queue_cdn The condition is notified every time a log has been added to the queue
     * @param _export_step If true step logs will be exported
     * @param _export_exit If true exit logs will be exported
     */
    SimLogger(const std::vector<RunLog> &run_logs,
        const RunPlanVector &run_plans,
        const std::string &out_directory,
        const std::string &out_format,
        std::queue<unsigned int> &log_export_queue,
        std::mutex &log_export_queue_mutex,
        std::condition_variable &log_export_queue_cdn,
        bool _export_step,
        bool _export_exit);
    /**
     * The thread which the logger is executing on, created by the constructor
     */
    std::thread thread;
    /**
     * Starts the SimLogger running in thread
     */
    void start();
    // External references
    /**
     * Reference to the vector to store generate run logs
     */
    const std::vector<RunLog> &run_logs;
    /**
     * Reference to the vector of run configurations to be executed
     */
    const RunPlanVector &run_plans;
    /**
     * The directory to write logs to disk
     */
    const std::string &out_directory;
    /**
     * The format to write logs to disk.
     */
    const std::string &out_format;
    /**
     * The queue of logs to exported to disk
     */
    std::queue<unsigned int> &log_export_queue;
    /**
     * This mutex must be locked to access log_export_queue
     */
    std::mutex &log_export_queue_mutex;
    /**
     * The condition is notified every time a log has been added to the queue
     */
    std::condition_variable &log_export_queue_cdn;
    /**
     * If true step log files will be exported
     */
    bool export_step;
    /**
     * If true exit log files will be exported
     */
    bool export_exit;
};

}  // namespace flamegpu

#endif  // INCLUDE_FLAMEGPU_SIM_SIMLOGGER_H_
