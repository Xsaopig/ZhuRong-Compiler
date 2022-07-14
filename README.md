# 重新开始中间代码生成
放弃LLVM IR，采用四元式

## 配置了debug文件

vscode环境下工作目录为ZhuRong-Complier文件夹时，F5可自动调试

## 修改了项目结构

函数调用关系如下

main->ASTBuilder.Build()

    |   |->printAST()

    |   |->calAttr()    //新增函数负责计算AST结点属性

    |   `->ASTtoSymtab()

    |

    `->IRBuilder.Build()

        `->genIR()


## 完善了类型系统

## 完成了部分语义检查
已完成：

+ 调用尚未定义的符号
+ 函数调用的实参与形参是否匹配
+ 赋值语句的隐式类型转换
+ 数组的非法调用

未完成：
+ 重复定义符号
+ 带初始化的声名语句的隐式类型转换