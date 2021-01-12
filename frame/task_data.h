#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace inf {
namespace frame {

/** 
 * @class BaseTaskData.
 * all the task implement this base task.
 * for pointer to use.
 **/
class BaseTaskData {
public: 
    BaseTaskData() = default;
    virtual ~BaseTaskData() = default;
};

using BaseTaskDataPtr = std::unique_ptr<BaseTaskData>;

/** 
 * @class TaskData.
 * @bref real task data. get_data() returns the raw DataType*
 **/
template <typename DataType>
class TaskData : public BaseTaskData {
public:
    /* ctor. */
    explicit TaskData(DataType *data = nullptr) : _data(data){};
    /* dtor. */
    virtual ~TaskData() = default;


    /* get data raw pointer. */
    DataType* get_data() {
        return _data.get();
    }

    /* get const data raw pointer. */
    const DataType* get_data() const {
        return _data.get();
    }


private:
    using DataPtr = std::unique_ptr<DataType>;
    
    /* none copy. */
    TaskData(const TaskData &rhs) = delete;
    TaskData &operator=(const TaskData &rhs) = delete;

    /* real data. */
    DataPtr _data;
    
};



/** 
 * @class TaskDataMap.
 * store multi task , unorder_map <string, BaseTaskDataPtr>
 * can store any data type, because data will be stored by BaseTaskDataPtr.
 * get the task inherent class pointer.
 **/
template <typename KeyType = std::string>
class TaskDataMap {
using DataMap = std::unordered_map<KeyType, BaseTaskDataPtr>;
public:
/**
* insert DataType* into table
* may be store the request data, feature data, user data, etc. can be found by name.
* @param key the data name.
* @param data raw_data' pointer, which will be stroed as TaskData in the map
* @return data; original data pointer, whith is the DataType* data if inserted, otherwise nullptr.
*/
template <typename DataType> 
DataType* insert(KeyType &key, DataType* data) {
    auto it = _data_table.find(key);
    if (it == _data_table.end()) {
        BaseTaskDataPtr task_data = std::make_unique<TaskData<DataType>>(data);
        _data_table.insert(std::make_pair(key, std::move(task_data)));
    } else {
        //data already exist, of no data or data has erased
        if (nullptr != data) {
            delete data;
        }
        return nullptr;
    }
    return data;
}

/**
* find the task_data by name from the data_table.
* @param key key of the data.
* @return data, be converted to DataType*, null ptr if not exist.
*/
template <typename DataType>
DataType* find(const KeyType &key) {
    auto it = _data_table.find(key);
    if (it == _data_table.end()) {
        return nullptr;
    }
    
    TaskData<DataType> *task_data = 
        static_cast<TaskData<DataType>*>(it->second.get());
    //task_data is TaskData, must return TaskData.get_data() (the raw data pointer)
    return task_data->get_data();
}

/**
* find the task_data by name from the data_table. const version
* @param key key of the data.
* @return data, be converted to DataType*, nullptr if not exist.
*/
template <typename DataType>
const DataType* find(const KeyType &key) const{
    auto it = _data_table.find(key);
    if (it == _data_table.end()) {
        return nullptr;
    }
    
    TaskData<DataType> *task_data = 
        static_cast<TaskData<DataType>*>(it->second.get());
    //task_data is TaskData, must return TaskData.get_data() (the raw data pointer)
    return task_data->get_data();
}


/**
* erase data by key name, the data will be removed from the table.
* @param key key of the data
* @return bool true if erase. othrer wise false.
*/
bool erase(const KeyType &key) {
    auto it = _data_table.find(key);
    if (it == _data_table.end()) {
        // key not exist
        return false;
    }

    _data_table.erase(key);

    return true;
}


private: 
    /* data map <string, BaseTaskDataPtr>. */
    DataMap             _data_table;
};



} // end name space frame
} // end namespace inf