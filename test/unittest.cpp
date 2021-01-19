#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"
#include "frame/task_data.h"
#include "frame/task.h"
#include "test_task.h"
#include <string>
#include <iostream>
#include <memory>
#include <cstdint>

/**
 * @class FrameTest
 * @desc 测试DemoServer环境初始化类
 */
class FrameTest : public testing::Environment {
public:
    /* init test environment. */
    virtual void SetUp() {};

    /* clear test environment. */
    virtual void TearDown() {}; 
};

/*
 * @class TestDictManager
 */
class TestFrame : public testing::Test {
public:
    /* start test. */
    static void SetUpTestCace() {
        std::cout << "StartFrameTesting." << std::endl;
    }
    
    /* end test. */
    static void TearDownTestCase() {
        std::cout << "TearDownFrameTesting." << std::endl;
    }

};

// test 
TEST_F(TestFrame, test_gtest)  {
    ASSERT_EQ(1, 1);
}


//test TaskData
TEST_F(TestFrame, test_TaskData)  {
    int *task_data_ptr = new int(100);
    ::inf::frame::TaskData<int> task(task_data_ptr);

    //get raw task_data
    auto raw_task_data = task.get_data();

    ASSERT_EQ(100, *raw_task_data);
    
}

//test TaskDataMap
TEST_F(TestFrame, test_TaskDataMap)  {
     //data map
    ::inf::frame::TaskDataMap data_map;
    std::string number_key = "my_string";
    int *task_data_ptr2 = new int(10000);
    data_map.insert<int>(number_key, task_data_ptr2);
    auto data = data_map.find<int>(number_key);
    ASSERT_EQ(*task_data_ptr2, *data);

    //different type. string
    std::string user_one = "i am user info.";
    std::string *user_info_ptr  =  new std::string(user_one);
    std::string user_info_key = "user_info";
    data_map.insert<std::string>(user_info_key, user_info_ptr);
    auto user_ptr = data_map.find<std::string>(user_info_key);
    ASSERT_EQ(user_one, *user_ptr);

    //complex data type
    auto feature_ptr = new std::unordered_map<std::string, std::string>();
    feature_ptr->insert(std::make_pair(user_one, user_one));
    std::string feature_key = "feature_key";
    data_map.insert<std::unordered_map<std::string, std::string>>(feature_key, feature_ptr);
    
    auto feature_data_ptr = data_map.find<std::unordered_map<std::string, std::string>>(feature_key);
    ASSERT_EQ(user_one, (*feature_data_ptr)[user_one]);

    // erase 
    data_map.erase(number_key);
    auto ret = data_map.find<int>(number_key);
    ASSERT_EQ(nullptr, ret);
    

}

TEST_F(TestFrame, test_Task) {
    const std::string task_conf = "../conf/task.yaml";
    auto conf = YAML::LoadFile(task_conf.c_str());
    std::cout << conf["task_name"].as<std::string>() << std::endl;
    RecallTask recall_task;
    recall_task.init(conf);
    std::unique_ptr<std::string> ptr(new std::string("hello"));
    void *raw_ptr = static_cast<void*>(ptr.get());
    std::cout << "here" << std::endl;
    recall_task.run(raw_ptr);
    std::string expected_str = "helloworld";
    ASSERT_EQ(conf["task_name"].as<std::string>(), recall_task.get_task_name());
    

}



// TEST_F(TestDict,  test_parser_manager) {
//    std::shared_ptr<gcs::parser::MessageParserManager> msg_manager_ptr = std::make_shared<gcs::parser::MessageParserManager>();
//    /* test none-exist key. */
//    gcs::parser::parser_func_t msg_parser =  msg_manager_ptr->get_parser_func("empty_key");
//    EXPECT_EQ(msg_parser, nullptr);

// }
const std::string DB_CONF = "../conf/config.yaml";
int main(int argc, char* argv[]) {
    testing::AddGlobalTestEnvironment(new FrameTest);
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
