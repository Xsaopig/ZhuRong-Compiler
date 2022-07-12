IRv1版本不可用
仅完成genIR（）部分，剩余很多功能函数没实现

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