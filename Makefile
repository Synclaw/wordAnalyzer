# 编译器配置
CXX = g++

# 项目结构
SRC_DIR = src
BUILD_DIR = build
EXEC = wordanalyzer

# 源文件清单
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# 默认目标
all: $(BUILD_DIR) $(EXEC)

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 可执行文件生成规则
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 通用编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 显式声明头文件依赖
$(BUILD_DIR)/main.o: include/token.h include/tokenizer.h
$(BUILD_DIR)/tokenizer.o: include/token.h include/tokenizer.h

# 清理规则
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all clean