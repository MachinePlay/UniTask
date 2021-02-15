#pragma once
#include "utils/common_log.h"
#include <unordered_map>
#include <memory>



namespace inf {
namespace frame {
/** 
 * @class Factory.
 * abstract factory. register different task_type creator factory.
 * ProductType product = Creator.creat()
 **/
tempalte <typename CreatorType,
          typename ProductType,
          typename KeyType = std::string>
class Factory {
private:
    using CreatorPtr = std::unique_ptr<CreatorType>;
    using ProductPtr = std::unique_ptr<ProductType>;
    using CreatorTable = std::unordered_map<KeyType, CreatorPtr>;

public:
    /* ctor. */
    Factory() = default;
    /* dtor. */
    virtual ~Factory() = default;
    
    /**
    * init function, specific derived factory will use it to initilize 
    * @return bool true if init ok, otherwise fasle;
    */
    virtual bool init() = 0;
    
    virtual ProductPtr create(const KeyType &creator_key) const {
        auto it = _registered_table.find(creator_key);
        if (it == _registered_table.end()) {
            ERR_LOG << "Creator Not Exist, creator_key key : " << creator_key << std::endl;
            return ProductPtr(nullptr);
        } else {
            //create an Product use the creator.
            return it->second->create();
        }
    }
    
protected:
    /**
    * register specific creator by name
    * @param creator_key, name of the creator. 
    * @return bool true if register ok, otherwise else.
    * @note in fact, we registe the task_name as the task-creator name.
    * ConcreteCreator<ConcreteTask> to create task instance.
    */
    template<typename ConcreteCreatorType>
    bool _register(const KeyType& creator_key) {
        auto it = _registered_table.find(reator_key);
        if (it == _registered_table.end()) {
            CreatorPtr creator = std::make_unique<ConcreteCreatorType>();
            _registered_table.insert(std::make_pair(creator_key, std::move(creator)));
            return true;
        } else {
            ERR_LOG << "Duplicated creator key, key : " << creator_key << std::endl;
            return false;
        }
    }


private:
    /* none copy. */
    Factory(const Factory &lhs) = delete;
    Factory &opertor=(const Factory &lhs) = delete;
    
    /* registered Creator table. */
    CreatorTable _registered_table;
};


} // end namespace frame
} // end namespace inf
