import re


test_string = "int main(void) { /* some code */ }"

# 调用函数进行替换
result = test_string.replace("main(void)", "main()")

# 打印结果
print(result)
