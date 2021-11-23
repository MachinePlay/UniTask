#pragma once
#include "strategy_creator.h"
#include "task.h"
namespace inf {
namespace frame {
/** 
 * @class RecTaskFactory.
 **/
class RecTaskFactory : public Factory<BaseTaskCreator, BaseTask> {
public:
    /* singleton. */
    static RecTaskFactory& instance() {
        static RecTaskFactory factory_instance;
        return factory_instance;
    }

    virtual bool init();
    
private:
    
};

} // end namespace frame
} // end namespace inf
