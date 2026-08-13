#include <iostream>

// void process_data(int *ptr) {
//     std::cout << "正在处理数据" << std::endl;
//     *ptr = 100;
// }

void process_data(int *ptr) {
    printf("正在处理数据...\n");
    if (ptr == NULL) { // 修复点：增加空指针检查
        printf("警告：指针为空，放弃处理！\n");
        return;
    }
    *ptr = 100;
}

int main() {
    int *data_ptr = nullptr;
    std::cout << "程序开始运行" << std::endl;
    process_data(data_ptr);
    std::cout << "程序运行结束" << std::endl;
    return 0;
}