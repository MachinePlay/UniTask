#pragma once
#include "strategy_creator.h"

namespace inf {
namespace frame {
/** 
 * @class RecTaskFactory.
 **/
class RecTaskFactory : public Factory<Task, BaseTaskCreator> {
public:
    /* singleton. */
    static RecTaskFactory& instance() {
        static RecTaskFactory factory_instance();
        return factory_instance;
    }

    virtual bool init() override;
    
private:
    
};

} // end namespace frame
} // end namespace inf
