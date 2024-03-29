#include "include/EasyIni/EasyIni.hpp"
#include <iostream>

template<typename T>
inline void vectorPrint(const std::vector<T>& vec){
    for (const auto& item : vec){
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    try{
        EasyIni::Configuration config("Config.ini");

        auto Int = config["BASETYPES"]["Int"].get<int>();
        auto String = config["BASETYPES"]["String"].get<std::string>();
        auto Float = config["BASETYPES"]["Float"].get<float>();
        std::cout << "Int: " << Int << std::endl;
        std::cout << "String: " << String << std::endl;
        std::cout << "Float: " << Float << std::endl;

        auto IntVector = config["VECTORTYPES"]["IntVector"].getVector<int>();
        auto StringVector = config["VECTORTYPES"]["StringVector"].getVector<std::string>();
        auto FloatVector = config["VECTORTYPES"]["FloatVector"].getVector<float>();
        vectorPrint(IntVector);
        vectorPrint(StringVector);
        vectorPrint(FloatVector);

        config.save("ExportedConfig.ini");

        config["BASETYPES"]["Int"] = 10;
        config["BASETYPES"]["String"] = "I am A string";
        config["BASETYPES"]["Float"] = 6.85698f;

        config["VECTORTYPES"]["IntVector"] = std::vector<int>{1, 2, 3, 4, 5};
        config["VECTORTYPES"]["StringVector"] = std::vector<std::string>{"Hello", "World", " Mom ", "Is", "A", "Nice", "Lady"};
        config["VECTORTYPES"]["FloatVector"] = std::vector<float>{1.1f, 2.2f, 3.3f, 4.4f, 5.5f};

        config.save("EditedConfig.ini");

        config["NEWSECTION"]["NewInt"] = 10;
        config["NEWSECTION"]["NewString"] = "Epic string";
        config["NEWSECTION"]["NewFloat"] = 89.654f;
        config.save();

        EasyIni::Configuration config2("EditedConfig.ini");
        Int = config2["BASETYPES"]["Int"].get<int>();
        String = config2["BASETYPES"]["String"].get<std::string>();
        Float = config2["BASETYPES"]["Float"].get<float>();
        std::cout << "Int: " << Int << std::endl;
        std::cout << "String: " << String << std::endl;
        std::cout << "Float: " << Float << std::endl;

        IntVector = config2["VECTORTYPES"]["IntVector"].getVector<int>();
        StringVector = config2["VECTORTYPES"]["StringVector"].getVector<std::string>();
        FloatVector = config2["VECTORTYPES"]["FloatVector"].getVector<float>();
        vectorPrint(IntVector);
        vectorPrint(StringVector);
        vectorPrint(FloatVector);

        auto Int2 = config2["NOT_EXIST"]["Width"].getOrDefault<int>(800);
        auto String2 = config2["NOT_EXIST"]["Name"].getOrDefault<std::string>("Default");
        auto Float2 = config2["NOT_EXIST"]["Height"].getOrDefault<float>(600.0f);
        std::cout << "Int2: " << Int2 << std::endl;
        std::cout << "String2: " << String2 << std::endl;
        std::cout << "Float2: " << Float2 << std::endl;

        auto IntVector2 = config2["NOT_EXIST"]["IntVector"].getVectorOrDefault<int>({5, 7, 8, 23});
        auto StringVector2 = config2["NOT_EXIST"]["StringVector"].getVectorOrDefault<std::string>({"Life", "Is", "Good"});
        auto FloatVector2 = config2["NOT_EXIST"]["FloatVector"].getVectorOrDefault<float>({1.1f, 2.2f, 3.3f, 4.4f, 5.5f});
        vectorPrint(IntVector2);
        vectorPrint(StringVector2);
        vectorPrint(FloatVector2);

        config2.save();





    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }
    return 0;
}


