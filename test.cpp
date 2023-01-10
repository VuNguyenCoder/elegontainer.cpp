#include <gtest/gtest.h>

#include "stlwrapper/container/map.h" 
#include "stlwrapper/container/vector.h" 

struct TestData { 
    float num; 
    std::string s; 
};

bool operator==(const TestData& d1, const TestData& d2) { 
    return d1.num == d2.num && d1.s == d2.s;
}

std::ostream& operator<<(std::ostream& out, const TestData& v) {
    out << "{" << v.num << ", " << v.s << "}";
    return out;
}

TEST(Common, STLWrapper_vector) {
    // Test with primitive item
    asilla::vector<int> number_vec = {1,2,3,4};
 
    EXPECT_EQ(number_vec.map([&](auto& value, auto index) { return value * 2; }),
                asilla::vector<int>({2,4,6,8}));

    EXPECT_EQ(number_vec.filter([&](auto& value, auto index) { return value % 2 == 0; }),
                asilla::vector<int>({2,4}));
   
    EXPECT_EQ(number_vec.reduce([&](auto& total_value, auto& value, int index){ return total_value + value; }, 0), 10);
    EXPECT_EQ(number_vec.to_string(), "{1, 2, 3, 4}");

    // Test with struct 
    asilla::vector<TestData> struct_vec = {{1, "ab"}, {2, "cd"}, {3, "ef"}, {4, "gh"}};
 
    EXPECT_EQ(struct_vec.map<float>([&](auto& value, auto index) { return value.num * 3; }),
                asilla::vector<float>({3, 6, 9, 12}));
    EXPECT_EQ(struct_vec.map<std::string>([&](auto& value, auto index) { return value.s.substr(0,1); }),
                asilla::vector<std::string>({"a", "c", "e", "g"}));

    EXPECT_EQ(struct_vec.filter([&](auto& value, auto index) { return int(value.num) % 2 == 0; }),
                asilla::vector<TestData>({{2, "cd"},{4, "gh"}}));
   
    EXPECT_EQ(struct_vec.reduce<int>([&](auto& total_value, auto& value, int index){ return total_value * value.num; }, 1), 24);
    EXPECT_EQ(struct_vec.to_string(), "{{1, ab}, {2, cd}, {3, ef}, {4, gh}}");
}

TEST(Common, STLWrapper_map) {
    // Test with primitive 
    asilla::map<int, int> number_map = {{1, 100}, {2, 200}, {3, 300}, {4, 400}};  
    number_map.remove_if([&](auto& value, auto& key, auto& map) { return value % 200 == 0; });
    
    asilla::map<int, int> number_map2 = {{1, 100}, {3, 300}};
    EXPECT_EQ(number_map, number_map2);

    // Test with string key and struct value 
    asilla::map<std::string, TestData> struct_map = {{"ab", {1, "xxx"}}, 
                                                 {"cd", {2, "yyy"}}, 
                                                 {"ef", {3, "zzz"}}};
    EXPECT_EQ(struct_map.reduce<std::string>([&](auto& total_value, auto& value, auto& key){
        return total_value + key + value.s;
    }, "") , "efzzzcdyyyabxxx");

    EXPECT_EQ(struct_map.to_string(), "{{ef, {3, zzz}}, {cd, {2, yyy}}, {ab, {1, xxx}}}");
}
