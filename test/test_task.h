#pragma once
#include "../frame/task.h"

/* task used for unittask. */
class RecallTask : public ::inf::frame::UnitTask {
public:
    virtual bool run(void *data) const{
        auto data_ptr = static_cast<std::string*>(data);
        data_ptr->append("world");
        return true;
    }

    virtual bool init(const YAML::Node &conf_info) {
        if (!::inf::frame::UnitTask::init(conf_info)) {
            return false;
        }
        return true;
    }
        
};


/* task used for unittask. will be failing
*/

class SortTask : public ::inf::frame::UnitTask {
public:
    virtual bool run(void *data) const{
        return false;
    }

    virtual bool init(const YAML::Node &conf_info) {
        if (!inf::frame::UnitTask::init(conf_info)) {
            return false;
        }
        return false;
    }
        
};

