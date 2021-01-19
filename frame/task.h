#pragma once
#include "../utils/common_log.h"
#include "yaml-cpp/yaml.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace inf{
namespace frame {

/* task type. */
enum TaskType {
    UNIT_TASK       = 0;
    SERIAL_TASK     = 1;
    PARALLEL_TASK   = 2;
    DEFAULT_TASK    = 3;
};


/** 
 * @class TaskBase.
 * task executor base class , task implement this task's init() and run() function
 * during the framework, task will be run one by one or parallel
 **/
class BaseTask {
public:
    /* ctor. */
    explicit BaseTask(TaskType task_type = DEFAULT_TASK) : _task_type(task_type) {};
    
    /* dtor. */
    virtual ~BaseTask() = default;

    /**
    * init the task info, each task can read it's own config, and do some bussiness 
    * @param conf_info yaml node info, store the task config
    * @return bool true : ok, otherwise false;
    */
    virtual bool init(const Yaml::Node &conf_info) {
        try {
            _task_name = conf_info['task_name'].as<std::string>();
        } catch (const std::exception &e) {
            ERR_LOG << e.what() << std::endl;
            return false;
        } catch (...) {
            ERR_LOG << "unknow error" << std::endl;
            return false;
        }

        return true;
    }


    /**
    * execute the task instance
    * @param data the task data.(TaskDataMap*)
    * @return bool true: execute ok, false : failed
    */
    virtual run(void *data) const = 0;

    /**
    * retrieve task name
    * @return  task_name
    */
    std::string get_task_name() const{
        return _task_name;
    }

    /**
    * retrieve task type
    * @return task_type
    */
    TaskType get_task_type() const {
        return _task_type;
    }

private:
    /* none-copy. */
    BaseTask(const BaseTask &lhs) = delete;
    BaseTask &operator=(const BaseTask &lhs) = delete;
    
    /* real registered name of the task, can be found by name. */
    std::string _task_name;
    
    /* task type. */
    TaskType    _task_type;
};

using TaskPtr = std::unique_ptr<BaseTask>;

/** 
 * @class UnitTask.
 * @brief task be executed in a independent unit.
 **/
class UnitTask : public BaseTask {
public:
    /* ctor. */
    UnitTask() : BaseTask(UNIT_TASK) {};
    
    /**
    * initializing task by configuration
    * @param conf, yaml configure node
    * @return true : if init ok, otherwise false.
    * @note BaseTaskLoader::load will call UnitTask::init for all subclass of UnitTask.
    * so there is no need to init each UnitTask subclass manaunlly.
    */
   virtual bool init(const YAML::Node &conf_info) const = 0;


    /**
    * execute the task
    * @param data task_data
    * @return true if execute ok, otherwise false.
    */
   virtual bool run(void *data) const = 0;
};

/** 
 * @class SerialTask.
 * @brief task be executed in a serial.
 **/
class SerialTask : public BaseTask {
public:
    /* ctor. */
    SerialTask() : BaseTask(SERIAL_TASK) {};
    
    /**
    * initializing task by configuration
    * @param conf, yaml configure node
    * @return true : if init ok, otherwise false.
    * @note BaseTaskLoader::load will call SerialTask::init for all subclass of UnitTask.
    * so there is no need to init each UnitTask subclass manaunlly.
    */
   virtual bool init(const YAML::Node &conf_info) const = 0;


    /**
    * execute the task
    * @param data task_data
    * @return true if execute ok, otherwise false.
    */
   virtual bool run(void *data) const = 0;
};

/** 
 * @class ParallelTask.
 * @brief task be executed in a serial.
 **/
class ParallelTask : public BaseTask {
public:
    /* ctor. */
    ParallelTask() : BaseTask(PARALLEL_TASK) {};
    
    /**
    * initializing task by configuration
    * @param conf, yaml configure node
    * @return true : if init ok, otherwise false.
    * @note BaseTaskLoader::load will call ParallelTask::init for all subclass of UnitTask.
    * so there is no need to init each UnitTask subclass manaunlly.
    */
   virtual bool init(const YAML::Node &conf_info) const = 0;


    /**
    * execute the task
    * @param data task_data
    * @return true if execute ok, otherwise false.
    */
   virtual bool run(void *data) const = 0;
};




} // end namespace frame
} // end namespace inf


