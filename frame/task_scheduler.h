#include "task.h"

namespace inf {
namespace frame {

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
       
   }


    /**
    * execute the task. by tasks
    * @param 
    * @return 
    */


private:
    /* scheduler name. */
    std::string             _scheduler_name{""};
    
    /* tasks of the scheduler. */
    std::vector<std::string> _tasks;
    
    /* skip failure flag. */
    int64_t                 skip_failure{0};
};

} // end namespace frame
} // end namespace inf