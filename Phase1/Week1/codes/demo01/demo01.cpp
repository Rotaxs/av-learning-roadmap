#include <iostream>
#include <fstream>
#include <cstring>

void ofstreamTest() {
    std::ofstream file("./fstreamTest.txt");
    if (!file.is_open()) {
        std::cout << "file open failed" << std::endl;
        return;
    }
    file << "Hello world" << std::endl;
    const char* buf = "Hello fstream";
    file.write(buf, strlen(buf));
    file.close();
}

void ifstreamTest() {
    std::ifstream file("./fstreamTest.txt");
    if (!file.is_open()) {
        std::cout << "ifstreamTest: file open fialed" << std::endl;
        return;
    }

    std::string s;
    while (!file.eof()) {
        file >> s; // 以空白字符分隔
        std::cout << s << std::endl;
    }
    file.seekg(0, std::ios::beg); // 把文件位置指针移动到开头

    std::cout << "--------" << std::endl;
    char buf[512];
    file.read(buf, 512);
    std::cout << buf << std::endl;
    file.seekg(0, std::ios::beg);
    
    // 上面的会影响 getline 的使用
    // std::cout << "--------" << std::endl;
    // std::string str;
    // std::getline(file, str);
    // std::cout << str << std::endl;

    file.close();
}

void fstreamTest() {
    // out: 以只写的方式打开文件，如果不存在会创建文件，如果存在会将内容全部清除
    // in: 以只读的方式打开文件
    // app: 以只写的方式打开文件，并将内容追加到文件末尾
    // binary: 打开二进制文件
    // 不同的模式可以用 | 连接，比如 std::ios::in | std::ios::binary
    std::fstream file("./fstreamTest.txt", std::ios::in);
}

int main() {
    ofstreamTest();
    ifstreamTest();
    return 0;
}