#pragma once
#include "utils/common_log.h"
#include "yaml-cpp/yaml.h"
#include "double_buffer.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdint.h>

namespace inf{
namespace frame {

/* task type. */
enum TaskType {
    UNIT_TASK       = 0,
    SERIAL_TASK     = 1,
    PARALLEL_TASK   = 2,
    DEFAULT_TASK    = 3,
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
    virtual bool init(const YAML::Node &conf_info) {
        try {
            _task_name = conf_info["task_name"].as<std::string>();
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
    virtual bool run(void *data) const = 0;

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
   virtual bool init(const YAML::Node &conf_info) override{
        return BaseTask::init(conf_info);
   }


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
    virtual bool init(const YAML::Node &conf_info) {
        return BaseTask::init(conf_info);
    }


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
    virtual bool init(const YAML::Node &conf_info) {
        return BaseTask::init(conf_info);
    }


    /**
    * execute the task
    * @param data task_data
    * @return true if execute ok, otherwise false.
    */
   virtual bool run(void *data) const = 0;
};

using TaskMap = std::unordered_map<std::string, TaskPtr>;
using TaskMapPtr = std::unique_ptr<TaskMap>;
/** 
 * @class TaskLoader.
 * double buffer task loader,
 * TODO: maybe should be defined as inner class of Manager?
 **/
template <typename UnitTaskCreator>
class TaskLoader {
public:
    /* ctor. */
    TaskLoader() = default;
    virtual ~TaskLoader() = default;

    /* init function, read file from yaml. */
    bool init(const std::string &file_name) {
        if (file_name.empty()) {
            return false;
        }
        _config_file_name = file_name;
        //whether the file exists
        struct stat file_stat;
        if (stat(file_name.c_str(), &file_stat) != 0) {
            return false;
        }
        return true;
    }

    /* config file name, user relative path*/
    std::string get_load_file_name() const {
        return _config_file_name;
    }

    /* load funtion, always load latest file, and parse data. */
    TaskMapPtr load() {
        try {
            UnitTaskCreator task_creator;
            TaskMapPtr task_table(new TaskMap());
            
            YAML::Node conf = YAML::LoadFile(_config_file_name.c_str());
            for (int64_t i = 0; i < conf.size(); ++i) {
                //create by alias name, but registered name is conf["task_name"]
                std::string task_alias_name = conf["task_alias_name"].as<std::string>();
                auto it = task_table->find(task_alias_name);
                if (it != task_table->end()) {
                    ERR_LOG << "Duplicate task alias name, alias : " << task_alias_name << std::endl;
                    return TaskMapPtr(nullptr);
                }
                
                // task can be create by conf, creator's proxy mode.
                TaskPtr task_ptr = task_creator.create(conf[i]);
                if (!task_ptr) {
                    ERR_LOG << "Create Task Failed, alias : << task_alias_name" << std::endl;
                    return TaskMapPtr(nullptr);
                }

                //task init by it's own conf
                if (!task_ptr->init(conf[i])) {
                    ERR_LOG << "Initialize Task Failed, alias :  << task_alias_name" << std::endl;
                    return TaskMapPtr(nullptr); 
                }

                task_table->insert(std::make_pair(task_alias_name, std::move(task_ptr)));
            }

            return task_table;
        } catch (const std::exception &e) {
            ERR_LOG << e.what() << std::endl;
            return TaskMapPtr(nullptr);
        } catch (...) {
            ERR_LOG << "Unknown Error" << std::endl;
        }
        
    }

    
private:
    /* none copy. */
    TaskLoader(const TaskLoader &rhs) = delete;
    TaskLoader &operator=(const TaskLoader &rhs) = delete;
    
    /* conf filename. */
    std::string _config_file_name {""};
};



/** 
 * @class TaskManager.
 * manage the task map, store the task rawtype.
 **/
template <typename UnitTaskCreator>
class TaskManager {
public:
    /* singleton. */
    static TaskManager& instance() {
        static TaskManager instance;
        return instance;
    }

    /* init task manager. */
    bool init(const std::string &file_path, const std::string &comand) {
        try {
            TaskLoaderPtr task_loader_ptr = std::make_unique<TaskLoader<UnitTaskCreator>>();
            //bind loader to conf
            task_loader_ptr->init(file_path);
            _task_double_buffer_ptr = TaskDoubleBuffer(std::move(task_loader_ptr));
            _task_double_buffer_ptr->init();
        } catch (const std::exception &e) {
            ERR_LOG << e.what() << std::endl;
            return false;
        } catch (...) {
            ERR_LOG << "Unknown Error" << std::endl;
            return false;
        }
    }

    /* get task instance by alias name. */
    const TaskPtr& get_task(const std::string& task_alias_name) const {
        const TaskMapPtr task_map = _task_double_buffer_ptr->get_current();
        if (!task_map) {
            ERR_LOG << " _task_double_buffer_ptr->get_current() Failed!" << std::endl;
            return _invalid_ptr;
        }
        auto it = task_map->find(task_alias_name);
        if (it == task_map->end()) {
            return _invalid_ptr;
        }

        return it->second;
    }

private:
    /* loader unique ptr. */
    using TaskLoaderPtr = std::unique_ptr<TaskLoader<UnitTaskCreator>>;
    /* double buffer of TaskMapPtr. */
    using TaskDoubleBuffer = ::inf::utils::DoubleData<TaskMapPtr, TaskLoader<UnitTaskCreator>>;
    using TaskDoubelBufferPtr = std::unique_ptr<TaskDoubleBuffer>;

    
    /* invalid ptr. */
    TaskPtr _invalid_ptr;
    
    /* task double buffer ptr. */
    TaskDoubelBufferPtr _task_double_buffer_ptr;

};





} // end namespace frame
} // end namespace inf


