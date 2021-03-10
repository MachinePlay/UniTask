#include "task.h"

namespace inf {
namespace frame {

const int64_t SKIP_FAILURE_FLAG = 1;
/** 
 * @class TaskScheduler.
 * schedule the task by sequence
 **/
template <typename UnitTaskCreator>
class TaskScheduler {
public:
    /**
    * initialize the scheduler.
    * @param conf conf of the scheduler
    * @return bool true if ok, other false
    */
   bool init(const YAML::Node &conf) {
        try {
            if (!conf["scheduler_name"].IsDefined()) {
                ERR_LOG << "scheduler_name not define" << std::endl;
                return false;
            }
            _scheduler_name = conf["scheduler_name"].as<std::string>();
        
            //whether skip
            if (!conf["skip_failure"].IsDefined()) {
                ERR_LOG << "skip_failure no define" << std::endl;
                return false;
            }
            _skip_failure = conf["skip_failure"].as<int64_t>();

            //load task
            YAML::Node &task_config = conf["tasks"];

            _tasks.reserve(task_config.size());

            for (auto &task : task_config) {
                _task.push_back(task["task_alias_name"]).as<std::string>();
            }
        } catch (const std::exception &e) {
            ERR_LOG << e.what() << std::endl;
            return false;
        } catch (...) {
            ERR_LOG << "Unknown Exception" << std::endl;
            return false;
        }

        return true;
    

   }


    /**
    * execute the task. by tasks
    * @param data
    * @return true if ok, otherwise false.
    */
   bool schedule(void *data) const {
       if (data == nullptr) {
           ERR_LOG << "data is nullptr\n";
           return false;
       }

       if (_tasks.empty()) {
           ERR_LOG << "no task to execute" << _scheduler_name << std::endl;
       }

       //prepare task instance before task execute
       std::vector<Task*> task_executors;
       for (auto &task : _tasks) {
           const TaskPtr &task_ptr = TaskManager<UnitTaskCreator>::instance().get_task(task);
           if (task_ptr.empty()) {
               ERR_LOG << "not found task name : " task << std::endl;
               return false;
           } 
           task_executors.push_back(task_ptr.get());
        }


        // scheduler the task
        for (auto &task_instance : task_executors) {
            if (task_instance == nullptr) {
                return false;
            }
            
            auto ret = task_instance->run(data);
            // skip failure task if flag is open, otherwise not execute next task.
            if (_skip_failure != SKIP_FAILURA_FLAG) {
                ERR_LOG << "skip failure " << task->get_task_name() << std::endl;
                return false;
            }
        }
        
        return true;
   }

   /* get scheduler name. */
   std::string get_scheduler_name() const {
       return _scheduler_name;
   }



private:
    /* scheduler name. */
    std::string             _scheduler_name{""};
    
    /* tasks of the scheduler. */
    std::vector<std::string> _tasks;
    
    /* skip failure flag. */
    int64_t                 _skip_failure{0};
};


//task manager 

} // end namespace frame
} // end namespace inf
