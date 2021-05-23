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


template <typename UnitTaskCreator>
class TaskSchedulerManager {
public:
    static TaskSchedulerManager& instance() {
        static TaskSchedulerManager instance;
        return instance;
    }
 
    /*load conf*/
    bool init(const std::string &conf_path, const std::string &command) {
        try {
            SchedulerLoaderPtr scheduler_loader(new SchedulerLoader(conf_path));
            _scheduler_double_buffer_ptr = SchedulerDoubleBufferPtr(
                    new SchedulerDoubleBuffer(std::move(scheduler_loader)));
        
            return _scheduler_double_buffer_ptr->init(
                    util::DoubleBufferConfigureManager::instance().get_configure_node(command));
        } catch (const std::exception &e) {
            ERR_LOG << e.what() << "|" << conf_path << std::endl;
            return false;
        } catch (...) {
            ERR_LOG << "unknown exception|" << conf_path << std::endl;
            return false;
        }
    }

    /*get scheduler by name from double buffer. */
    const TaskScheduler<UnitTaskCreator>* get_scheduler(
            const std::string &schedule_name) const {
        const HashTaskSchedulerPtrPtr &task_scheduler_table = 
                _scheduler_double_buffer_ptr->get_buffer();
        if (!task_scheduler_table) {
            ERR_LOG << "_scheduler_double_buffer_ptr.get_buffer() failed.\n";
            return nullptr;
        }

        typename HashTaskSchedulerPtr::const_iterator iter = 
                task_scheduler_table->find(schedule_name);
        if (iter == task_scheduler_table->end()) {
            return nullptr;
        }

        return iter->second.get();
    }

private:
    typedef std::unique_ptr<TaskScheduler<UnitTaskCreator> >  TaskSchedulerPtr;
    typedef std::unordered_map<std::string, TaskSchedulerPtr> HashTaskSchedulerPtr;
    typedef std::unique_ptr<HashTaskSchedulerPtr>             HashTaskSchedulerPtrPtr;

    /* ctor */
    TaskSchedulerManager() {}
    
    /* None-copy */
    TaskSchedulerManager(const TaskSchedulerManager &rhs);
    TaskSchedulerManager& operator=(const TaskSchedulerManager &rhs);

    /* Schedule Loader */
    class SchedulerLoader {
    public:
        /* Constructor */
        SchedulerLoader(const std::string &scheduler_conf) : 
                _scheduler_conf(scheduler_conf) {}

        /* Destructor*/
        ~SchedulerLoader() {}

        /* Scheduler table load */
        HashTaskSchedulerPtrPtr load() const {
            try {
                HashTaskSchedulerPtrPtr task_scheduler_table(new HashTaskSchedulerPtr());

                YAML::Node conf = YAML::LoadFile(_scheduler_conf.c_str());
                for (uint32_t i = 0; i < conf.size(); ++i) {
                    TaskSchedulerPtr new_task_schedule_ptr(new TaskScheduler<UnitTaskCreator>());
                    if (!new_task_schedule_ptr->init(conf[i])) {
                        ERR_LOG << "Failed to init TaskScheduler : " << i << std::endl;
                        return nullptr;
                    }

                    if (task_scheduler_table->find(
                            new_task_schedule_ptr->get_name()) != task_scheduler_table->end()) {
                        ERR_LOG << "Exist duplicated schedule_name : " << 
                                new_task_schedule_ptr->get_name() << std::endl;
                        return nullptr;
                    }
                    
                    task_scheduler_table->insert(
                            std::make_pair(new_task_schedule_ptr->get_name(), 
                            std::move(new_task_schedule_ptr)));
                }

                return task_scheduler_table;
            } catch (const std::exception &e) {
                ERR_LOG << e.what() << std::endl;
                return nullptr;
            } catch (...) {
                ERR_LOG << "unknown exception." << std::endl;
                return nullptr;
            }
        }
    
    private:
        /* None copy declare*/
        SchedulerLoader(const SchedulerLoader &rhs);
        SchedulerLoader& operator=(const SchedulerLoader &rhs);

        /* scheduler conf */
        std::string _scheduler_conf;
    };

    /*scheduler loader unique_ptr */
    typedef std::unique_ptr<SchedulerLoader> SchedulerLoaderPtr;

    /* Double buffer unique_ptr define */
    typedef inf::util::DoubleBuffer<HashTaskSchedulerPtr, SchedulerLoader> SchedulerDoubleBuffer;
    typedef inf::std::unique_ptr<SchedulerDoubleBuffer>                    SchedulerDoubleBufferPtr;

    /*scheduler double buffer structure */
    SchedulerDoubleBufferPtr _scheduler_double_buffer_ptr;
};

//task manager 

} // end namespace frame
} // end namespace inf
