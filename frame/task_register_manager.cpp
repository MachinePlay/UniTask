#include "task_register_manager.h"

namespace inf {
namespace frame {

bool RecTaskFactory::init() {
    //register task creator name here
    // const std::string USER_FEATURE_TASK = "user_feature_task";
    // const std::string COMMON_RECALL_TASK = "common_recall_task";

    // create task, and implement the creator
    // _register<StandardTaskCreator<FeatureTask>>(USER_FEATURE_TASK);
    // _register<StandardTaskCreator<CommonRecallTask>>(COMMON_RECALL_TASK);
    return true;
};

} // end namespace frame
} // end namespace inf
