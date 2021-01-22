#pragma once
#include "task.h"
#include "factory.h"
#include "yaml-cpp/yaml.h"

namespace inf {
namespace frame{
/** 
 * @class BaseTaskCreator.
 * create rec task creator base class
 * inherented this class to implement specific platform Crator.
 **/
class BaseTaskCreator {
    /* ctor. */
    BaseTaskCreator() = default;
    /* dtor. */
    virtual ~BaseTaskCreator() = default;

    /**
    * create a task 
    * @return TaskPtr
    */
   virtual TaskPtr create() = 0;
};



/** 
 * @class StandardTaskCreator.
 **/
template <typename TaskType>
class StandardTaskCreator : public BaseTaskCreator {
public:
    /** 
     * create a standard task.
     * @return task
     **/
    virtual TaskPtr create() override const {
        return TaskPtr(new TaskType);
    }
}


/** 
 * @class UnitTaskCreator.
 **/
class UnitTaskCreator {
public: 
    /**
    * create a task by configure
    * @param conf configure
    * @return task
    */
    virtual TaskPtr create(const YAML::Node &conf) const;
};



} // end namespace frame
} // end namespace inf