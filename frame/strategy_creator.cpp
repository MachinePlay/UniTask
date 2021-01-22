#include "strategy_creator.h"
#include "utils/common_log.h"
#include "task_register_manager.h"


namespace inf {
namespace frame {
TaskPtr UnitTaskCreator::create(const YAML::Node &conf) const {
    try {
        std::string task_name = conf["task_name"].as<std::string>();
        TaskPtr new_task = RecTaskFactory::instance().create(task_name);
        if (!new_task) {
            ERR_LOG << "Create Task Failed, task name : " << task_name << std::endl;
            return TaskPtr(nullptr);
        }
        return new_task;
    } catch (const std::exception &e) {
        ERR_LOG << e.what() << std::endl;
        return TaskPtr(nullptr);
    } catch (...) {
        ERR_LOG << "Unknown exception" << std::endl;
        return TaskPtr(nullptr);
    }
}

} // end namespace frame
} // end namespace inf